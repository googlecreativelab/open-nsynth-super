/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Multisampler.h"
#include "MidiUtils.h"
#include <cmath>
#include <iostream>
#include "ofMath.h"
#include <fstream>
#include <sstream>

#ifndef ABS
#define ABS(a) ((a)>=0 ? (a) : (-(a)))
#endif

/// Input: vector of N note values
/// Output: vector of 128 (index, rate) pairs
std::vector<std::pair<int, float>> Multisampler::buildMultisampleLookup(const std::vector<int>& centers) {
	std::vector<std::pair<int, float>> lookup(128);
	for(int i = 0; i < 128; i++) {
		int nearestDistance = -1;
		for(size_t j = 0; j < centers.size(); j++) {
			int note = centers[j];
			int curDistance = std::abs(i - note);
			if(nearestDistance == -1 || curDistance < nearestDistance) {
				nearestDistance = curDistance;
				float target = getFrequency(i);
				float source = getFrequency(note);
				lookup[i].first = j;
				lookup[i].second = target / source;
			}
		}
	}
	return lookup;
}

void Multisampler::addNoteOn(int note, float vel) {
	commandQueueMutex.lock();
	noteCommandQueue.push_back({note,vel});
	commandQueueMutex.unlock();
}


void Multisampler::addNoteOff(int note) {
	commandQueueMutex.lock();
	noteCommandQueue.push_back({note,0});
	commandQueueMutex.unlock();
}


void Multisampler::setup(int rows, int cols, int samplerate) {
	this->nrows = rows;
	this->ncols = cols;
	this->samplerate = samplerate;
	NoteQueueCollection::setup();
}

void Multisampler::load(std::string filename) {
	clear();
	
	auto start = get_time_ms();
	sources.load(filename + ".bin", nrows, ncols);
	auto stop = get_time_ms();
	std::cout << "load time: " << (stop - start) << "ms" << std::endl;
	
	std::vector<int> centers;
	std::ifstream file;
	file.open(filename + ".txt");
	std::string line;
	int pitch;
	if(file.is_open()) {
		while(std::getline(file, line)) {
			std::istringstream ss(line);
			ss >> pitch;
			centers.push_back(pitch);
		}
		file.close();
	} else {
		std::cerr << "Unable to load " << filename << ".txt" << std::endl;
	}
	lookup = buildMultisampleLookup(centers);
}

size_t Multisampler::rows() const {
	return nrows;
}

size_t Multisampler::cols() const {
	return ncols;
}

void Multisampler::on(int note, float volume) {
	std::pair<int, float>& settings = lookup[note];
	unsigned int offset = position * ncols;
	NoteQueueCollection::on(note, settings.first, settings.second, offset, volume);
}

void Multisampler::processNoteQueue() {
	clear_done();

	std::pair<int,float> n;
	size_t notes = get_note_count();

	commandQueueMutex.lock();
	while(noteCommandQueue.size()) {
		n = noteCommandQueue.front();
		noteCommandQueue.pop_front();
		if(n.second==0) {
			off(n.first);
		} else if (notes<hardMaxVoices) {
			on(n.first, n.second);
			++notes;
		}
	}
	commandQueueMutex.unlock();
}

void Multisampler::set_position(float position) {
	if(position < 0) position = 0;
	if(position > 1) position = 1;
	this->position = position;
}

void Multisampler::set_volume(float volume) {
	if(volume < 0) volume = 0;
	if(volume > 1) volume = 1;
	if(volume != next_volume) {
		next_volume = volume;
	}
}

float sneakySqrt(float x) {
	// roughly a sqrt in the range of 0-1
	return 2.55f*x - 2.88f*x*x + 1.33*x*x*x;
}

inline int16_t lerpInt16(int16_t a, int16_t b, float amt) {
	// performs a linear interpolation of two int16_t's
	// this uses integer maths for performance, slightly impacting precision
	int32_t iamt = amt * 0xffff;
	iamt *= (b-a);
	iamt >>= 16;
	iamt += a;
	return iamt;
}

