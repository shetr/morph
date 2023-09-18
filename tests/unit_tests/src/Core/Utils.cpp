
#include <gtest/gtest.h>

#include <Core/Utils.hpp>

using namespace Morph;


TEST(CoreUtils, match_int) {
  variant<int, string> v;
  v = 0;
  bool variantIsInt = false;
  match(v,
    [&](int arg) { variantIsInt = true; },
    [](const string& arg) {  }
  );
  ASSERT_TRUE(variantIsInt);
}

TEST(CoreUtils, match_not_int) {
  variant<int, string> v;
  v = "test";
  bool variantIsNotInt = false;
  match(v,
    [](int arg) { },
    [&](const auto& arg) { variantIsNotInt = true; }
  );
  ASSERT_TRUE(variantIsNotInt);
}

TEST(CoreUtils, ivec3_to_ostream) {
  ivec3 v = {0, 1, 2};
  string vstring = "0, 1, 2";
  std::stringstream ss;
  ss << v;
  ASSERT_EQ(ss.str(), vstring);
}

TEST(CoreUtils, imat2_to_ostream) {
  imat2 m;
  m[0][0] = 0; m[1][0] = 1;
  m[0][1] = 2; m[1][1] = 3;
  string mstring = "\n0, 1\n2, 3";
  std::stringstream ss;
  ss << m;
  ASSERT_EQ(ss.str(), mstring);
}