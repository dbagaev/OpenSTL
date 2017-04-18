#pragma once

#include "Data.h"

#include "Attributes.h"
#include "Indexer.h"

#include <math/Vector3d.h>
#include <math/GeometryTraits.h>
#include <math/Functions.h>

namespace mesh { namespace Data {

class __mesh_DATA_EXPORT Point : public AttributeOwner, public IndexedKey<Point>
{
public:
    friend class TriangleSurface;

    Point();
    Point(const Point & other);
    Point(double x, double y, double z);
    Point(const Math::Vector3d<double> & pos);
    ~Point();


    class ComparePosition
    {
    public:
        bool operator()(const Point * p1, const Point * p2) const;

    private:
        inline bool compare(const Math::Vector3dDouble & v1, const Math::Vector3dDouble & v2) const;
    };
    
    const Math::Vector3d<double> & position() const { return _Position; }

private:
    Math::Vector3d<double> _Position;
};

} } // namespace mesh::Data


namespace mesh { namespace Math {

template <>
struct GeometryTraits<mesh::Data::Point>
{
    typedef mesh::Data::Point point_type;
    typedef double value_type;
    typedef Box<value_type, 3> box_type;

    static box_type box(const point_type & p)
    {
        return box_type(p.position(), p.position());
    }
};

template <typename T>
bool intersects(const mesh::Data::Point & a, const Box<T, 3> & b)
{
    const auto p = a.position();
    for (size_t i = 0; i < 3; ++i)
    {
        if (p[i] < b.minPosition(i) || p[i] > b.maxPosition(i))
            return false;
    }
    return true;
}


} }