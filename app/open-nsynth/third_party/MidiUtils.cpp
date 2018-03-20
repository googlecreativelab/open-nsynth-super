/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MidiUtils.h"
#include <cmath>

float getFrequency(int midiNote) {
    const static float frequencyA4 = 440.;
    const static int midiNoteA4 = 69;
    return frequencyA4 * pow(2, (midiNote - midiNoteA4) / 12.);
}

int clampMidiNote(int note) {
    if(note < 0) {
        return 0;
    } else if(note > 127) {
        return 127;
    }
    return note;
}

int clampMidiOctave(int octave) {
    if(octave < -1) {
        return -1;
    }
    if(octave > 9) {
        return 9;
    }
    return octave;
}

int keyAndOctaveToMidiNote(int key, int octave) {
    octave = clampMidiOctave(octave);
    int note = 12 * (octave + 1) + key;
    return clampMidiNote(note);
}

uint64_t get_time_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

uint64_t get_time_ns() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}
