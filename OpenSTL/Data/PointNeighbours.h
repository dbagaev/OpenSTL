#pragma once

#include "Attributes.h"
#include "Point.h"
#include "Triangle.h"

#include <memory>

namespace OpenSTL {

namespace Data {


template <typename Me, typename Neighbour>
class Neighbours
{
};

template <typename Neighbour>
class PointNeighbours : public Neighbours<Neighbour, Point>
{
private:
    typedef std::vector<Neighbour *> NeighbourData;
    typedef NeighbourData NeighbourDataPtr;

    std::unique_ptr<AttributesImpl<NeighbourDataPtr, Me>> _Neighbours;
    Stl _Stl;

public:
    Neighbours(Stl * p_stl)
    {
    _Neighbours.reset(p_stl->createPointAttributes<NeighbourDataPtr>());
    }
};

class PointNeighbourTriangles : public PointNeighbours< Triangle >
{
public:
    void Rebuild();
};


}  // namespace Data

}  // namespace OpenSTL




