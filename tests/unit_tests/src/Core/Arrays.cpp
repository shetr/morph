
#include <gtest/gtest.h>

#include <Core/Arrays.hpp>

using namespace Morph;


TEST(CoreArrays, array2d_basic) {
    array<int, 6> basicArray = {
        6, 7, 8, 9, 10, 11
    };
    array<int, 6> basicArray2 = {
        1, 1, 1, 1, 1, 1
    };
    array2d<int, 3, 2> basicArray2d = {
        6, 7, 8,
        9, 10, 11
    };
    array2d<int, 3, 2> basicArray2d2(1);
    // dimensions
    ASSERT_FALSE(basicArray2d.empty());
    ASSERT_EQ(basicArray2d.size(), 6);
    ASSERT_EQ(basicArray2d.dim(), uvec2(3, 2));
    ASSERT_FALSE(basicArray2d2.empty());
    ASSERT_EQ(basicArray2d2.size(), 6);
    ASSERT_EQ(basicArray2d2.dim(), uvec2(3, 2));
    // index conversion
    ASSERT_EQ(basicArray2d.index(1, 0), 1);
    ASSERT_EQ(basicArray2d.index(1, 1), 4);
    ASSERT_EQ(basicArray2d.index(uvec2(1, 0)), 1);
    ASSERT_EQ(basicArray2d.index(uvec2(1, 1)), 4);
    // indexing
    ASSERT_EQ(basicArray2d(1, 0), 7);
    ASSERT_EQ(basicArray2d(1, 1), 10);
    ASSERT_EQ(basicArray2d[uvec2(1, 0)], 7);
    ASSERT_EQ(basicArray2d[uvec2(1, 1)], 10);
    ASSERT_EQ(basicArray2d[1], 7);
    ASSERT_EQ(basicArray2d[4], 10);
    // correct initialization
    ASSERT_TRUE(std::equal(basicArray2d.begin(), basicArray2d.end(), basicArray.begin()));
    ASSERT_TRUE(std::equal(basicArray2d2.begin(), basicArray2d2.end(), basicArray2.begin()));
    //swap
    basicArray2d.swap(basicArray2d2);
    ASSERT_TRUE(std::equal(basicArray2d.begin(), basicArray2d.end(), basicArray2.begin()));
    ASSERT_TRUE(std::equal(basicArray2d2.begin(), basicArray2d2.end(), basicArray.begin()));
    std::swap(basicArray2d, basicArray2d2);
    ASSERT_TRUE(std::equal(basicArray2d.begin(), basicArray2d.end(), basicArray.begin()));
    ASSERT_TRUE(std::equal(basicArray2d2.begin(), basicArray2d2.end(), basicArray2.begin()));
    // value change
    basicArray2d(1, 0) = 2;
    ASSERT_EQ(basicArray2d(1, 0), 2);
    // base constructor
    ASSERT_EQ(int(), 0);
    array<int, 6> basicArray3 = {
        0, 0, 0, 0, 0, 0
    };
    array2d<int, 3, 2> basicArray2d3;
    ASSERT_TRUE(std::equal(basicArray2d3.begin(), basicArray2d3.end(), basicArray3.begin()));
}

TEST(CoreArrays, array3d_basic) {
    array<int, 12> basicArray = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    };
    array<int, 12> basicArray2 = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    array3d<int, 3, 2, 2> basicArray3d = {
        0, 1, 2,
        3, 4, 5,

        6, 7, 8,
        9, 10, 11
    };
    array3d<int, 3, 2, 2> basicArray3d2(1);
    // dimensions
    ASSERT_FALSE(basicArray3d.empty());
    ASSERT_EQ(basicArray3d.size(), 12);
    ASSERT_EQ(basicArray3d.dim(), uvec3(3, 2, 2));
    ASSERT_FALSE(basicArray3d2.empty());
    ASSERT_EQ(basicArray3d2.size(), 12);
    ASSERT_EQ(basicArray3d2.dim(), uvec3(3, 2, 2));
    // index conversion
    ASSERT_EQ(basicArray3d.index(1, 0, 0), 1);
    ASSERT_EQ(basicArray3d.index(1, 1, 1), 10);
    ASSERT_EQ(basicArray3d.index(uvec3(1, 0, 0)), 1);
    ASSERT_EQ(basicArray3d.index(uvec3(1, 1, 1)), 10);
    // indexing
    ASSERT_EQ(basicArray3d(1, 0, 0), 1);
    ASSERT_EQ(basicArray3d(1, 1, 1), 10);
    ASSERT_EQ(basicArray3d[uvec3(1, 0, 0)], 1);
    ASSERT_EQ(basicArray3d[uvec3(1, 1, 1)], 10);
    ASSERT_EQ(basicArray3d[1], 1);
    ASSERT_EQ(basicArray3d[10], 10);
    // correct initialization
    ASSERT_TRUE(std::equal(basicArray3d.begin(), basicArray3d.end(), basicArray.begin()));
    ASSERT_TRUE(std::equal(basicArray3d2.begin(), basicArray3d2.end(), basicArray2.begin()));
    //swap
    basicArray3d.swap(basicArray3d2);
    ASSERT_TRUE(std::equal(basicArray3d.begin(), basicArray3d.end(), basicArray2.begin()));
    ASSERT_TRUE(std::equal(basicArray3d2.begin(), basicArray3d2.end(), basicArray.begin()));
    std::swap(basicArray3d, basicArray3d2);
    ASSERT_TRUE(std::equal(basicArray3d.begin(), basicArray3d.end(), basicArray.begin()));
    ASSERT_TRUE(std::equal(basicArray3d2.begin(), basicArray3d2.end(), basicArray2.begin()));
    // value change
    basicArray3d(1, 0, 0) = 2;
    ASSERT_EQ(basicArray3d(1, 0, 0), 2);
    // base constructor
    ASSERT_EQ(int(), 0);
    array<int, 12> basicArray3 = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    array3d<int, 3, 2, 2> basicArray3d3;
    ASSERT_TRUE(std::equal(basicArray3d3.begin(), basicArray3d3.end(), basicArray3.begin()));
}