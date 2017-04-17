#pragma once

#ifndef mesh_DATA_POINTNEIGHTBOURS_H_
#define mesh_DATA_POINTNEIGHTBOURS_H_

#include "Attributes.h"
#include "Point.h"
#include "Triangle.h"

#include <memory>

namespace mesh {

namespace Data {

class TriangleSurface;

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

protected:
    TriangleSurface * _Stl;

public:
    PointNeighbours(TriangleSurface * p_stl) : _Stl(p_stl)
    {
    }
};

namespace Details
{

template <typename NeighbourType>
class NeighboursInfo
{
public:
    size_t getCount();
    NeighbourType * getNeighbour(size_t _idx);
};

}  // namespace Datail

}  // namespace Data

}  // namespace mesh

#endif  // mesh_DATA_POINTNEIGHTBOURS_H_



