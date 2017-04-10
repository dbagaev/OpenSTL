#include <Data/Stl.h>
#include <Math/Vector3d.h>

#include "gtest/gtest.h"

using namespace OpenSTL;

TEST(Test_Stl, Basic_Test) 
{
    Data::Stl stl;
    EXPECT_EQ(0, stl.getNumberPoints());
    EXPECT_EQ(0, stl.getNumberTriangles());

    Data::Point * p_points[3];
    Data::Point * p_point;

    p_point = stl.addPoint(0, 0, 0);
    EXPECT_EQ(1, stl.getNumberPoints());
    EXPECT_FALSE(p_point == nullptr);
    p_points[0] = p_point;

    p_point = stl.addPoint(0, 0, 0);
    EXPECT_EQ(1, stl.getNumberPoints());
    EXPECT_EQ(p_points[0], p_point);

    p_point = stl.addPoint(100, 0, 0);
    EXPECT_EQ(2, stl.getNumberPoints());
    EXPECT_FALSE(p_point == nullptr);
    p_points[1] = p_point;

    p_point = stl.addPoint(100, 0, 0);
    EXPECT_EQ(2, stl.getNumberPoints());
    EXPECT_EQ(p_points[1], p_point);

    p_point = stl.addPoint(0, 100, 0);
    EXPECT_EQ(3, stl.getNumberPoints());
    EXPECT_FALSE(p_point == nullptr);
    p_points[2] = p_point;

    p_point = stl.addPoint(0, 100, 0);
    EXPECT_EQ(3, stl.getNumberPoints());
    EXPECT_EQ(p_points[2], p_point);

    auto p_triangle = stl.addTriangle(p_points[0], p_points[1], p_points[2]);
    EXPECT_FALSE(p_triangle == nullptr);
    EXPECT_EQ(3, stl.getNumberPoints());
    EXPECT_EQ(1, stl.getNumberTriangles());

    p_triangle = stl.addTriangle(Math::Vector3dDouble(0, 0, 0), Math::Vector3dDouble(100, 0, 0), Math::Vector3dDouble(0, 0, 100));
    EXPECT_FALSE(p_triangle == nullptr);
    EXPECT_EQ(4, stl.getNumberPoints());
    EXPECT_EQ(2, stl.getNumberTriangles());

    p_triangle = stl.addTriangle(Math::Vector3dDouble(0, 0, 0), Math::Vector3dDouble(100, 0, 0), Math::Vector3dDouble(0, 0, 100));
    EXPECT_FALSE(p_triangle == nullptr);
    EXPECT_EQ(4, stl.getNumberPoints());
    EXPECT_EQ(3, stl.getNumberTriangles());
  }
