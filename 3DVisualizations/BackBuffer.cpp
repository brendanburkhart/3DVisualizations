#include "stdafx.h"
#include "BackBuffer.h"

#include <array>

BackBuffer::BackBuffer() {
    width = 0;
    height = 0;
    scanLineSize = 0;
}

BackBuffer::BackBuffer(int width, int height) {
    this->width = width;
    this->height = height;
    this->scanLineSize = width * 4;

    size_t size = static_cast<size_t>(height * scanLineSize);
    data.resize(size);
}

char& BackBuffer::operator [] (int index) {
    return data[index];
}
