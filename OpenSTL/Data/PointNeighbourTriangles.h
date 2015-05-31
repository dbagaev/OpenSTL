#pragma once

#ifndef OPENSTL_DATA_POINTNEIGHBOURTRIANGLES_H_
#define OPENSTL_DATA_POINTNEIGHBOURTRIANGLES_H_

#include "Attributes.h"
#include "Stl.h"
#include "Point.h"
#include "Triangle.h"
#include "PointNeighbours.h"

#include <memory>

namespace OpenSTL {

namespace Data {

/**
 * Class holds neighbours triangles for all points in the Stl.
 */

class PointNeighbourTrianglesAttributeTraits
{
    public:
        typedef Data::Point KeyType;
        typedef Details::NeighboursInfo<Data::Triangle> DataType;
        typedef Data::Triangle ** StorageType;
};

class PointNeighbourTrianglesImpl : public PointNeighbours< Triangle >
{
public:
	PointNeighbourTrianglesImpl(Stl * _stl) : PointNeighbours<Triangle>(_stl) {}

    void update();

    class NeighbourData
    {
    	//
    };
};

typedef Attribute<PointNeighbourTrianglesAttributeTraits, PointNeighbourTrianglesImpl> PointNeighbourTriangles;


}  // namespace Data

}  // namespace OpenSTL


#endif /* OPENSTL_DATA_POINTNEIGHBOURTRIANGLES_H_ */
