#include <Data/Triangle.h>
#include <Data/Point.h>
#include <Math/Vector3d.h>
#include <Math/Box.h>

#include "gtest/gtest.h"

#include <memory>

using namespace mesh;

TEST(Test_triangle, Intersect_Test_BBoxSeparable)
{
    Math::Box<double, 3> b({ 0, 0, 0 }, { 10, 10, 10 });

    auto p1 = std::make_unique<Data::Point>(16, 6, 5);
    auto p2 = std::make_unique<Data::Point>(11, 16, 5);
    auto p3 = std::make_unique<Data::Point>(16, 20, 7);

    Data::Triangle t(p1.get(), p2.get(), p3.get());

    ASSERT_FALSE(intersects(t, b));
}

TEST(Test_triangle, Intersect_Test_TriangleNormalSeparable)
{
    Math::Box<double, 3> b({ 0, 0, 0 }, { 10, 10, 10 });

    auto p1 = std::make_unique<Data::Point>(16, 6, 5);
    auto p2 = std::make_unique<Data::Point>(6, 16, 5);
    auto p3 = std::make_unique<Data::Point>(11, 11, 15);

    Data::Triangle t(p1.get(), p2.get(), p3.get());

    ASSERT_FALSE(intersects(t, b));
}

TEST(Test_triangle, Intersect_Test_TriangleNormalNotSeparable)
{
    Math::Box<double, 3> b({ 0, 0, 0 }, { 10, 10, 10 });

    auto p1 = std::make_unique<Data::Point>(16, 6, 5);
    auto p2 = std::make_unique<Data::Point>(6, 16, 5);
    auto p3 = std::make_unique<Data::Point>(16, 16, 7);

    Data::Triangle t(p1.get(), p2.get(), p3.get());

    ASSERT_FALSE(intersects(t, b));
}

TEST(Test_triangle, Intersect_Test_IntersectsAllTests)
{
    Math::Box<double, 3> b({ 0, 0, 0 }, { 10, 10, 10 });

    auto p1 = std::make_unique<Data::Point>(14, 4, 5);
    auto p2 = std::make_unique<Data::Point>(4, 14, 5);
    auto p3 = std::make_unique<Data::Point>(16, 16, 7);

    Data::Triangle t(p1.get(), p2.get(), p3.get());

    ASSERT_TRUE(intersects(t, b));

}