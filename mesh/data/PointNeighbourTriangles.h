#pragma once

#ifndef mesh_DATA_POINTNEIGHBOURTRIANGLES_H_
#define mesh_DATA_POINTNEIGHBOURTRIANGLES_H_

#include "AttributeMap.h"
#include "Point.h"
#include "Triangle.h"

#include <memory>
#include <set>
#include <algorithm>

namespace mesh { namespace Data {

class TriangleSurface;

class PointNeighbourTriangles : public AttributeMap<Point, std::set<Data::Triangle *>>
{
public:
	PointNeighbourTriangles(TriangleSurface * p_stl)
	{
    }
};


} }  // Namespace mesh::Data


#endif /* mesh_DATA_POINTNEIGHBOURTRIANGLES_H_ */
