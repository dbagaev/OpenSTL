#pragma once

#ifndef OPENSTL_DATA_POINTNEIGHTBOURS_H_
#define OPENSTL_DATA_POINTNEIGHTBOURS_H_

#include "Attributes.h"
#include "Stl.h"
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

protected:
    Stl * _Stl;

public:
    PointNeighbours(Stl * p_stl) : _Stl(p_stl)
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

}  // namespace OpenSTL

#endif  // OPENSTL_DATA_POINTNEIGHTBOURS_H_



