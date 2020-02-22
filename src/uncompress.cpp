/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {
    // if input is emptyfile, outfile will be empty
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName);
        outFile.close();
    }

    HCTree* tree = new HCTree();
    vector<unsigned int> vec(256, 0);
    ifstream inFile;
    string str;
    inFile.open(inFileName);

    // loop through the vector and add the count
    for (int i = 0; i < vec.size(); i++) {
        // get the line without nextline char
        getline(inFile, str);
        str.substr(0, str.size() - 1);

        // convert str to number and add to vector
        vec[i] = stoi(str);
    }

    tree->build(vec);
    ofstream outFile;

    outFile.open(outFileName);
    while (1) {
        byte symbol = tree->decode(inFile);
        if (inFile.eof()) break;
        outFile.put(symbol);
    }
    outFile.close();
    inFile.close();
    delete tree;
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {
    // if input is emptyfile, outfile will be empty
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName);
        outFile.close();
    }

    // std::cout << "a" << endl;
    HCTree* tree = new HCTree();
    ifstream inFile;
    inFile.open(inFileName);

    // std::cout << "b" << endl;

    // decode header
    string count;
    getline(inFile, count);
    count.substr(0, count.size() - 1);
    int theCount;
    theCount = stoi(count);

    char charBuf[800];
    // std::cout << "b1" << endl;
    inFile.getline(charBuf, 800);
    // std::cout << "b2" << endl;
    BitInputStream headerIn(inFile, 800, charBuf);

    // std::cout << "c" << endl;

    vector<char> sym;
    vector<unsigned int> vec;
    unsigned int bit;
    unsigned int character = 0;

    // // decode the stopBit from header
    // unsigned int stopBit = 0;
    // for (int i = 2; i >= 0; i--) {
    //     bit = headerIn.readBitHeader();
    //     std::cout << bit << endl;
    //     std::cout << "each bit of stop bit ^" << endl;
    //     stopBit += bit * pow(2, i);
    // }

    // std::cout << stopBit << endl;

    // std::cout << "d" << endl;
    while (!headerIn.eofHeader()) {
        bit = headerIn.readBitHeader();
        vec.push_back(bit);
        // std::cout << "structure bit:" << endl;
        // std::cout << bit << endl;
        // std::cout << "e" << endl;

        if (bit == 0) {
            character = 0;
            for (int i = 0; i < 8; i++) {
                bit = headerIn.readBitHeader();
                character += bit * pow(2, 7 - i);

                // std::cout << "f" << endl;
            }
            sym.push_back((char)character);
        }
    }

    inFile.close();
    inFile.open(inFileName);
    string str;
    getline(inFile, str);
    getline(inFile, str);

    // std::cout << "before rebuild" << endl;

    for (unsigned int i : vec) {
        std::cout << i << endl;
    }
    // for (char i : sym) {
    //     std::cout << i << endl;
    // }

    // read structure code
    tree->rebuildAll(sym, vec);

    // std::cout << "g" << endl;

    ofstream outFile;
    outFile.open(outFileName);
    BitInputStream bodyIn(inFile, 4000);

    std::cout << "the count of characrters is " << endl;
    std::cout << theCount << endl;
    int countInd = 0;

    while (countInd < theCount) {
        byte symbol = tree->decode(bodyIn);
        // if (bodyIn.eof()) break;
        // if (bodyIn.lastBuf &&
        //     stopBit == (bodyIn.curBit() - 8 * (bodyIn.lastCount - 1))) {
        //     std::cout << "last buffer" << endl;
        //     break;
        // }
        // std::cout << symbol << endl;
        outFile.put(symbol);
        countInd++;
    }
    outFile.close();
    inFile.close();
    delete tree;
}

/* TODO: Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./uncompress",
                             "Uncompresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    if (isAsciiOutput) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }
}
