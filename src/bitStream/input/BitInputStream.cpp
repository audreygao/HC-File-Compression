/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() {}

/* TODO */
bool BitInputStream::atEndOfFile() { return false; }

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/* TODO */
unsigned int BitInputStream::readBit() { return 0; }