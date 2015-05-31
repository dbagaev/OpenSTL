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
}
