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
    ASSERT_EQ(0, s0.first.min()[0]);
    ASSERT_EQ(1, s0.first.max()[0]);
    ASSERT_EQ(1, s0.second.min()[0]);
    ASSERT_EQ(2, s0.second.max()[0]);

    ASSERT_EQ(0, s0.first.min()[1]);
    ASSERT_EQ(0, s0.first.min()[2]);
    ASSERT_EQ(2, s0.first.max()[1]);
    ASSERT_EQ(2, s0.first.max()[2]);
    ASSERT_EQ(0, s0.second.min()[1]);
    ASSERT_EQ(0, s0.second.min()[2]);
    ASSERT_EQ(2, s0.second.max()[1]);
    ASSERT_EQ(2, s0.second.max()[2]);

    auto s1 = bx.split(1);
    ASSERT_EQ(0, s1.first.min()[1]);
    ASSERT_EQ(1, s1.first.max()[1]);
    ASSERT_EQ(1, s1.second.min()[1]);
    ASSERT_EQ(2, s1.second.max()[1]);

    ASSERT_EQ(0, s1.first.min()[0]);
    ASSERT_EQ(0, s1.first.min()[2]);
    ASSERT_EQ(2, s1.first.max()[0]);
    ASSERT_EQ(2, s1.first.max()[2]);
    ASSERT_EQ(0, s1.second.min()[0]);
    ASSERT_EQ(0, s1.second.min()[2]);
    ASSERT_EQ(2, s1.second.max()[0]);
    ASSERT_EQ(2, s1.second.max()[2]);

    auto s2 = bx.split(2);
    ASSERT_EQ(0, s2.first.min()[2]);
    ASSERT_EQ(1, s2.first.max()[2]);
    ASSERT_EQ(1, s2.second.min()[2]);
    ASSERT_EQ(2, s2.second.max()[2]);

    ASSERT_EQ(0, s2.first.min()[0]);
    ASSERT_EQ(0, s2.first.min()[1]);
    ASSERT_EQ(2, s2.first.max()[0]);
    ASSERT_EQ(2, s2.first.max()[1]);
    ASSERT_EQ(0, s2.second.min()[0]);
    ASSERT_EQ(0, s2.second.min()[1]);
    ASSERT_EQ(2, s2.second.max()[0]);
    ASSERT_EQ(2, s2.second.max()[1]);


}