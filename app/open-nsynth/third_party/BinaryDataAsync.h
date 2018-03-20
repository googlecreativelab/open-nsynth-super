/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

template <class T>
class BinaryDataAsync {
private:
    T* data = NULL;
    size_t nbytes = 0;
    size_t nrows = 0;
    size_t ncols = 0;
public:
    BinaryDataAsync() {}
    ~BinaryDataAsync() {
        unload();
    }
    int load(std::string filename, size_t rows, size_t cols, bool sequential=false) {
        if(data != NULL) {
            unload();
        }
        this->nrows = rows;
        this->ncols = cols;
        size_t n = rows * cols;
        nbytes = n * sizeof(T);
        int fd = open(filename.c_str(), O_RDONLY);
        if(fd < 0) {
            std::cerr << "Couldn't open file " << filename << std::endl;
            return -1;
        }
        data = reinterpret_cast<T*>(mmap(NULL, nbytes, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0));
        if(data == MAP_FAILED) {
            std::cerr << "Couldn't map " << filename << std::endl;
            return -1;
        }
        if(madvise(data, nbytes, (sequential?MADV_SEQUENTIAL:MADV_RANDOM) | MADV_WILLNEED) != 0) {
            std::cerr << "Couldn't set hints." << std::endl;
            return -1;
        }
        close(fd);
        return 0;
    }
    int unload() {
        if(data == NULL) {
            return 0;
        }
        if(munmap(data, nbytes) != 0) {
            std::cerr << "Couldn't unmap." << std::endl;
            return -1;
        }
        data = NULL;
        nrows = 0;
        ncols = 0;
        return 0;
    }
    T* operator[](size_t row) {
        return get_row(row);
    }
    T* get_row(size_t row) {
        return &data[row * ncols];
    }
    T& get_element(size_t row, size_t col) {
        return data[row * ncols + col];
    }
    size_t rows() const {
        return nrows;
    }
    size_t cols() const {
        return ncols;
    }
};
