/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** TODO: class header */
class BitInputStream {
  private:
    istream& in;           // reference to the input stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits that have been read from buf
    bool eofBit;           // true when trying to read past the end of the file

    /* TODO: add function header */
    bool atEndOfFile();

  public:
    /**
     * Initializes member variables.
     * the char array should have size bufSize.
     */
    explicit BitInputStream(istream& is, unsigned int bufSize) : in(is) {
        buf = new char[bufSize];
        unsigned int zero = 0;
        std::fill(&buf[0], &buf[bufSize], zero);
        this->bufSize = bufSize;
        fill();
        nbits = 0;
        eofBit = false;
    };

    /**
     * Fill the buffer of size bufSize from input stream
     */
    void fill();

    /**
     * true when we are trying to read past the end of the file
     */
    bool eof();

    /* TODO: add function header */
    unsigned int readBit();
};

#endif