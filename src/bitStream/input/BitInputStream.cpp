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
    unsigned int zero = 0;
    std::fill(&buf[0], &buf[bufSize], zero);
    in.read(buf, bufSize);
    int numRead = in.gcount();
    // std::cout << numRead << endl;
    // std::cout << "^ if gcount " << endl;

    nbits = 0;
}

/**
 * Return true when istream tried to read more bytes than the remaining byte in
 * the file AND the buffer was then also fully read.
 * Otherwise, return false
 */
bool BitInputStream::atEndOfFile() {
    int numRead = in.gcount();
    lastCount = numRead;
    // std::cout << numRead << endl;
    // istream tried to read more than remaining bytes
    if (!in) {
        lastBuf = true;
        // buf was then fully read
        if (numRead == 0 || numRead * 8 == nbits) {
            // std::cout << "atendofFile true" << endl;
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
        // std::cout << "end of file" << endl;
        eofBit = true;
        return 0;
    }

    // find the index and read the bit
    int byteIndex = nbits / 8;
    int bitIndex = nbits % 8;
    nbits++;
    return (buf[byteIndex] >> 7 - bitIndex) & 1;
}

unsigned int BitInputStream::readBitHeader() {
    // fill the buf if all bits in buf has been read
    if (bufSize * 8 == nbits) {
        eofHeaderBit = true;
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
bool BitInputStream::eofHeader() { return eofHeaderBit; }
unsigned int BitInputStream::curBit() { return nbits; }