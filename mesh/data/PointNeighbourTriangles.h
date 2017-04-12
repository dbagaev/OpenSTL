#pragma once

#ifndef mesh_DATA_POINTNEIGHBOURTRIANGLES_H_
#define mesh_DATA_POINTNEIGHBOURTRIANGLES_H_

#include "AttributeMap.h"
#include "TriangleSurface.h"
#include "Point.h"
#include "Triangle.h"
#include "PointNeighbours.h"

#include <memory>
#include <algorithm>

namespace mesh { namespace Data {


class PointNeighbourTriangles : public PointNeighbours<Data::Triangle>, public AttributeMap<Point, std::set<Data::Triangle *>>
{
public:
	PointNeighbourTriangles(TriangleSurface * p_stl) : PointNeighbours<Data::Triangle>(p_stl)
	{
    }

    __mesh_DATA_EXPORT void update();

    class NeighbourData
    {
    	//
    };
};


} }  // Namespace mesh::Data


#endif /* mesh_DATA_POINTNEIGHBOURTRIANGLES_H_ */
