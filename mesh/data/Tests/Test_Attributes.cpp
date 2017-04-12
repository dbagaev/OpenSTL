#include <Data/TriangleSurface.h>
#include <Data/PointNeighbourTriangles.h>

#include "gtest/gtest.h"

using namespace mesh;

TEST(Test_Attributes, Simple)
{
    Data::TriangleSurface surface;

    Data::Point * ps[3];
    ps[0] = surface.addPoint(0, 0, 0);
    ps[1] = surface.addPoint(0, 100, 0);
    ps[2] = surface.addPoint(0, 0, 100);

    auto p_tri = surface.addTriangle(ps[0], ps[1], ps[2]);

    // Check one flag attribute

    auto flags = surface.createPointAttributes<Data::PointFlagAttribute>();

    flags->setValue(ps[0], 1);
    flags->setValue(ps[1], 2);
    flags->setValue(ps[2], 3);

    EXPECT_EQ(1, flags->getValue(ps[0]));
    EXPECT_EQ(2, flags->getValue(ps[1]));
    EXPECT_EQ(3, flags->getValue(ps[2]));

    // Check second flag attribute

    auto flags2 = surface.createPointAttributes<Data::PointFlagAttribute>();

    flags2->setValue(ps[0], 21);
    flags2->setValue(ps[1], 22);
    flags2->setValue(ps[2], 23);

    EXPECT_EQ(21, flags2->getValue(ps[0]));
    EXPECT_EQ(22, flags2->getValue(ps[1]));
    EXPECT_EQ(23, flags2->getValue(ps[2]));

    // Conflicts with first one?

    EXPECT_EQ(1, flags->getValue(ps[0]));
    EXPECT_EQ(2, flags->getValue(ps[1]));
    EXPECT_EQ(3, flags->getValue(ps[2]));

    flags->setValue(ps[0], 11);
    flags->setValue(ps[1], 12);
    flags->setValue(ps[2], 13);

    EXPECT_EQ(11, flags->getValue(ps[0]));
    EXPECT_EQ(12, flags->getValue(ps[1]));
    EXPECT_EQ(13, flags->getValue(ps[2]));

}
