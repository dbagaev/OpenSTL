#pragma once

#include "Data.h"

#include "../Math/Vector3d.h"

namespace OpenSTL {

namespace Data {

class __OPENSTL_DATA_EXPORT Point
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
    

private:
    Math::Vector3d<double> _Position;
};

}  // namespace Data
}  // namespace OpenSTL