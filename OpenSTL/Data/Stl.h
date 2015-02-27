#pragma once

#include "Data.h"

#include <Math/Vector3d.h>

#include "Point.h"
#include "Triangle.h"

#include <set>
#include <list>

namespace OpenSTL { 
namespace Data {

// class Triangle;

class __OPENSTL_DATA_EXPORT Stl
{
public:
    Stl();
    Stl(const Stl & stl);
    ~Stl();

    Stl & operator=(const Stl & other);

    // Points collection
    Point * addPoint(const Point * p_point);
    Point * addPoint(const Math::Vector3dDouble & position);
    Point * addPoint(double x, double y, double z);
    void removePoint(Point * p_pount);

    Triangle * addTriangle(const Point * p_p0, const Point * p_p1, const Point * p_p2);    
    Triangle * addTriangle(const Math::Vector3dDouble & p0, const Math::Vector3dDouble & p1, const Math::Vector3dDouble & p2);
    Triangle * addTriangle(const Triangle * p_tri);
    void removeTriangle(Triangle * p_tri);

    // Triangles
    //Triangle * addTriangle();
private:
    std::set<Point *, Point::ComparePosition> _Points;
    std::set<Triangle *> _Triangles;
    
    std::list<Triangle> _TrianglesPool;
    bool isMyTriangle(const Triangle * p_tri) const;

    std::list<Point> _PointsPool;
    bool isMyPoint(const Point * p_point) const;
    Point * findPoint(const Math::Vector3dDouble & position) const;
};

}  // namespace Data
}  // namespace OpenSTL