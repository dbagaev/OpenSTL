#pragma once

#include "Data.h"

#include "../Math/Vector3d.h"

#include "Point.h"
#include "Triangle.h"

#include "Attributes.h"
#include "PointNeighbourTriangles.h"

#include <boost/iterator/iterator_facade.hpp>

#include <set>
#include <list>
#include <memory>
#include <algorithm>

namespace mesh { 
namespace Data {

// class Triangle;

class PointNeighbourTriangles;

class __mesh_DATA_EXPORT TriangleSurface
{
public:
    TriangleSurface();
    TriangleSurface(const TriangleSurface & stl);
    ~TriangleSurface();

    TriangleSurface & operator=(const TriangleSurface & other);

    // Points collection
    Point * addPoint(const Point * p_point);
    Point * addPoint(const Math::Vector3dDouble & position);
    Point * addPoint(double x, double y, double z);
    void removePoint(Point * p_pount);

    Triangle * addTriangle(const Point * p_p0, const Point * p_p1, const Point * p_p2);    
    Triangle * addTriangle(const Math::Vector3dDouble & p0, const Math::Vector3dDouble & p1, const Math::Vector3dDouble & p2);
    Triangle * addTriangle(const Triangle * p_tri);
    void removeTriangle(Triangle * p_tri);

    void replacePointByPoint(Point * oldPoint, Point * newPoint);

    size_t getNumberPoints() const { return _Points.size(); }
    size_t getNumberTriangles() const { return _Triangles.size(); }

    const PointNeighbourTriangles & pointNeighbours() {
        return m_pointNeighbours;
    }

    class PointIterator : public boost::iterator_facade < PointIterator, Point, boost::forward_traversal_tag >
    {
    friend class TriangleSurface;

    public:
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

    class TriangleIterator : public boost::iterator_facade < TriangleIterator, Triangle, boost::forward_traversal_tag >
    {
    friend class TriangleSurface;

    public:
        typedef std::set<Triangle *>::iterator InternalIterator;

        InternalIterator _Iter;

        TriangleIterator(InternalIterator it) : _Iter(it) {}

        void increment() { ++_Iter; }

        bool equal(const TriangleIterator & other) const
          {
          return _Iter == other._Iter;
          }

        Triangle & dereference() const { return **_Iter; }
    };

    TriangleIterator beginTriangle() { return TriangleIterator(_Triangles.begin()); }
    TriangleIterator endTriangle() { return TriangleIterator(_Triangles.end()); }


    // Triangles
    //Triangle * addTriangle();

    template <typename AttributeType>
    AttributeType * createPointAttributes()
    {
        auto p_attribute = new AttributeType(&_PointsAttributesAllocMap, this);
        std::for_each(beginPoint(), endPoint(), [p_attribute](Point & p) { p_attribute->setDefaultValue(&p); });
        return p_attribute;
    }
    template <typename AttributeType>
    AttributeType * createTriangleAttributes()
    {
        auto p_attribute = new AttributeType(&_TrianglesAttributesAllocMap, this);
        std::for_each(beginTriangle(), endTriangle(), [p_attribute](Triangle & p) { p_attribute->setDefaultValue(&p); });
        return p_attribute;
    }

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

    Indexer<Point> m_PointIndexer;

    PointNeighbourTriangles m_pointNeighbours;
};

template <typename OwnerType, typename DataType>
class FlagAttributeTraits : public AttributeTraits<OwnerType, DataType>
{

};

template <typename OwnerType, typename DataType>
class FlagAttribute: public Attribute<FlagAttributeTraits<OwnerType, DataType>>
{
public:
    FlagAttribute(AttributesAllocMap<OwnerType> * map, TriangleSurface *) : Attribute<FlagAttributeTraits<OwnerType, DataType>>(map) {};
};

typedef FlagAttribute<Point, long> PointFlagAttribute;
typedef FlagAttribute<Triangle, long> TriangleFlagAttribute;

}  // namespace Data
}  // namespace mesh
