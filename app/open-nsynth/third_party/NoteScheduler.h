/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <deque>
#include <vector>
#include <algorithm>

class Note {
private:
    bool done = false;
    
public:
    unsigned int source;
    float rate;
    unsigned int offset;
    unsigned int time = 0;
    unsigned int noteoff = 0;
    float volume = 1;
    
    Note(int source=0, float rate=1, unsigned int offset=0, float volume=1)
    :source(source)
    ,rate(rate)
    ,offset(offset)
    ,volume(volume) {
    }
    
    /// Called when the note has been released.
    void off() {
        noteoff = time;
    }
    /// Called when the note is no longer playing.
    void set_done() {
        done = true;
    }
    /// True when the note is no longer needed.
    bool is_done() const {
        return done;
    }
};

template <class Container, class UnaryPredicate>
void erase_and_remove_if(Container& container, UnaryPredicate predicate) {
    container.erase(std::remove_if(container.begin(),
                                   container.end(),
                                   predicate),
                    container.end());
}

/// Don't call on() and update() from different threads.
class NoteQueue {
private:
    std::deque<Note> queue;
    
public:
    void on(const Note&& note) {
        queue.push_front(note);
    }
    void off() {
        if(queue.empty()) {
            // this happens when a note has been removed
            // after playing out, but the key was never lifted
            // until now
            return;
        }
        queue.front().off();
    }
    void clear_done() {
        erase_and_remove_if(queue, [](Note& note){
            return note.is_done();
        });
    }
    void clear() {
        queue.clear();
    }
    std::deque<Note>& get() {
        return queue;
    }
};

class NoteQueueCollection {
private:
    int minNote, maxNote;
    
protected:
    std::vector<NoteQueue> queues;
    
public:
    void setup(int minNote=0, int maxNote=128) {
        this->minNote = minNote;
        this->maxNote = maxNote;
        queues.resize(maxNote - minNote);
    }
    void clear_done() {
        for(auto& queue : queues) {
            queue.clear_done();
        }
    }
    void on(int note, int source, float rate=1, unsigned int offset=0, float volume=1) {
        if(note < minNote || note >= maxNote) {
            return;
        }
        int i = note - minNote;
        queues[i].on(Note(source, rate, offset, volume));
    }
    void off(int note) {
        if(note < minNote || note >= maxNote) {
            return;
        }
        int i = note - minNote;
        queues[i].off();
    }
    void clear() {
        for(auto& queue : queues) {
            queue.clear();
        }
    }
    std::vector<NoteQueue>& get() {
        return queues;
    }

};
