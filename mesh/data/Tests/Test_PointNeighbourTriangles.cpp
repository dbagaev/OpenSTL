#include <Data/Stl.h>
#include <Data/PointNeighbourTriangles.h>

#include "gtest/gtest.h"

using namespace mesh;

TEST(Test_PointNeighbourTriangles, Simple)
{
	Data::Stl stl;

	Data::Point * ps[4];
	ps[0] = stl.addPoint(0, 0, 0);
	ps[1] = stl.addPoint(0, 100, 0);
	ps[2] = stl.addPoint(0, 0, 100);
    ps[3] = stl.addPoint(0, 100, 100);

	Data::Triangle * p_tri[2];
	p_tri[0] = stl.addTriangle(ps[0], ps[1], ps[2]);

    Data::PointNeighbourTriangles neighbours(&stl);
	neighbours.update();

	EXPECT_EQ(1, neighbours.get(*ps[0]).size());
	EXPECT_EQ(1, neighbours.get(*ps[1]).size());
	EXPECT_EQ(1, neighbours.get(*ps[2]).size());
    EXPECT_EQ(0, neighbours.get(*ps[3]).size());

    EXPECT_EQ(p_tri[0], *neighbours.get(*ps[0]).begin());
    EXPECT_EQ(p_tri[0], *neighbours.get(*ps[1]).begin());
    EXPECT_EQ(p_tri[0], *neighbours.get(*ps[2]).begin());

    p_tri[1] = stl.addTriangle(ps[3], ps[2], ps[1]);
    neighbours.update();

    EXPECT_EQ(1, neighbours.get(*ps[0]).size());
    EXPECT_EQ(2, neighbours.get(*ps[1]).size());
    EXPECT_EQ(2, neighbours.get(*ps[2]).size());
    EXPECT_EQ(1, neighbours.get(*ps[3]).size());

    EXPECT_TRUE(neighbours.get(*ps[0]).find(p_tri[0]) != neighbours.get(*ps[0]).end());
    EXPECT_TRUE(neighbours.get(*ps[1]).find(p_tri[0]) != neighbours.get(*ps[1]).end());
    EXPECT_TRUE(neighbours.get(*ps[1]).find(p_tri[1]) != neighbours.get(*ps[1]).end());
    EXPECT_TRUE(neighbours.get(*ps[2]).find(p_tri[0]) != neighbours.get(*ps[2]).end());
    EXPECT_TRUE(neighbours.get(*ps[2]).find(p_tri[1]) != neighbours.get(*ps[2]).end());
    EXPECT_TRUE(neighbours.get(*ps[3]).find(p_tri[1]) != neighbours.get(*ps[3]).end());

} 
