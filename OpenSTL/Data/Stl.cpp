#include "Stl.h"

#include <Math/Vector3d.h>

using namespace OpenSTL::Data;

namespace OpenSTL {

Stl::Stl()
{
}

Stl::Stl(const Stl & stl)
{
}

Stl::~Stl()
{
}

Stl & Stl::operator=(const Stl & other)
{
    return *this;
}

// Points collection
Point * Stl::addPoint(const Point * p_point)
{
    if (isMyPoint(p_point))
    { 
        return addPoint(p_point->_Position);
    }
    else 
        return const_cast<Point *>(p_point);
}

Point * Stl::addPoint(const Math::Vector3dDouble & position)
{
    Point * p_point = findPoint(position);
    if (p_point == nullptr)
    {
        _PointsPool.push_back(Point(position));
        p_point = &_PointsPool.back();
        _Points.insert(p_point);
    }
    
    return p_point;
}

Point * Stl::addPoint(double x, double y, double z)
{
    return addPoint(Math::Vector3dDouble(x, y, z));
}

void Stl::removePoint(Point * p_pount)
{
    if (!isMyPoint(p_pount))
        return;

    _Points.erase(p_pount);
}

bool Stl::isMyPoint(const Point * p_point) const
{
    auto p_found_point = _Points.find(const_cast<Point *>(p_point));
    if (p_found_point == _Points.end())
        return false;

    return (*p_found_point == p_point);
}

Point * Stl::findPoint(const Math::Vector3dDouble & position) const
{
    Point pt(position);
    auto p_found_point = _Points.find(&pt);
    return (p_found_point != _Points.end() ? *p_found_point : nullptr);
}

}