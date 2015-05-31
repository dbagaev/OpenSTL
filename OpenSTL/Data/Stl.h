#pragma once

#include "Data.h"

#include "../Math/Vector3d.h"

#include "Point.h"
#include "Triangle.h"

#include "Attributes.h"

#include <boost/iterator/iterator_facade.hpp>

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

    class PointIterator : public boost::iterator_facade < PointIterator, Point, boost::forward_traversal_tag >
    {
    friend class Stl;

    private:
        typedef std::set<Point *, Point::ComparePosition>::iterator InternalIterator;

        InternalIterator _Iter;

        PointIterator(InternalIterator it) : _Iter(it) {}

        void increment() { ++_Iter; }

        bool equal(const PointIterator & other) const
          {
          return _Iter == other._Iter;
          }

        Point & dereference() const { return **_Iter; }
    };

    PointIterator beginPoint() { return PointIterator(_Points.begin()); }
    PointIterator endPoint() { return PointIterator(_Points.end()); }

    // Triangles
    //Triangle * addTriangle();

    template <typename AttributeType>
    AttributeType * createPointAttributes() { return new AttributeType(&_PointsAttributesAllocMap, this); }
    template <typename AttributeType>
    AttributeType * createTriangleAttributes() { return new AttributeType(&_TrianglesAttributesAllocMap, this); }

private:
    std::set<Point *, Point::ComparePosition> _Points;
    std::set<Triangle *> _Triangles;
    
    std::list<Triangle> _TrianglesPool;
    bool isMyTriangle(const Triangle * p_tri) const;

    std::list<Point> _PointsPool;
    bool isMyPoint(const Point * p_point) const;
    Point * findPoint(const Math::Vector3dDouble & position) const;

    AttributesAllocMap<Point> _PointsAttributesAllocMap;
    AttributesAllocMap<Triangle> _TrianglesAttributesAllocMap;
};

class TransparentStlAttribute
{
public:
    TransparentStlAttribute(Stl *) {};
};

template <typename OwnerType, typename DataType>
class FlagAttributeTraits : public AttributeTraits<OwnerType, DataType>
{

};

typedef Attribute<FlagAttributeTraits<Point, long>, TransparentStlAttribute> PointFlagAttribute;
typedef Attribute<FlagAttributeTraits<Triangle, long>, TransparentStlAttribute> TriangleFlagAttribute;

}  // namespace Data
}  // namespace OpenSTL
