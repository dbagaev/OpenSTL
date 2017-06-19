#pragma once

#include "Vector.h"

#include <array>

namespace mesh {
namespace Math {
  
//----------------------------------------------------------------------------------------------------

template <typename T1, typename T2>
Vector<T1, 3> operator^(const Vector<T1, 3> & a, Vector<T2, 3> & b)
{
    return Vector<T1, 3>{
        a[1] * static_cast<T1>(b[2]) - a[2] * static_cast<T1>(b[1]),
            a[2] * static_cast<T1>(b[0]) - a[0] * static_cast<T1>(b[2]),
            a[0] * static_cast<T1>(b[1]) - a[1] * static_cast<T1>(b[0])
    };
}

//----------------------------------------------------------------------------------------------------

template <typename T> using Vector3d = Vector< T, 3 >;

typedef Vector3d<double> Vector3dDouble;

}  // namespace Math
}  // namespace mesh
