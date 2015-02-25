#pragma once

#include "Data.h"

#include <Math/Vector3d.h>

#include "Point.h"

#include <set>
#include <list>

namespace OpenSTL { 
namespace Data {

// class Triangle;

class __declspec(dllexport) Stl
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

    // Triangles
    //Triangle * addTriangle();
private:
    std::set<Point *, Point::ComparePosition> _Points;
    //std::list<Triangle *> _Triangle;

    std::list<Point> _PointsPool;

    bool isMyPoint(const Point * p_point) const;
    Point * findPoint(const Math::Vector3dDouble & position) const;
};

}  // namespace Data
}  // namespace OpenSTL