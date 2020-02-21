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
        std::cout << "will add flush size" << endl;
        flushSize++;
    }

    out.write(buf, flushSize);

    nbits = 0;
    isize++;
    fill(&buf[0], &buf[bufSize], 0);  // zero fill
}

/* TODO */
void BitOutputStream::writeBit(unsigned int i) {
    if (bufSize * 8 == nbits) {
        std::cout << "will flush" << endl;
        flush();
    }
    int byteIndex = nbits / 8;
    int bitIndex = nbits % 8;
    i = i << (7 - bitIndex);
    if (i == 128) {
        std::cout << "i is 128" << endl;
    }
    buf[byteIndex] = buf[byteIndex] | i;
    if (buf[byteIndex] == 128) {
        std::cout << "i is 128" << endl;
    }
    nbits++;
}
