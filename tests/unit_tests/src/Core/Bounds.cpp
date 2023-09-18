#include <gtest/gtest.h>

#include <Core/Bounds.hpp>

using namespace Morph;


TEST(CoreTypes, not_zero_true) {
    size_t value = 5;
    not_zero<size_t> value_not_zero(value);
    ASSERT_EQ(value_not_zero.get(), 5);
}

/*
TEST(CoreTypes, not_zero_false) {
    size_t value = 0;
    not_zero<size_t> value_not_zero(value);
}*/