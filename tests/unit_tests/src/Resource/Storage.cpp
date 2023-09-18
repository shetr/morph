#include <gtest/gtest.h>

#include <Resource/Storage.hpp>

using namespace Morph;

TEST(ResourceStorage, write_read) {
    string filename = "test.txt";
    string contents = "some testing \n words, { some more \n words, [bla, bla] }";

    ResourceStorage::WriteToFile(filename, contents);
    opt<string> contentsAfterRead = ResourceStorage::ReadFile(filename);
    ASSERT_TRUE(contentsAfterRead.has_value());
    ASSERT_EQ(contents, contentsAfterRead.value());
}