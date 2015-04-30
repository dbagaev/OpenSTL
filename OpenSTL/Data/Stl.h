#pragma once

#include "Data.h"

#include "../Math/Vector3d.h"

#include "Point.h"
#include "Triangle.h"

#include "Attributes.h"

#include <set>
#include <list>
#include <memory>

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

    size_t getNumberPoints() const { return _Points.size(); }
    size_t getNumberTriangles() const { return _Triangles.size(); }

    // Triangles
    //Triangle * addTriangle();

    template <typename DataType>
    AttributesImpl<DataType, Point> * createPointAttributes() { return new AttributesImpl<DataType, Point>(_PointsAttributesAllocMap); }
    template <typename DataType>
    AttributesImpl<DataType, Triangle> * createTriangleAttributes() { return new AttributesImpl<DataType, Point>(_TrianglesAttributesAllocMap); }

private:
    std::set<Point *, Point::ComparePosition> _Points;
    std::set<Triangle *> _Triangles;
    
    std::list<Triangle> _TrianglesPool;
    bool isMyTriangle(const Triangle * p_tri) const;

    std::list<Point> _PointsPool;
    bool isMyPoint(const Point * p_point) const;
    Point * findPoint(const Math::Vector3dDouble & position) const;

    std::unique_ptr<AttributesAllocMap<Point>> _PointsAttributesAllocMap;
    std::unique_ptr<AttributesAllocMap<Triangle>> _TrianglesAttributesAllocMap;
};

}  // namespace Data
}  // namespace OpenSTL
