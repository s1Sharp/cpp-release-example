#include <gtest/gtest.h>
#include <fancy_function.h>
#include <fancy_version.h>

TEST(FancyFunction_BeFancy, BasicAssertions) {
  auto ff = fancy_function();

  EXPECT_STREQ(ff.be_fancy(42).c_str(), "I am fancy one a level of 42");
}

TEST(FancyVersion, BasicAssertions) {

    auto fvmj = get_major_version();
    auto fvmn = get_minor_version();
    auto fvpt = get_patch_version();

    ASSERT_GE(fvmj, 1);
    ASSERT_GE(fvmn, 0);
    ASSERT_GE(fvpt, 0);
}
