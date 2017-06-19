#include <Data/TriangleSurface.h>
#include <Math/Vector3d.h>

#include "gtest/gtest.h"

using namespace mesh;

TEST(Test_surface, Basic_Test) 
{
    Data::TriangleSurface surface;
    EXPECT_EQ(0, surface.getNumberPoints());
    EXPECT_EQ(0, surface.getNumberTriangles());

    Data::Point * p_points[3];
    Data::Point * p_point;

    p_point = surface.addPoint(0, 0, 0);
    EXPECT_EQ(1, surface.getNumberPoints());
    EXPECT_FALSE(p_point == nullptr);
    p_points[0] = p_point;

    p_point = surface.addPoint(0, 0, 0);
    EXPECT_EQ(1, surface.getNumberPoints());
    EXPECT_EQ(p_points[0], p_point);

    p_point = surface.addPoint(100, 0, 0);
    EXPECT_EQ(2, surface.getNumberPoints());
    EXPECT_FALSE(p_point == nullptr);
    p_points[1] = p_point;

    p_point = surface.addPoint(100, 0, 0);
    EXPECT_EQ(2, surface.getNumberPoints());
    EXPECT_EQ(p_points[1], p_point);

    p_point = surface.addPoint(0, 100, 0);
    EXPECT_EQ(3, surface.getNumberPoints());
    EXPECT_FALSE(p_point == nullptr);
    p_points[2] = p_point;

    p_point = surface.addPoint(0, 100, 0);
    EXPECT_EQ(3, surface.getNumberPoints());
    EXPECT_EQ(p_points[2], p_point);

    auto p_triangle = surface.addTriangle(p_points[0], p_points[1], p_points[2]);
    EXPECT_FALSE(p_triangle == nullptr);
    EXPECT_EQ(3, surface.getNumberPoints());
    EXPECT_EQ(1, surface.getNumberTriangles());

    p_triangle = surface.addTriangle(Math::Vector3dDouble{0, 0, 0}, Math::Vector3dDouble{100, 0, 0}, Math::Vector3dDouble{0, 0, 100});
    EXPECT_FALSE(p_triangle == nullptr);
    EXPECT_EQ(4, surface.getNumberPoints());
    EXPECT_EQ(2, surface.getNumberTriangles());

    p_triangle = surface.addTriangle(Math::Vector3dDouble{0, 0, 0}, Math::Vector3dDouble{100, 0, 0}, Math::Vector3dDouble{0, 0, 100});
    EXPECT_FALSE(p_triangle == nullptr);
    EXPECT_EQ(4, surface.getNumberPoints());
    EXPECT_EQ(3, surface.getNumberTriangles());
  }
