#include <gtest/gtest.h>

#include <Core/Types.hpp>

using namespace Morph;

struct TestClass {
    int a = 5;
    int b = 2; 

    int f() {
        return 8 + b;
    }
};

TEST(CoreTypes, ref_dereference) {
    TestClass t;
    TestClass& t_nref = t;
    ref<TestClass> t_ref1 = t_nref;
    ref<TestClass> t_ref2(t_nref);
    ref<TestClass> t_ref3(t);
    ref<TestClass> t_ref4 = t;


    ASSERT_EQ(t_ref1->a, 5);
    ASSERT_EQ(t_ref2->a, 5);
    ASSERT_EQ(t_ref3->a, 5);
    ASSERT_EQ(t_ref4->a, 5);
    ASSERT_EQ((*t_ref1).a, 5);
    ASSERT_EQ((*t_ref2).a, 5);
    ASSERT_EQ((*t_ref3).a, 5);
    ASSERT_EQ((*t_ref4).a, 5);

    ASSERT_EQ(t_ref1->f(), 10);
    ASSERT_EQ(t_ref2->f(), 10);
    ASSERT_EQ(t_ref3->f(), 10);
    ASSERT_EQ(t_ref4->f(), 10);
    ASSERT_EQ((*t_ref1).f(), 10);
    ASSERT_EQ((*t_ref2).f(), 10);
    ASSERT_EQ((*t_ref3).f(), 10);
    ASSERT_EQ((*t_ref4).f(), 10);

    t_ref2->b = 3;

    ASSERT_EQ(t.f(), 11);
    ASSERT_EQ(t_ref1->f(), 11);
    ASSERT_EQ(t_ref2->f(), 11);
    ASSERT_EQ(t_ref3->f(), 11);
    ASSERT_EQ(t_ref4->f(), 11);
    ASSERT_EQ((*t_ref1).f(), 11);
    ASSERT_EQ((*t_ref2).f(), 11);
    ASSERT_EQ((*t_ref3).f(), 11);
    ASSERT_EQ((*t_ref4).f(), 11);

}

TEST(CoreTypes, ref_assign_value) {
    int a = 5;
    ref<int> ra = a;
    ASSERT_EQ(a, 5);
    ASSERT_EQ(ra, 5);
    ra.set(8);
    ASSERT_EQ(a, 8);
    ASSERT_EQ(ra, 8);
    ASSERT_TRUE(ra == 8);
}