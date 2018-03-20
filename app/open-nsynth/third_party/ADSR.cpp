/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "ADSR.h"

float remap(const float& value, const float& istart, const float& istop, const float& ostart, const float& ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

const float& clamp(const float& v, const float& lo, const float& hi) {
    if(v < lo) return lo;
    if(v > hi) return hi;
    return v;
}

void ADSR::limit() {
    float sum = ref.attack + ref.decay + ref.release;
    if(sum > 1) {
        lim.attack = ref.attack / sum;
        lim.decay = ref.decay / sum;
        lim.release = ref.release / sum;
    } else {
        lim.attack = ref.attack;
        lim.decay = ref.decay;
        lim.release = ref.release;
    }
}

// not normalized to length
float ADSR::get_envelope(float time, float noteoff, float length, bool* done) {
    return get_envelope(time / length, noteoff / length, done);
}

// normalized to length
float ADSR::get_envelope(float time, float noteoff, bool* done) {
    *done = false; // this could be removed if necessary
    if(noteoff > 0) { // release (post-noteoff)
        float end = noteoff + lim.release;
        if(end > 1) end = 1;
        if(time >= end) { // done
            *done = true;
            return 0;
        }
        // making a second call isn't efficient, but it is correct
        float peak = get_envelope(noteoff, 0, done);
        return remap(time, noteoff, end, peak, 0); // releasing
    }
    if(time <= lim.attack) { // attack
        return time / lim.attack;
    }
    if((time - lim.attack) <= lim.decay) { // decay
        return remap(time - lim.attack, 0, lim.decay, 1, sustain);
    }
    if((1 - time) <= lim.release) { // release
        if(time >= 1) {
            *done = true;
            return 0;
        }
        return remap(time, 1 - lim.release, 1, sustain, 0);
    }
    return sustain; // sustain, default
}

void ADSR::set_attack(float attack) {
    ref.attack = clamp(attack, 0, 1);
    limit();
}

void ADSR::set_decay(float decay) {
    ref.decay = clamp(decay, 0, 1);
    limit();
}

void ADSR::set_sustain(float sustain) {
    this->sustain = clamp(sustain, 0, 1);
}

void ADSR::set_release(float release) {
    ref.release = clamp(release, 0, 1);
    limit();
}
