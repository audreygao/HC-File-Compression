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
#include "cxxopts.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(string inFileName, string outFileName) {
    std::cout << "1" << endl;
    HCTree* tree = new HCTree();

    ifstream theFile;
    unsigned char nextChar;
    theFile.open("inFileName");
    vector<unsigned int> vector(256, 0);
    while (1) {
        nextChar = theFile.get();
        if (theFile.eof()) {
            break;
        }
        std::cout << nextChar << endl;
        vector[nextChar]++;
    }
    std::cout << "2" << endl;

    theFile.close();
    tree->build(vector);
    ofstream outFile;
    outFile.open(outFileName);
    for (int i : vector) {
        outFile << i;
        outFile << '\n';
    }
    std::cout << "3" << endl;

    theFile.open("inFileName");
    while (1) {
        nextChar = theFile.get();
        if (theFile.eof()) break;
        tree->encode(nextChar, outFile);
    }
    std::cout << "4" << endl;

    outFile.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {}

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
        std::cout << "call pseudo" << endl;
        pseudoCompression(inFileName, outFileName);
    }
}