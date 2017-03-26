#include "stdafx.h"
#include "BackBuffer.h"

BackBuffer::BackBuffer () {
    buffer = nullptr;
    
    width = 0;
    height = 0;
    scanLineSize = 0;
}

BackBuffer::BackBuffer (int width, int height) {
    buffer = new char [width * height * 4];
    
    this->width = width;
    this->height = height;
    this->scanLineSize = width * 4;
}

void BackBuffer::Release () {
    delete [] buffer;
}

char &BackBuffer::operator [] (int i) {
    return *(buffer + i);
}