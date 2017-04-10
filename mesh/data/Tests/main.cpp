#include "gtest/gtest.h"

// Tests positive input.
TEST(FooTest, BarTest)
{
    EXPECT_FALSE(false);
    EXPECT_TRUE(true);
    EXPECT_LE(1, 2);
    EXPECT_LE(1, 1);
}