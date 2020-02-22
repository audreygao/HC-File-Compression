/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** TODO: class header */
class BitOutputStream {
  private:
    ostream& out;          // reference to the output stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits have been writen to buf

  public:
    unsigned int stopBit;  // index of bit past last written bit
    /**
     * Initializes member variables of BitOutputStream
     * param: - os: output stream to use
     *  - bufsize: char array buf's size
     */
    explicit BitOutputStream(ostream& os, unsigned int bufSize) : out(os) {
        buf = new char[bufSize];
        unsigned int zero = 0;
        std::fill(&buf[0], &buf[bufSize], zero);
        this->bufSize = bufSize;
        nbits = 0;
    };

    ~BitOutputStream();

    /**
     * Write the part of buffer that was written by the user to the output
     * stream and clear the buffer
     */
    void flush();

    /* TODO: add function header */
    void writeBit(unsigned int i);
};

#endif
