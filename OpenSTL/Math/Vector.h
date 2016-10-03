#pragma once

#include <cmath>
#include <array>

namespace OpenSTL {

namespace Math {

template <typename T, size_t D>
struct VectorData
{
	T v[D];

	inline T operator[](size_t i) const { return v[i]; };
	inline T & operator[](size_t i) { return v[i]; };
};

template <typename T>
struct VectorData<T, 2>
{
	VectorData() = default;
	VectorData(T x_, T y_) : x(x_), y(y_) {}

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
	VectorData() = default;
	VectorData(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

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

template <typename T, size_t D>
class Vector : public VectorData<T, D>
{
public:
    template <typename OT>
    Vector<T, D> operator+(const Vector<OT, D> & a);
    template <typename OT>
    Vector<T, D> operator-(const Vector<OT, D> & a);
    template <typename OT>
    T operator*(const Vector<OT, D> & a);
    template <typename OT>
    Vector<T, D> operator/(const OT a);

    template <typename OT>
    bool operator==(const Vector<OT, D> & a);
    bool operator==(const Vector<T, D> & a);

    double length();
    double length2();
};

//----------------------------------------------------------------------------------------------------

template <typename T, size_t D>
template <typename OT>
Vector<T, D> Vector<T, D>::operator+(const Vector<OT, D> & a)
{
    Vector<T, D> res;
    for (size_t i = 0; i < D; ++i)
      res[i] = v[i] + static_cast<T>(a.v[i]);
    return res;
}

template <typename T, size_t D>
template <typename OT>
Vector<T, D> Vector<T, D>::operator-(const Vector<OT, D> & a)
{
    Vector<T, D> res;
    for (size_t i = 0; i < D; ++i)
      res[i] = v[i] - static_cast<T>(a.v[i]);
    return res;
}

template <typename T, size_t D>
template <typename OT>
T Vector<T, D>::operator*(const Vector<OT, D> & a)
{
    T s;
    for (size_t i = 0; i < D; ++i)
      s += v[i] * static_cast<T>(a.v[i]);
    return s;
}

template <typename T, size_t D>
template <typename OT>
Vector<T, D> Vector<T, D>::operator/(const OT a)
{
    Vector<T, D> res(this);
    for (size_t i = 0; i < D; ++i)
      res[i] /= static_cast<T>(a);
    return res;
}

template <typename T, size_t D>
template <typename OT>
bool Vector<T, D>::operator==(const Vector<OT, D> & a)
{
    Vector<T, D> res(this);
    for (size_t i = 0; i < D; ++i)
        if (v[i] != static_cast<T>(a.v[i]) || static_cast<OT>(v[i]) != a.v[i])
          return false;
    return true;
}

template <typename T, size_t D>
bool Vector<T, D>::operator==(const Vector<T, D> & a)
  {
  Vector<T, D> res(this);
  for (size_t i = 0; i < D; ++i)
    if (v[i] != a.v[i])
      return false;
  return true;
  }

//----------------------------------------------------------------------------------------------------

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

template <typename T, size_t D>
double Vector<T, D>::length2()
{
    double s = 0;
    for (size_t i = 0; i < D; ++i)
    {
        s += v[i] * v[i];
    }
}

//----------------------------------------------------------------------------------------------------

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
}  // namespace OpenSTL