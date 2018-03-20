/* 
    Copyright (c) 2017- Kyle McDonald
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>

template <class T>
class BinaryDataSync {
private:
    std::vector<T> data;
    size_t nrows = 0;
    size_t ncols = 0;
public:
    void load(std::string filename, size_t rows, size_t cols) {
        nrows = rows;
        ncols = cols;
        size_t n = rows * cols;
        data.resize(n);
        FILE* file = fopen(filename.c_str() , "rb");
        if(file == NULL) {
            std::cerr << "Couldn't open file " << filename << std::endl;
            return;
        }
        size_t result = fread(&data[0], sizeof(T), n, file);
        if(result == 0) {
            std::cerr << "Couldn't read file " << filename << std::endl;
            nrows = 0;
            ncols = 0;
            data.resize(0);
        }
        fclose(file);
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
