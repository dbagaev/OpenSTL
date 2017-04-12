#pragma once

#include "Data.h"

#include "Attributes.h"

#include "../Math/Vector3d.h"

namespace mesh {
namespace Data {

class Point;

class __mesh_DATA_EXPORT Triangle : public AttributeOwner
{
    friend class TriangleSurface;

public:
    Triangle(const Point * p_point0, const Point * p_point1, const Point * p_point2);

    Math::Vector3dDouble normal() const;

    Point * getPoint(size_t idx) { return _Points[idx]; };

private:
    Triangle();

    Point * _Points[3];

    Math::Vector3dDouble _Normal;
};

}  // namespace Data
}  // namespace mesh

