/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() {
    std::fill(buf[0], buf[bufSize], 0);
    for (int i = 0; i < bufSize; i++) {
        buf[i] = in.get();
    }
}

/* TODO */
bool BitInputStream::atEndOfFile() { return false; }

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/* TODO */
unsigned int BitInputStream::readBit() {
    // set eofbit and return 0;
    if (atEndOfFile()) {
        return 0;
    }

    if (bufSize * 8 == nbits) {
        fill();
    }

    int byteIndex = nbits / 8;
    int bitIndex = nbits % 8;
    return (buf[byteIndex] >> 7 - bitIndex) & 1;
}