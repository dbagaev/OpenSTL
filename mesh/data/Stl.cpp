#include "Stl.h"

#include <Math/Vector3d.h>

using namespace mesh::Data;

namespace mesh {

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

Triangle * Stl::addTriangle(const Point * p_p0, const Point * p_p1, const Point * p_p2)
{
    _TrianglesPool.push_back( Triangle(p_p0, p_p1, p_p2) );
    Triangle * p_tri = &_TrianglesPool.back();
    _Triangles.insert(p_tri);

    return p_tri;
}

Triangle * Stl::addTriangle(const Math::Vector3dDouble & p0, const Math::Vector3dDouble & p1, const Math::Vector3dDouble & p2)
{
    Point * p_pts[3];

    p_pts[0] = addPoint(p0);
    p_pts[1] = addPoint(p1);
    p_pts[2] = addPoint(p2);

    return addTriangle(p_pts[0], p_pts[1], p_pts[2]);    
}

Triangle * Stl::addTriangle(const Triangle * p_tri)
{
    if (isMyTriangle(p_tri))
        return const_cast<Triangle *>(p_tri);
    else
      return addTriangle(p_tri->_Points[0], p_tri->_Points[1], p_tri->_Points[2]);
}

void Stl::removeTriangle(Triangle * p_tri)
{
    auto my_tri = _Triangles.find(p_tri);
    if (my_tri == _Triangles.end())
        return;

    _Triangles.erase(my_tri);
}

bool Stl::isMyTriangle(const Triangle * p_tri) const
{
  auto p_found_tri = _Triangles.find(const_cast<Triangle *>(p_tri));
  if (p_found_tri == _Triangles.end())
    return false;

  return (*p_found_tri == p_tri);
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
