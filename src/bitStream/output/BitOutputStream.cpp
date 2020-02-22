/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include "BitOutputStream.hpp"
#include <algorithm>

// using namespace std;

/* TODO */
void BitOutputStream::flush() {
    int flushSize = nbits / 8;
    if (nbits % 8 != 0) {
        flushSize++;
    }
    out.write(buf, flushSize);
    stopBit = nbits % 8;
    nbits = 0;

    fill(&buf[0], &buf[bufSize], 0);  // zero fill
}

/* TODO */
void BitOutputStream::writeBit(unsigned int i) {
    if (bufSize * 8 == nbits) {
        flush();
    }
    int byteIndex = nbits / 8;
    int bitIndex = nbits % 8;
    i = i << (7 - bitIndex);

    buf[byteIndex] = buf[byteIndex] | i;
    nbits++;
}

BitOutputStream::~BitOutputStream() { delete[] buf; }