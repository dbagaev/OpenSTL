#include "TriangleSurface.h"

#include <Math/Vector3d.h>

using namespace mesh::Data;

namespace mesh {

TriangleSurface::TriangleSurface()
{
}

TriangleSurface::TriangleSurface(const TriangleSurface & surface)
{
}

TriangleSurface::~TriangleSurface()
{
}

TriangleSurface & TriangleSurface::operator=(const TriangleSurface & surface)
{
    return *this;
}

// Points collection
Point * TriangleSurface::addPoint(const Point * p_point)
{
    if (isMyPoint(p_point))
    { 
        return addPoint(p_point->_Position);
    }
    else 
        return const_cast<Point *>(p_point);
}

Point * TriangleSurface::addPoint(const Math::Vector3dDouble & position)
{
    Point * p_point = findPoint(position);
    if (p_point == nullptr)
    {
        _PointsPool.emplace_back(position);
        p_point = &_PointsPool.back();
        m_PointIndexer.addIndex(*p_point);
        _Points.insert(p_point);
    }
    
    return p_point;
}

Point * TriangleSurface::addPoint(double x, double y, double z)
{
    return addPoint(Math::Vector3dDouble(x, y, z));
}

void TriangleSurface::removePoint(Point * p_pount)
{
    if (!isMyPoint(p_pount))
        return;

    _Points.erase(p_pount);
}

Triangle * TriangleSurface::addTriangle(const Point * p_p0, const Point * p_p1, const Point * p_p2)
{
    _TrianglesPool.push_back( Triangle(p_p0, p_p1, p_p2) );
    Triangle * p_tri = &_TrianglesPool.back();
    _Triangles.insert(p_tri);

    return p_tri;
}

Triangle * TriangleSurface::addTriangle(const Math::Vector3dDouble & p0, const Math::Vector3dDouble & p1, const Math::Vector3dDouble & p2)
{
    Point * p_pts[3];

    p_pts[0] = addPoint(p0);
    p_pts[1] = addPoint(p1);
    p_pts[2] = addPoint(p2);

    return addTriangle(p_pts[0], p_pts[1], p_pts[2]);    
}

Triangle * TriangleSurface::addTriangle(const Triangle * p_tri)
{
    if (isMyTriangle(p_tri))
        return const_cast<Triangle *>(p_tri);
    else
      return addTriangle(p_tri->_Points[0], p_tri->_Points[1], p_tri->_Points[2]);
}

void TriangleSurface::removeTriangle(Triangle * p_tri)
{
    auto my_tri = _Triangles.find(p_tri);
    if (my_tri == _Triangles.end())
        return;

    _Triangles.erase(my_tri);
}

bool TriangleSurface::isMyTriangle(const Triangle * p_tri) const
{
  auto p_found_tri = _Triangles.find(const_cast<Triangle *>(p_tri));
  if (p_found_tri == _Triangles.end())
    return false;

  return (*p_found_tri == p_tri);
  }


bool TriangleSurface::isMyPoint(const Point * p_point) const
{
    auto p_found_point = _Points.find(const_cast<Point *>(p_point));
    if (p_found_point == _Points.end())
        return false;

    return (*p_found_point == p_point);
}

Point * TriangleSurface::findPoint(const Math::Vector3dDouble & position) const
{
    Point pt(position);
    auto p_found_point = _Points.find(&pt);
    return (p_found_point != _Points.end() ? *p_found_point : nullptr);
}

}
