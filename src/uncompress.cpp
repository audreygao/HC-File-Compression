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
    HCTree* tree = new HCTree();
    ifstream inFile;
    inFile.open(inFileName);

    BitInputStream bitos(inFile, 4000);

    /// get an int: TODO
    unsigned int numSym = inFile.get() + 1;
    vector<unsigned int> vec;
    vector<char> sym(numSym);

    // read the symbols into vector
    for (int i = 0; i < numSym; i++) {
        sym[i] = inFile.get();
    }

    // read structure code
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
    }
}
