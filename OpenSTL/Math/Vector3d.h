#pragma once

#include "Vector.h"

#include <array>

namespace OpenSTL {
namespace Math {
  
template <typename T>
class Vector < T, 3 >
{
public:
    Vector() : v({ 0, 0, 0 }) {};
    Vector(T x, T y, T z) : v({ x, y, z }) {};

    template <typename OT>
    Vector<T, 3> operator^(const Vector<OT, 3> & a);

    inline T operator[](size_t i) const { return v[i]; };

private:
    std::array<double, 3> v;
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
}  // namespace OpenSTL