void Multisampler::audio_loop(std::vector<float>& audio, unsigned int samplerate) {
	processNoteQueue();
	auto start = get_time_ns();
	std::fill(audio.begin(), audio.end(), 0);
	size_t samples = audio.size();
	size_t cols = sources.cols();
	float samplerate_ratio = this->samplerate / (float)samplerate;
	float normalization = 1./(float)std::numeric_limits<int16_t>::max();

	float invCrossfateDuration = 1.0f / crossfadeDuration;

	bool looping = this->looping;
	int loopStartPoint = floor((float)loopStart * (float)cols);
	int loopEndPoint = floor((float)loopEnd * (float)cols);

	// error checking - we need the end to be after the start
	if(loopEndPoint<loopStartPoint) {
        std::swap(loopEndPoint, loopStartPoint);
	}

	if(loopEndPoint - loopStartPoint < crossfadeDuration*2) {
		if(loopEndPoint - loopStartPoint < 100) loopEndPoint = loopStartPoint+100;
		crossfadeDuration = (loopEndPoint - loopStartPoint)/2;
	}

	// we want a loop region that's at least 'crossfadeDuration' apart
	if(loopEndPoint-loopStartPoint < crossfadeDuration) {
		// if loop region is not big enough, try to move the end point back if there's space
		if(loopStartPoint+crossfadeDuration < cols) {
			loopEndPoint = loopStartPoint + crossfadeDuration;
			// if not, move the start point forward
		} else if(loopEndPoint - crossfadeDuration > 0) {
			loopStartPoint = loopEndPoint - crossfadeDuration;
		} else {
			// uh ohh.. would only really happen if we were working with really short samples
			// which we're not.
			looping = false;
		}
	}

	// limit loop points
	if(loopStartPoint < 2) loopStartPoint = 2;
	if(loopEndPoint > cols-3) loopEndPoint = cols - 3;


	int crossfadeStartPoint = loopStartPoint + crossfadeDuration;
	int crossfadeEndPoint = loopEndPoint - crossfadeDuration;
	// again check bounds.
	if(crossfadeEndPoint < crossfadeStartPoint+crossfadeDuration*2) {
		if(crossfadeStartPoint > crossfadeDuration*2) {
			crossfadeStartPoint -= crossfadeDuration*2;
		} else {
			crossfadeEndPoint += crossfadeDuration*2;
		}
	}

	int voiceCount = 0;
	int oldestNoteTime = 0;
	Note *oldestNote = nullptr;
	float releaseTime = 0.001 + ref.release*ref.release*ref.release*10;
	float invThisSamplerate = 1.0f / float(this->samplerate);

	// If the envelope amplification changes too suddenly
	// it causes audible artifacts. Limit changes, allowing changes
	// to take up to 50ms.
	float envAmpMaxAdj = invThisSamplerate / 0.050f;

	for(auto& queue : queues) {
		for(auto& note : queue.get()) {
			const auto& source = sources[note.source];
			float speed = note.rate * samplerate_ratio;

			float noteAmp = normalization * note.volume;
			float attack = std::max(0.001, ref.attack*ref.attack*4.);
			float ad = attack + ref.decay;

			for(int i = 0; i < samples; i++) {
				float amp = noteAmp;
				// note is off
				// RELEASE
				if(note.noteoff>0) {
					if(note.killQuickly) {
						amp *= note.envelopeBeforeNoteOff;
						note.envelopeBeforeNoteOff *= 0.97;
						if(note.envelopeBeforeNoteOff<0.01) {
							note.set_done();
							break;
						}
					} else {
						float timeSinceNoteOff = (note.time - note.noteoff) * invThisSamplerate;
						float rel = ofMap(timeSinceNoteOff, 0, releaseTime, 1, 0, true);
						rel = rel * rel;  // exponential decay
						rel *= note.envelopeBeforeNoteOff;
						amp *= rel;

						// release is finished, kill note
						if(rel<=0) {
							note.set_done();
							break;
						}
					}
				} else {
					float timeSinceNoteOn = note.time * invThisSamplerate;
					float lastEnv = note.envelopeBeforeNoteOff;
					float envAmp;

					// ATTACK
					if(timeSinceNoteOn < attack) {
						float att = timeSinceNoteOn/attack;
						envAmp = sneakySqrt(att);
						// DECAY
					} else if(timeSinceNoteOn < ad) {
						float dec = ofMap(timeSinceNoteOn, attack, ad, 1, 0);
						dec = dec * dec; // expo curve
						envAmp = ofMap(dec, 0, 1, sustain, 1); // remap
						// SUSTAIN
					} else {
						envAmp = sustain;
					}

					envAmp = ofClamp(envAmp, lastEnv - envAmpMaxAdj, lastEnv + envAmpMaxAdj);
					note.envelopeBeforeNoteOff = envAmp;
					amp *= envAmp;
				}

				int a = note.playhead;
				int b = a + 1;
				float m = note.playhead - a;

				if(looping) {
					// wrapping code - only send the playhead backwards if the note is down.
					if(note.dir==1 && note.noteoff==0 && a >= loopEndPoint) {
						note.dir = -1;
					} else if(note.dir==-1 && a <= loopStartPoint) {
						//note.dir = 1;
						//printf("boing\n");
					}


					if(note.dir==1 && note.playhead>=crossfadeEndPoint) {

						// drop in crossfader here
						note.crossfadePlayhead = note.playhead;
						note.crossfadeDir = note.dir;
						note.crossfadeTime = 0;

						note.dir = -1;
						note.playhead = loopEndPoint-1;

					} else if(note.dir==-1 && note.playhead<=crossfadeStartPoint) {

						// drop in cross fader here.
						note.crossfadePlayhead = note.playhead;
						note.crossfadeDir = note.dir;
						note.crossfadeTime = 0;

						note.dir = 1;
						note.playhead = loopStartPoint;
					}

					// now, if we're crossfading...
					if(note.crossfadeTime!=-1) {

						int ca = note.crossfadePlayhead;
						int cb = ca + 1;
						float cm = note.crossfadePlayhead - ca;

						float crossfadePos = note.crossfadeTime / (float) crossfadeDuration;
						// actual playhead fades up
						audio[i] += lerpInt16(source[a], source[b], m) * amp * sneakySqrt(crossfadePos);

						// crossfade playhead fades down
						audio[i] += lerpInt16(source[ca], source[cb], cm) * amp * sneakySqrt(1.f - crossfadePos);

						note.crossfadeTime++;

						// increment crossfade playhead
						note.crossfadePlayhead += speed * note.crossfadeDir;
						// check to see if crossfade is done
						if(note.crossfadeTime>crossfadeDuration) {
							note.crossfadeTime = -1;
						}

					} else {
						// or we're not crossfading
						audio[i] += lerpInt16(source[a], source[b], m) * amp;
					}

					// increment playhead
					note.playhead += speed*note.dir;
				} else {
					// NON-LOOPING MODE
					// taper at the end if not in looping mode
					if(b >= cols - 100) {
						amp *= ofMap(b, cols - 100, cols - 1, 1, 0, true);
					}
					// if hit the end then bail
					if(b >= cols) {
						note.set_done();
						break;
					}
					// normal interpolation
					audio[i] += (source[a] * (1.f - m) + source[b]* m) * amp;
					note.playhead += speed;
				}

				note.time++;
			}

			if(note.noteoff<=0) {
				voiceCount++;
				if(note.time>oldestNoteTime) {
					oldestNote = &note;
					oldestNoteTime = note.time;
				}
			}
		}
	}

	if(voiceCount>softMaxVoices) {
		// this is a soft voice limit
		// if there are too many voices kill the oldest one
		if(oldestNote!=nullptr) {
			oldestNote->kill();
		}
	}

	float volume_start = current_volume;
	float volume_stop = next_volume;
	float volume_step = (volume_stop - volume_start) / samples;
	float volume = volume_start;
	for(auto& sample : audio) {
		
		float absSample = ofClamp(ABS(sample), 0.0, 0.05)/0.05;
		
		if(absSample > signalAmplitude) {
			signalAmplitude = absSample * 0.0005 + signalAmplitude * 0.9995;
		} else {
			signalAmplitude = absSample * 0.0003 + signalAmplitude * 0.9997;
		}
		sample *= volume;
		volume += volume_step;
		
	}
	current_volume = volume;
	
	
	auto stop = get_time_ns();
	if(previous_time > 0) {
		auto cur_time_used = stop - start;
		average_time_used = average_time_used*.99 + cur_time_used*.01;
		auto cur_time_available = start - previous_time;
		average_time_available = average_time_available*.99 + cur_time_available*.01;
	}
	previous_time = start;
	lastReleaseTime = releaseTime;
}


float Multisampler::get_performance() const {
	return average_time_used / average_time_available;
}

