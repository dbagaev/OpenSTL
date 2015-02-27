#pragma once

#include <Math/Vector3d.h>

namespace OpenSTL {
namespace Data {

class Point;

class __OPENSTL_DATA_EXPORT Triangle
{
    friend class Stl;

public:
  Triangle(const Point * p_point0, const Point * p_point1, const Point * p_point2);

    Math::Vector3dDouble normal() const;

private:
    Triangle();

    Point * _Points[3];

    Math::Vector3dDouble _Normal;
};

}  // namespace Data
}  // namespace OpenSTL