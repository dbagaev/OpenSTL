#pragma once

#include "Vector.h"

#include <algorithm>

namespace mesh { namespace Math {

template <typename T, size_t D>
class Box
{
public:
    typedef Vector<T, D> VectorT;

    Box() {};
    Box(const Box & b) : m_Min(b.m_Min), m_Max(b.m_Max) {}
    Box(VectorT v1, VectorT v2) : m_Min(v1), m_Max(v2)
    {
        normalize();
    }

    void normalize()
    {
        for (size_t i = 0; i < D; ++i)
        {
            if (m_Min[i] > m_Max[i])
                std::swap(m_Min[i], m_Max[i]);
        }
    }

    std::pair<Box<T, D>, Box<T, D>> split(size_t dim)
    {
        std::pair<Box<T, D>, Box<T, D>> r(*this, *this);
        const T middle = (m_Min[dim] + m_Max[dim]) / 2;
        r.first.m_Max[dim] = middle;
        r.second.m_Min[dim] = middle;
        return r;
    }

    static size_t numDimensions() { return D; }

    const VectorT & minPosition() const { return m_Min; }
    const VectorT & maxPosition() const { return m_Max; }
    
    T minPosition(size_t i) const { return m_Min[i]; }
    T maxPosition(size_t i) const { return m_Max[i]; }

    const VectorT size() const { return m_Max - m_Min; }
    T size(size_t i) const { return m_Max[i] - m_Min[i]; }

private:
    VectorT m_Min;
    VectorT m_Max;
};

template <typename T, size_t D>
bool operator &&(const Box<T, D> & b1, const Box<T, D> & b2)
{
    for (size_t i = 0; i < D; ++i)
    {
        if (b1.m_Min[i] > b2.m_Max[i] || b1.m_Max[i] < b2.m_Min[i])
            return false;
    }
    return true;
}


template <typename T, size_t D>
Box<T, D> operator &(const Box<T, D> & b1, const Box<T, D> & b2)
{
    static Box<T, D> zero_box;
    Vector<T, D> r_min;
    Vector<T, D> r_max;
    for (size_t i = 0; i < D; ++i)
    {
        r_min[i] = std::max(b1.minCorner[i], b2.minCorner[i]);
        r_max[i] = std::min(b1.maxCorner[i], b2.maxCorner[i]);
        if (r_min[i] > r_max[i])
            return zero_box;
    }
    return Box<T, D>(r_min, r_max);
}

template <typename T, size_t D>
Box<T, D> operator |(const Box<T, D> & b1, const Box<T, D> & b2)
{
    Vector<T, D> r_min;
    Vector<T, D> r_max;
    for (size_t i = 0; i < D; ++i)
    {
        r_min[i] = std::min(b1.minPosition(i), b2.minPosition(i));
        r_max[i] = std::max(b1.maxPosition(i), b2.maxPosition(i));
    }
    return Box<T, D>(r_min, r_max);
}

template <typename T>
double volume(const Box<T, 3> & b)
{
    double v = 1;
    for (size_t i = 0; i < 3; ++i)
        v *= (b.m_Max[i] - b.m_Min[i]);
    return v;
}

} }

