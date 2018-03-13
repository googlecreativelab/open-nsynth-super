/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

    
#include "Multisampler.h"
#include "MidiUtils.h"
#include "ofMain.h"
#include <cmath>
#include <iostream>
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



void Multisampler::audio_loop(std::vector<float>& audio, unsigned int samplerate) {
    auto start = get_time_ns();
    std::fill(audio.begin(), audio.end(), 0);
    size_t samples = audio.size();
    size_t cols = sources.cols();
    float samplerate_ratio = float(this->samplerate) / samplerate;
    float normalization = 1./std::numeric_limits<int16_t>::max();
    for(auto& queue : queues) {
        for(auto& note : queue.get()) {
            const auto& source = sources[note.source];
            float start_time = note.time;
            float stop_time = start_time + samples;
            
            bool done = false;
            unsigned int length = cols - note.offset;
            float amp_start = normalization * get_envelope(start_time, note.noteoff, length, &done) * note.volume;
            float amp_stop = normalization * get_envelope(stop_time, note.noteoff, length, &done) * note.volume;
            float amp_step = (amp_stop - amp_start) / samples;
            float amp = amp_start;
            
            float i_start = (note.offset + start_time) * note.rate * samplerate_ratio;
            float i_stop = (note.offset + stop_time) * note.rate * samplerate_ratio;
            float i_step = (i_stop - i_start) / samples;
            float i = i_start;
            
            for(auto& sample : audio) {
                if(i+1 >= cols) {
                    break;
                }
                
                // linear interpolation for resampling
                unsigned int i0 = i;
                float t = i - i0;
                float interpolated = source[i0] * (1-t) + source[i0+1] * t;
                sample += interpolated * amp;

                i += i_step;
                amp += amp_step;
            }
            note.time += samples;
            
            if(done) {
                note.set_done();
            }
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
    
    clear_done();
    auto stop = get_time_ns();
    if(previous_time > 0) {
        auto cur_time_used = stop - start;
        average_time_used = average_time_used*.99 + cur_time_used*.01;
        auto cur_time_available = start - previous_time;
        average_time_available = average_time_available*.99 + cur_time_available*.01;
    }
    previous_time = start;
}

float Multisampler::get_performance() const {
    return average_time_used / average_time_available;
}
