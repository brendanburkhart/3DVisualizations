#include "stdafx.h"
#include "BackBuffer.h"

BackBuffer::BackBuffer () {
    buffer = nullptr;
    memBuffer = nullptr;

    pixelWidth = 0;
    pixelHeight = 0;
}

BackBuffer::BackBuffer (int width, int height) {
    buffer = new char [width * height * 4];
    memBuffer = new int [width * height];

    pixelWidth = width;
    pixelHeight = height;
}

BackBuffer::~BackBuffer () {
    delete [] buffer;
    delete [] memBuffer;
}

char &BackBuffer::operator [] (int i) {
    return *(buffer + i);
}

// Converts byte array (represented as char*) to 32-bit int array, 4 bytes packed into each int
int* BackBuffer::getBuffer () {
    for (auto i = 0; i < pixelHeight * pixelWidth; i++) {
        memBuffer [i] = (UINT32)(buffer [0] << 24) + (buffer [1] << 16) + (buffer [2] << 8) + buffer [3];
    }

    return memBuffer;
}

int BackBuffer::getBufferWidth () {
    return pixelWidth;
}

int BackBuffer::getBufferHeight () {
    return pixelHeight;
}