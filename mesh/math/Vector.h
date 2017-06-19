#pragma once

#include <cmath>
#include <array>

namespace mesh {

namespace Math {

template <typename T, size_t D>
struct VectorData
{
    VectorData() = default;

    VectorData(std::initializer_list<T> l)
    {
        if (l.size() != D)
            throw std::invalid_argument("Invalid initializer list passed to Vector");

        auto p_v = v;
        for (auto x : l)
        {
            *p_v = x;
            ++p_v;
        }
    }

	T v[D];

	inline T operator[](size_t i) const { return v[i]; };
	inline T & operator[](size_t i) { return v[i]; };
};

template <typename T>
struct VectorData<T, 2>
{
    VectorData() : x(0), y(0) {};
	VectorData(T x_, T y_) : x(x_), y(y_) {}

    VectorData(std::initializer_list<T> l)
    {
        if (l.size() != 2)
            throw std::invalid_argument("Invalid initializer list passed to Vector");

        auto p_v = v;
        for (auto x : l)
        {
            *p_v = x;
            ++p_v;
        }
    }

	union {
		T v[2];
		struct {
			T x;
			T y;
		};
	};

	T operator[](size_t i) const { return v[i]; };
	inline T & operator[](size_t i) { return v[i]; };
};

template <typename T>
struct VectorData<T, 3>
{
	VectorData() : x(0), y(0), z(0) {};
	VectorData(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    VectorData(std::initializer_list<T> l)
    {
        if (l.size() != 3)
            throw std::invalid_argument("Invalid initializer list passed to Vector");

        auto p_v = v;
        for (auto x : l)
        {
            *p_v = x;
            ++p_v;
        }
    }

	union {
		T v[3];
		struct {
			T x;
			T y;
			T z;
		};
	};

	T operator[](size_t i) const { return v[i]; };
	inline T & operator[](size_t i) { return v[i]; };
};


template <typename T1, typename T2, size_t D>
bool operator==(const VectorData<T1, D> & a, const VectorData<T2, D> & b)
{
    for (size_t i = 0; i < D; ++i)
        if (a[i] != static_cast<T1>(b[i]) || static_cast<T2>(a[i]) != b[i])
            return false;
    return true;
}

template <typename T, size_t D>
bool operator==(const VectorData<T, D> & a, const VectorData<T, D> & b)
{
    for (size_t i = 0; i < D; ++i)
        if (a[i] != b[i])
            return false;
    return true;
}


template <typename T, size_t D>
class Vector : public VectorData<T, D>
{
public:
    Vector() = default;
    Vector(std::initializer_list<T> x) : VectorData(x) {}

    double length() const;
    double length2() const;

    Vector<T, D> norm() const;
};

//----------------------------------------------------------------------------------------------------

template <typename T1, typename T2, size_t D>
Vector<T1, D> operator+(const Vector<T1, D> & a, const Vector<T2, D> & b)
{
    Vector<T1, D> res;
    for (size_t i = 0; i < D; ++i)
      res[i] = a[i] + static_cast<T1>(b[i]);
    return res;
}

template <typename T1, typename T2, size_t D>
Vector<T1, D> operator-(const Vector<T1, D> & a, const Vector<T2, D> & b)
{
    Vector<T1, D> res;
    for (size_t i = 0; i < D; ++i)
      res[i] = a[i] - static_cast<T1>(b[i]);
    return res;
}

template <typename T1, typename T2, size_t D>
T1 operator*(const Vector<T1, D> & a, const Vector<T2, D> & b)
{
    T1 s = 0;
    for (size_t i = 0; i < D; ++i)
      s += a[i] * static_cast<T1>(b[i]);
    return s;
}

template <typename T1, typename T2, size_t D>
Vector<T1, D> operator/(const Vector<T1, D> & a, const T2 b)
{
    Vector<T1, D> res(a);
    for (size_t i = 0; i < D; ++i)
      res[i] /= static_cast<T1>(b);
    return res;
}

//----------------------------------------------------------------------------------------------------

template <typename T, size_t D>
double Vector<T, D>::length() const
{
    double s = 0;
    for(size_t i = 0; i<D; ++i)
    {
        s += v[i]*v[i];
    }
    return sqrt(s);
}

template <typename T, size_t D>
double Vector<T, D>::length2() const
{
    double s = 0;
    for (size_t i = 0; i < D; ++i)
    {
        s += v[i] * v[i];
    }
}

template <typename T, size_t D>
Vector<T, D> Vector<T, D>::norm() const
{
    const auto l = length();
    return (*this / l);
}

//----------------------------------------------------------------------------------------------------

//template <typename T1, typename T2, size_t D>
//T1 dot(const Vector<T1, D> & a, const <T2, D> & b)
//{
//    T1 r = 0;
//    for (size_t i = 0; i < D; ++i)
//        r += a[i] * const_cast<T2>(b[i]);
//    return r;
//}

template <typename T, typename OT, size_t D>
double distance(const Vector<T, D> & a, const Vector<OT, D> & b)
{
    double s = 0;
    for (size_t i = 0; i < D; ++i)
    {
        const T d = a.v[i] - static_cast<T>(b.v[i]);
        s += d * d;
    }
    return sqrt(s);
}

}  // namespace Math
}  // namespace mesh

#include <cmath>

namespace std {

template <typename T, size_t D>
mesh::Math::Vector<T, D> abs(const mesh::Math::Vector<T, D> & v)
{
    mesh::Math::Vector<T, D> r;
    for (size_t i = 0; i < D; ++i)
        r[i] = std::abs(v[i]);
    return r;
}

}
