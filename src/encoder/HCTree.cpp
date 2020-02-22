/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description: include functionalites of a HCTree
 */
#include "HCTree.hpp"
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
using namespace std;

/**
 * helper method of destructor
 * recursively delete all the HCNode
 */
void deleteAll(HCNode* node) {
    // base case
    if (node == 0) {
        return;
    }

    // delete children then itself
    deleteAll(node->c0);
    deleteAll(node->c1);
    delete (node);
}

/**destructor of HCTree **/
HCTree::~HCTree() { deleteAll(root); }

/**
 * build the HCTree with the given vector of frequencies
 * param: freqs - vector containing frequencies of symbol
 * at each index
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) != 0) {
            pq.push(new HCNode(freqs.at(i), i, 0, 0, 0));
        }
    }
    while (pq.size() > 1) {
        // pop the first two node with lowest freq
        HCNode* n0 = pq.top();
        pq.pop();
        // if it's a leave, add to leaves vector
        if (n0->c0 == 0 && n0->c1 == 0) {
            leaves.push_back(n0);
        }

        HCNode* n1 = pq.top();
        pq.pop();
        // if it's a leave, add to leaves vector
        if (n1->c0 == 0 && n1->c1 == 0) {
            leaves.push_back(n1);
        }

        // create parent node with added freq
        int pCount = n0->count + n1->count;
        HCNode* parent = new HCNode(pCount, n0->symbol, n0, n1, 0);

        // update parent of the two poped nodes
        n0->p = parent;
        n1->p = parent;

        // push new parent node to pq
        pq.push(parent);
    }

    // only 1 node in pq: is root
    if (pq.size() == 1) {
        root = pq.top();
    }
}

/**
 * Write the encoding bits of given symbol to the given BitOutputStream.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    vector<unsigned int> vec;

    // if only a root / only 1 type of symbol in file
    if (root->c0 == 0 && root->c1 == 0) {
        out.writeBit(0);
        return;
    }

    // find node ptr with the symbol
    for (int i = 0; i < leaves.size(); i++) {
        // node ptr found
        if (leaves.at(i)->symbol == symbol) {
            HCNode* node = leaves.at(i);

            HCNode* par = node->p;

            // go up the HCTree till the root
            while (par != 0) {
                // determine if it's 0 or 1
                if (par->c0 == node) {
                    // add to front of vector
                    vec.insert(vec.begin(), (unsigned int)0);
                } else if (par->c1 == node) {
                    vec.insert(vec.begin(), (unsigned int)1);
                }

                // set par to its parent
                node = par;
                par = par->p;
            }
            break;  // break out of for loop
        }
    }

    // write each bit
    for (char i : vec) {
        out.writeBit(i);
    }
}

/**
 * encode the given symbol with the HCtree
 * and write the code to the ostream
 * param: - symbol: symbol to be encoded
 *  - out: ostream to write to
 */
void HCTree::encode(byte symbol, ostream& out) const {
    vector<char> vec;

    if (root->c0 == 0 && root->c1 == 0) {
        out.put('0');
        return;
    }

    // find node ptr with the symbol
    for (int i = 0; i < leaves.size(); i++) {
        // node ptr found
        if (leaves.at(i)->symbol == symbol) {
            HCNode* node = leaves.at(i);

            HCNode* par = node->p;

            // go up the HCTree till the root
            while (par != 0) {
                // determine if it's 0 or 1
                if (par->c0 == node) {
                    // add to front of vector
                    vec.insert(vec.begin(), '0');
                } else if (par->c1 == node) {
                    vec.insert(vec.begin(), '1');
                }

                // set par to its parent
                node = par;
                par = par->p;
            }
            break;  // break out of for loop
        }
    }

    // write each bit as char to ostream out
    for (char i : vec) {
        out.put(i);
    }
}

/**
 * Decode the sequence of bits from the given BitInputStream to return the coded
 * symbol.
 */
byte HCTree::decode(BitInputStream& in) const {
    // only a root in the tree
    if (root->c0 == 0 && root->c1 == 0) {
        in.readBit();
        return root->symbol;
    }

    // start from the root to traverse down to leave
    HCNode* cur = root;
    unsigned int bitInt;
    // loop till the leave
    while (cur != nullptr) {
        bitInt = in.readBit();
        // decide which branch to go
        if (bitInt == 0) {
            cur = cur->c0;

        } else {
            cur = cur->c1;
        }
        if (cur->c1 == 0 && cur->c0 == 0) {
            break;
        }
    }
    // return founded leave
    return cur->symbol;
}

/**
 * read from the istream and decode
 * Return the decoded symbo
 * param: in - istream to read from
 */
byte HCTree::decode(istream& in) const {
    if (root->c0 == 0 && root->c1 == 0) {
        in.get();
        return root->symbol;
    }

    // start from the root to traverse down to leave
    HCNode* cur = root;
    unsigned char nextChar;
    // loop till the leave
    while (cur != nullptr) {
        nextChar = in.get();
        // decide which branch to go
        if (nextChar == '0') {
            cur = cur->c0;

        } else {
            cur = cur->c1;
        }
        if (cur->c1 == 0 && cur->c0 == 0) {
            break;
        }
    }

    // return founded leave
    return cur->symbol;
}

/**
 * Return whether the HCTree is empty
 */
bool HCTree::empty() { return root == 0; }

void HCTree::traverseAll(vector<unsigned char>& sym,
                         vector<unsigned int>& vec) {
    traverse(sym, vec, root);
}

void HCTree::traverse(vector<unsigned char>& sym, vector<unsigned int>& vec,
                      HCNode* node) {
    if (node->c0 != 0 && node->c1 != 0) {
        vec.push_back(1);
        traverse(sym, vec, node->c0);
        traverse(sym, vec, node->c1);
    } else {
        vec.push_back(0);
        sym.push_back(node->symbol);
        return;
    }
}

void HCTree::rebuildAll(vector<unsigned char>& sym, vector<unsigned int>& vec) {
    root = new HCNode(0, 0, 0, 0, 0);
    rebuild(sym, vec, root, 0, 0);
}

void HCTree::rebuild(vector<unsigned char>& sym, vector<unsigned int>& vec,
                     HCNode* node, int symInd, int vecInd) {
    unsigned int bit = vec[0];
    vec.erase(vec.begin());
    // vecInd++;
    if (bit == 0) {
        node->symbol = sym[0];
        // symInd++;
        leaves.push_back(node);
        sym.erase(sym.begin());
        return;
    }

    node->c0 = new HCNode(0, 0, 0, 0, 0);
    node->c0->p = node;
    node->c1 = new HCNode(0, 0, 0, 0, 0);
    node->c1->p = node;
    rebuild(sym, vec, node->c0, symInd, vecInd);
    rebuild(sym, vec, node->c1, symInd, vecInd);
}