#include <Data/Stl.h>
#include <Data/PointNeighbourTriangles.h>

#include "gtest/gtest.h"

using namespace OpenSTL;

TEST(Test_PointNeighbourTriangles, Simple)
{
	Data::Stl stl;

	Data::Point * ps[3];
	ps[0] = stl.addPoint(0, 0, 0);
	ps[1] = stl.addPoint(0, 100, 0);
	ps[2] = stl.addPoint(0, 0, 100);

	auto p_tri = stl.addTriangle(ps[0], ps[1], ps[2]);

	auto neighbours = stl.createPointAttributes<Data::PointNeighbourTriangles>();
	neighbours->update();

	EXPECT_EQ(1, neighbours->getValue(ps[0]).size());
	EXPECT_EQ(1, neighbours->getValue(ps[1]).size());
	EXPECT_EQ(1, neighbours->getValue(ps[2]).size());

    EXPECT_EQ(p_tri, neighbours->getValue(ps[0]).at(0));
    EXPECT_EQ(p_tri, neighbours->getValue(ps[1]).at(0));
    EXPECT_EQ(p_tri, neighbours->getValue(ps[2]).at(0));

}
