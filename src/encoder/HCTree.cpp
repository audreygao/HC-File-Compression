/**
 * Author: Ya Gao, Qingyang Xu
 * Email: yag003@ucsd.edu, q4xu@ucsd.edu
 * Description:
 */
#include "HCTree.hpp"
#include <vector>
using namespace std;

/* TODO */

void deleteAll(HCNode* node) {
    if (node == 0) {
        return;
    }
    deleteAll(node->c0);
    deleteAll(node->c1);
    delete (node);
}

HCTree::~HCTree() { deleteAll(root); }

/* TODO */
void HCTree::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    std::cout << "h1" << endl;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) != 0) {
            std::cout << "h2" << endl;
            pq.push(new HCNode(freqs.at(i), i, 0, 0, 0));
        }
    }
    while (pq.size() > 1) {
        std::cout << "h3" << endl;

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
        std::cout << "h4" << endl;
    }

    // only 1 node in pq: is root
    root = pq.top();
    std::cout << "done running build" << endl;
}

/* TODO */
// void HCTree::encode(byte symbol, BitOutputStream& out) const {}

/* TODO */
void HCTree::encode(byte symbol, ostream& out) const {
    vector<char> vec;
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

/* TODO */
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/* TODO */
byte HCTree::decode(istream& in) const {
    // start from the root to traverse down to leave
    HCNode* cur = root;
    HCNode* leave;
    unsigned char nextChar;
    // loop till the leave
    while (cur != nullptr) {
        nextChar = in.get();

        // decide which branch to go
        if (nextChar == '0') {
            leave = cur;
            cur = cur->c0;

        } else {
            leave = cur;
            cur = cur->c1;
        }
    }

    // return founded leave
    return leave->symbol;
}
