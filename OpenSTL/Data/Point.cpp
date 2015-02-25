#include "Point.h"

#include <Math/Vector3d.h>

using namespace OpenSTL::Data;

namespace OpenSTL {

Point::Point()
{
}

Point::Point(const Point & other) : _Position(other._Position)
{

}

Point::Point(double x, double y, double z) : _Position(x, y, z)
{

}

Point::Point(const Math::Vector3d<double> & pos) : _Position(pos)
{

}

Point::~Point()
{

}


//----------------------------------------------------------------------------------------------------

bool Point::ComparePosition::operator()(const Point * p1, const Point * p2) const
{
    return compare(p1->_Position, p2->_Position);
}

bool Point::ComparePosition::compare(const Math::Vector3dDouble & v1, const Math::Vector3dDouble & v2) const
{
    if (v1[0] == v2[0])
    {
        if (v1[1] == v2[1])
            return v1[2] < v2[2];
        else
            return v1[1] < v2[1];
    }
    else
        return v1[0] < v2[0];
}

}