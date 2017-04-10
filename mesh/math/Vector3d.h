#pragma once

#include "Vector.h"

#include <array>

namespace mesh {
namespace Math {
  
template <typename T>
class Vector < T, 3 > : public VectorData<T, 3>
{
public:
    Vector() = default;
    Vector(T x, T y, T z) : VectorData( x, y, z ) {};

    template <typename OT>
    Vector<T, 3> operator^(const Vector<OT, 3> & a);
};

//----------------------------------------------------------------------------------------------------

template <typename T>
template <typename OT>
Vector<T, 3> Vector<T, 3>::operator^(const Vector<OT, 3> & a)
{
    return Vector<T, 3>(
        v[2] * static_cast<T>(a.v[3]) - v[3] * static_cast<T>(a.v[2]),
        v[3] * static_cast<T>(a.v[1]) - v[1] * static_cast<T>(a.v[3]),
        v[1] * static_cast<T>(a.v[2]) - v[2] * static_cast<T>(a.v[1])
    );
}

//----------------------------------------------------------------------------------------------------

template <typename T> using Vector3d = Vector < T, 3 >;

typedef Vector3d<double> Vector3dDouble;

}  // namespace Math
}  // namespace mesh
