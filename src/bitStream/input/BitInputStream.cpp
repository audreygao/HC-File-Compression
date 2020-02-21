/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include "BitInputStream.hpp"
#include <algorithm>

/* TODO */
void BitInputStream::fill() {
    // read <= bufsize number of bytes from istream into buf
    in.read(buf, bufSize);
    nbits = 0;
    // std::fill(buf[0], buf[bufSize], 0);
    // for (int i = 0; i < bufSize; i++) {
    //     buf[i] = in.get();
    // }
}

/**
 * Return true when istream tried to read more bytes than the remaining byte in
 * the file AND the buffer was then also fully read.
 * Otherwise, return false
 */
bool BitInputStream::atEndOfFile() {
    int numRead = in.gcount();
    // istream tried to read more than remaining bytes
    if (!in) {
        // buf was then fully read
        if (numRead == 0 || numRead * 8 == nbits) {
            return true;
        }
    }
    return false;
}

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/* TODO */
unsigned int BitInputStream::readBit() {
    // fill the buf if all bits in buf has been read
    if (bufSize * 8 == nbits) {
        fill();
    }

    // set eofbit and return 0;
    if (atEndOfFile()) {
        eofBit = true;
        return 0;
    }

    // find the index and read the bit
    int byteIndex = nbits / 8;
    int bitIndex = nbits % 8;
    nbits++;
    return (buf[byteIndex] >> 7 - bitIndex) & 1;
}