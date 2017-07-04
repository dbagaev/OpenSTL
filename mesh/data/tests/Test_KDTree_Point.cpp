#include <Data/KDTree.h>
#include <Data/Point.h>
#include <Data/TriangleSurface.h>

#include "gtest/gtest.h"

using namespace mesh;

TEST(Test_KDTree_Point, Poin_t) 
{
    Data::TriangleSurface surface;

    surface.addPoint(0, 0, 0);
    surface.addPoint(1, 1, 1);

    Data::KDTree<Data::Point> tree(surface.beginPoint(), surface.endPoint());

}