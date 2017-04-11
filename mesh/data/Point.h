#pragma once

#include "Data.h"

#include "Attributes.h"
#include "Indexer.h"

#include <math/Vector3d.h>

namespace mesh {

namespace Data {

class __mesh_DATA_EXPORT Point : public AttributeOwner, public IndexedKey<Point>
{
public:
    friend class Stl;

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

}  // namespace Data
}  // namespace mesh
