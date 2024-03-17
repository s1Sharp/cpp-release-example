#include <gtest/gtest.h>
#include <fancy_version.h>

TEST(FancyVersion, BasicAssertions) {

    auto fvmj = get_major_version();
    auto fvmn = get_minor_version();
    auto fvpt = get_patch_version();

    ASSERT_GE(fvmj, 1);
    ASSERT_GE(fvmn, 0);
    ASSERT_GE(fvpt, 0);
}
