/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description: include description of
 * methods of a HCTree
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

/**
 * a class, instances of which are HCTree
 * built with huffman encoding method
 */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    /**
     * initialize an empty HCTree
     */
    HCTree() { root = nullptr; }

    /**
     * destructor of HCTree
     */
    ~HCTree();

    /**
     * build the HCTree with the given vector of frequencies
     * param: freqs - vector containing frequencies of symbol
     * at each index
     */
    void build(const vector<unsigned int>& freqs);

    /* TODO: add function header */
    void encode(byte symbol, BitOutputStream& out) const;

    /**
     * encode the given symbol with the HCtree
     * and write the code to the ostream
     * param: - symbol: symbol to be encoded
     *  - out: ostream to write to
     */
    void encode(byte symbol, ostream& out) const;

    /* TODO: add function header */
    byte decode(BitInputStream& in) const;

    /**
     * read from the istream and decode
     * Return the decoded symbo
     * param: in - istream to read from
     */
    byte decode(istream& in) const;
    /**
     * Return whether the HCTree is empty
     */
    bool empty();
};

#endif  // HCTREE_HPP
