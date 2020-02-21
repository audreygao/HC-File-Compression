/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include <fstream>
#include <iostream>

#include "BitOutputStream.hpp"
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(string inFileName, string outFileName) {
    // if input is emptyfile, outfile will be empty
    if (FileUtils::isEmptyFile(inFileName)) {
        std::cout << "empty file" << endl;
        ofstream outFile;
        outFile.open(outFileName);
        outFile.close();
        return;
    }

    HCTree* tree = new HCTree();

    ifstream theFile;
    unsigned char nextChar;
    int nextByte;
    theFile.open(inFileName, ios::binary);
    vector<unsigned int> vec(256);
    while ((nextByte = theFile.get()) != EOF) {
        nextChar = (unsigned int)nextByte;
        vec.at(nextChar) += 1;
    }

    theFile.close();

    tree->build(vec);

    ofstream outFile;
    outFile.open(outFileName, ios::trunc);

    for (int i : vec) {
        outFile << i;
        outFile << '\n';
    }

    theFile.open(inFileName);
    while (1) {
        nextChar = theFile.get();
        if (theFile.eof()) break;
        tree->encode(nextChar, outFile);
    }
    theFile.close();

    outFile.close();
    delete tree;
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {
    // if input is emptyfile, outfile will be empty
    if (FileUtils::isEmptyFile(inFileName)) {
        std::cout << "empty file" << endl;
        ofstream outFile;
        outFile.open(outFileName);
        outFile.close();
        return;
    }

    HCTree* tree = new HCTree();

    ifstream theFile;
    unsigned char nextChar;
    int nextByte;
    theFile.open(inFileName, ios::binary);
    vector<unsigned int> vec(256);
    while ((nextByte = theFile.get()) != EOF) {
        nextChar = (unsigned int)nextByte;
        vec.at(nextChar) += 1;
    }

    theFile.close();

    tree->build(vec);

    ofstream outFile;
    outFile.open(outFileName, ios::trunc);

    BitOutputStream bitos(outFile, 4000);

    // header
    // write symbols from left to right to outfile
    // traverse the tree and write bit to buffer
    vector<unsigned int> vect;
    vector<char> sym;
    tree->traverseAll(sym, vect);

    // write the number of symbols / 1 byte
    outFile.put(sym.size() - 1);

    for (char j : sym) {
        outFile.put(j);
    }

    for (unsigned int i : vect) {
        bitos.writeBit(i);
    }
    outFile.put('\n');

    // body
    theFile.open(inFileName);
    while (1) {
        nextChar = theFile.get();
        if (theFile.eof()) break;
        tree->encode(nextChar, bitos);
    }
    theFile.close();

    outFile.close();
    delete tree;
}

/* TODO: Main program that runs the compress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
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

    // ours

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    }
}