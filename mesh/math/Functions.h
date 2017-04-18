#pragma once

#include "GeometryTraits.h"

namespace mesh {
namespace Math {

template <typename A, typename B>
double distance(const A & a, const B & b);

// Basic naive implementation
template<typename A, typename B>
bool intersects(const A a, const B b)
{
    return false;
}


} // namespace Math
} // namespace mesh
