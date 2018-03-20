/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <vector>
#include <utility>

#include "ADSR.h"
#include "NoteScheduler.h"
#include "BinaryDataSync.h"

#define USE_ASYNC

#ifdef USE_ASYNC
#include "BinaryDataAsync.h"
#endif

class Multisampler : public NoteQueueCollection, public ADSR {
private:
    size_t nrows, ncols;
    unsigned int samplerate;
    float position = 0;
    float current_volume = 1, next_volume = 1;
    uint64_t previous_time = 0;
    float average_time_available = 0;
    float average_time_used = 0;
    
protected:
#ifdef USE_ASYNC
    BinaryDataAsync<int16_t> sources;
#else
    BinaryDataSync<int16_t> sources;
#endif
    std::vector<std::pair<int, float>> lookup;
    
    static std::vector<std::pair<int, float>> buildMultisampleLookup(const std::vector<int>& centers);
    
public:
    void setup(int rows, int cols, int samplerate);
    void load(std::string filename);
    size_t rows() const;
    size_t cols() const;
    void on(int note, float volume=1);
    void set_position(float position);
    void set_volume(float volume);
    void audio_loop(std::vector<float>& audio, unsigned int samplerate);
    float get_performance() const;
    float signalAmplitude = 0;
};
