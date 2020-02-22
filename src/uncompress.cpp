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

    std::cout << "a" << endl;
    HCTree* tree = new HCTree();
    ifstream inFile;
    inFile.open(inFileName);

    std::cout << "b" << endl;

    // decode header
    char charBuf[768];
    std::cout << "b1" << endl;
    inFile.getline(charBuf, 768);
    std::cout << "b2" << endl;
    BitInputStream headerIn(inFile, 768, charBuf);

    std::cout << "c" << endl;

    vector<char> sym;
    vector<unsigned int> vec;
    unsigned int bit;
    unsigned int character = 0;

    std::cout << "d" << endl;
    while (!headerIn.eofHeader()) {
        bit = headerIn.readBitHeader();
        vec.push_back(bit);

        std::cout << "e" << endl;

        if (bit == 0) {
            for (int i = 0; i < 8; i++) {
                bit = headerIn.readBitHeader();
                character += bit * pow(2, 7 - i);

                std::cout << "f" << endl;
            }
            sym.push_back((char)character);
        }
    }

    std::cout << "before rebuild" << endl;

    // read structure code
    tree->rebuildAll(sym, vec);

    std::cout << "g" << endl;

    ofstream outFile;
    outFile.open(outFileName);
    BitInputStream bodyIn(inFile, 4000);

    while (1) {
        byte symbol = tree->decode(bodyIn);
        std::cout << "after decode" << endl;
        outFile.put(symbol);
        if (bodyIn.eof()) break;
        // outFile.put(symbol);
        std::cout << "h" << endl;
    }
    outFile.close();
    inFile.close();
    delete tree;
    std::cout << "i" << endl;
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
