#include "Triangle.h"

using namespace OpenSTL::Data;

namespace OpenSTL {

Triangle::Triangle(const Point * p_point0, const Point * p_point1, const Point * p_point2)
{
    this->_Points[0] = const_cast<Point *>(p_point0);
    this->_Points[1] = const_cast<Point *>(p_point1);
    this->_Points[2] = const_cast<Point *>(p_point2);
}

Math::Vector3dDouble Triangle::normal() const
{
    return _Normal;
}










}
