/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(string inFileName, string outFileName) {
    HCTree* tree = new HCTree();
    ifstream theFile;
    unsigned char nextChar;
    theFile.open("inFileName");
    vector<unsigned int> vector(256, 0);
    while (1) {
        nextChar = theFile.get();
        if (theFile.eof()) break;
        vector[nextChar]++;
    }

    theFile.close();
    tree->build(vector);
    ofstream outFile;
    outFile.open(outFileName);
    for (int i : vector) {
        outFile << i;
        outFile << '\n';
    }
    theFile.open("inFileName");
    while (1) {
        nextChar = theFile.get();
        if (theFile.eof()) break;
        tree->encode(nextChar, outFile);
    }
    outFile.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {}

/* TODO: Main program that runs the compress */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 3;
    if (argc != NUM_ARG) {
        return -1;
    }
    if (argv[0] == "--ascii") {
        pseudoCompression(argv[1], argv[2]);
    }
}