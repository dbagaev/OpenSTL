#include "Triangle.h"

using namespace mesh::Data;

namespace mesh {

Triangle::Triangle(const Point * p_point0, const Point * p_point1, const Point * p_point2)
{
    this->_Points[0] = const_cast<Point *>(p_point0);
    this->_Points[1] = const_cast<Point *>(p_point1);
    this->_Points[2] = const_cast<Point *>(p_point2);

    _Normal = ((p_point2->position() - p_point0->position()) ^ (p_point1->position() - p_point0->position())).norm();
}

Math::Vector3dDouble Triangle::normal() const
{
    return _Normal;
}










}
