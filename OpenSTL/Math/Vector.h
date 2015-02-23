#pragma once

#include <cmath>

template <typename T = double, size_t D = 3>
class Vector
{
public:
  T v[D];

  double length();
};


template <typename T, size_t D>
double Vector<T, D>::length()
{
    double s = 0;
    for(size_t i = 0; i<D; ++i)
    {
        s += v[i]*v[i];
    }
    return sqrt(s);
}