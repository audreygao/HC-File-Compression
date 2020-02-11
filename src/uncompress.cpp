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

/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {
    HCTree* tree = new HCTree();
    vector<unsigned int> vec(256, 0);
    ifstream theFile;
    string str;
    theFile.open("inFileName");

    // loop through the vector and add the count
    for (int i = 0; i < vec.size(); i++) {
        // get the line without nextline char
        getline(theFile, str);
        str.substr(0, str.size() - 1);

        // convert str to number and add to vector
        vec[i] = stoi(str);
    }

    tree->build(vec);

    ofstream outFile;
    outFile.open(outFileName);
    while (1) {
        byte symbol = tree->decode(theFile);
        if (theFile.eof()) break;
        outFile.put(symbol);
    }
    outFile.close();
    theFile.close();
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {}

/* TODO: Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 3;
    if (argc != NUM_ARG) {
        return -1;
    }
    if (argv[0] == "--ascii") {
        pseudoDecompression(argv[1], argv[2]);
    }
}
