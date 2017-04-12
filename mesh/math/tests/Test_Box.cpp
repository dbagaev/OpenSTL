#include <Math/Box.h>
#include <Math/Vector3d.h>

#include "gtest/gtest.h"

using namespace mesh::Math;

TEST(Test_Box, Split) 
{
    Vector<double, 3> c1(0, 0, 0);
    Vector<double, 3> c2(2, 2, 2);
    Box<double, 3> bx(c1, c2);

    auto s0 = bx.split(0);
    ASSERT_EQ(0, s0.first.minPosition()[0]);
    ASSERT_EQ(1, s0.first.maxPosition()[0]);
    ASSERT_EQ(1, s0.second.minPosition()[0]);
    ASSERT_EQ(2, s0.second.maxPosition()[0]);

    ASSERT_EQ(0, s0.first.minPosition()[1]);
    ASSERT_EQ(0, s0.first.minPosition()[2]);
    ASSERT_EQ(2, s0.first.maxPosition()[1]);
    ASSERT_EQ(2, s0.first.maxPosition()[2]);
    ASSERT_EQ(0, s0.second.minPosition()[1]);
    ASSERT_EQ(0, s0.second.minPosition()[2]);
    ASSERT_EQ(2, s0.second.maxPosition()[1]);
    ASSERT_EQ(2, s0.second.maxPosition()[2]);

    auto s1 = bx.split(1);
    ASSERT_EQ(0, s1.first.minPosition()[1]);
    ASSERT_EQ(1, s1.first.maxPosition()[1]);
    ASSERT_EQ(1, s1.second.minPosition()[1]);
    ASSERT_EQ(2, s1.second.maxPosition()[1]);

    ASSERT_EQ(0, s1.first.minPosition()[0]);
    ASSERT_EQ(0, s1.first.minPosition()[2]);
    ASSERT_EQ(2, s1.first.maxPosition()[0]);
    ASSERT_EQ(2, s1.first.maxPosition()[2]);
    ASSERT_EQ(0, s1.second.minPosition()[0]);
    ASSERT_EQ(0, s1.second.minPosition()[2]);
    ASSERT_EQ(2, s1.second.maxPosition()[0]);
    ASSERT_EQ(2, s1.second.maxPosition()[2]);

    auto s2 = bx.split(2);
    ASSERT_EQ(0, s2.first.minPosition()[2]);
    ASSERT_EQ(1, s2.first.maxPosition()[2]);
    ASSERT_EQ(1, s2.second.minPosition()[2]);
    ASSERT_EQ(2, s2.second.maxPosition()[2]);

    ASSERT_EQ(0, s2.first.minPosition()[0]);
    ASSERT_EQ(0, s2.first.minPosition()[1]);
    ASSERT_EQ(2, s2.first.maxPosition()[0]);
    ASSERT_EQ(2, s2.first.maxPosition()[1]);
    ASSERT_EQ(0, s2.second.minPosition()[0]);
    ASSERT_EQ(0, s2.second.minPosition()[1]);
    ASSERT_EQ(2, s2.second.maxPosition()[0]);
    ASSERT_EQ(2, s2.second.maxPosition()[1]);


}