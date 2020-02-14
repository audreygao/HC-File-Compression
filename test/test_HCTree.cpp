#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256, 0);
        freqs['a'] = 2;
        freqs['b'] = 3;
        freqs['c'] = 3;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "10");
}
TEST_F(SimpleHCTreeFixture, TEST_ENCODE_2) {
    ostringstream os;
    tree.encode('b', os);
    ASSERT_EQ(os.str(), "0");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("10");
    ASSERT_EQ(tree.decode(is), 'a');
}
TEST_F(SimpleHCTreeFixture, TEST_DECODE2) {
    istringstream is1("0");
    ASSERT_EQ(tree.decode(is1), 'b');
}
TEST_F(SimpleHCTreeFixture, TEST_DECODE3) {
    istringstream is2("11");
    ASSERT_EQ(tree.decode(is2), 'c');
}