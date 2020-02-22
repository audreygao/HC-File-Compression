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

/**
 * Compress data in inFileName using Huffman Encoding
 * and write codes into outFileName
 * param:
 *  - inFileName: file to be compressed
 *  - outFileName: the compressed file
 */
void pseudoDecompression(string inFileName, string outFileName) {
    // if input is emptyfile, outfile will be empty
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName);
        outFile.close();
        return;
    }

    HCTree* tree = new HCTree();
    vector<unsigned int> vec(256, 0);

    // string for the counts
    string str;

    ifstream inFile;
    inFile.open(inFileName);

    // loop through the vector and add the count
    for (int i = 0; i < vec.size(); i++) {
        // get the line without nextline char
        getline(inFile, str);
        str.substr(0, str.size() - 1);

        // convert str to number and add to vector
        vec[i] = stoi(str);
    }

    // build the tree using the frequency vector
    tree->build(vec);

    // open the outFile
    ofstream outFile;
    outFile.open(outFileName);

    // decode all characters in the inFile
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
        return;
    }

    HCTree* tree = new HCTree();
    ifstream inFile;
    inFile.open(inFileName);

    // decode header
    string count;
    getline(inFile, count);
    count.substr(0, count.size() - 1);
    int theCount;
    theCount = stoi(count);

    char* charBuf = new char[800];
    unsigned int zero = 0;
    std::fill(&charBuf[0], &charBuf[800], zero);
    inFile.getline(charBuf, 800);
    BitInputStream* headerIn = new BitInputStream(inFile, 800, charBuf);
    // BitInputStream headerIn(inFile, 800, charBuf);

    vector<char> sym;
    vector<unsigned int> vec;
    unsigned int bit = 0;
    unsigned int character = 0;

    while (!headerIn->eofHeader()) {
        bit = headerIn->readBitHeader();
        vec.push_back(bit);

        if (bit == 0) {
            character = 0;
            for (int i = 0; i < 8; i++) {
                bit = headerIn->readBitHeader();
                character += bit * pow(2, 7 - i);
            }
            sym.push_back((char)character);
        }
    }

    inFile.close();
    inFile.open(inFileName);
    string str = "";
    getline(inFile, str);
    getline(inFile, str);

    // read structure code
    tree->rebuildAll(sym, vec);

    ofstream outFile;
    outFile.open(outFileName);
    BitInputStream* bodyIn = new BitInputStream(inFile, 10);

    int countInd = 0;

    while (countInd < theCount) {
        byte symbol = tree->decode(*bodyIn);
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
    delete headerIn;
    delete bodyIn;
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
