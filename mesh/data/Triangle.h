#pragma once

#include "Data.h"
#include "Point.h"
#include "Attributes.h"

#include <math/Vector3d.h>
#include <math/GeometryTraits.h>
#include <math/Functions.h>

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
    const Point * getPoint(size_t idx) const { return _Points[idx]; };

protected:
    void setPoint(size_t idx, Point * p_point) { _Points[idx] = p_point; };

private:
    Triangle();

    Point * _Points[3];

    Math::Vector3dDouble _Normal;
};

}  // namespace Data

namespace Math {

template <>
struct GeometryTraits<mesh::Data::Triangle>
{
    typedef mesh::Data::Point point_type;
    typedef mesh::Data::Triangle triangle_type;
    typedef double value_type;
    typedef Box<value_type, 3> box_type;

    static box_type box(const triangle_type & t)
    {
        return box_type(t.getPoint(0)->position()) | 
            t.getPoint(1)->position() | 
            t.getPoint(2)->position();
    }
};

template <typename T>
bool intersects(const mesh::Data::Triangle & t, const Box<T, 3> & b)
{
    // Algoruthm description can be found here
    // http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/tribox_tam.pdf
    // 
    // TODO : Optimize implementation
    //
    // TODO : Implement for different dimentions
    
    // e1..3 separable?
    auto t_b = GeometryTraits<mesh::Data::Triangle>::box(t);
    if (!intersects(t_b, b))
        return false;

    auto c = b.center();
    auto h = b.size() / 2;

    const size_t D = 3;

    // triangle normal separable?
    {
        const auto n = t.normal();
        const auto a = t.getPoint(0)->position() - c;

        double r = std::abs(n) * h;
        double p = n * a;
        if (p < -r || p > r)
            return false;
    }

    // Additional 9 separability tests
    {
        const std::array< Vector<T, 3>, 3> e{
            Vector<T, 3>{1., 0., 0.},
            Vector<T, 3>{0., 1., 0.},
            Vector<T, 3>{0., 0., 1.} };
        std::array< Vector<T, 3>, 3> ac;
        for (size_t i = 0; i < 3; ++i)
        {
            ac[i] = t.getPoint(i)->position() - c;
        }
        for (size_t i = 0; i < 3; ++i)
        {
            auto f = ac[(i + 1) % 3] - ac[i];
            for (size_t j = 0; j < D; ++j)
            {
                auto a = (e[j] ^ f).norm();
                double p[3];
                for (size_t k = 0; k < 3; ++k)
                    p[k] = ac[k] * a;
                double r = std::abs(a) * h;

                if (std::min(p[0], std::min(p[1], p[2])) > r ||
                    std::max(p[0], std::max(p[1], p[2])) < -r)
                    return false;
            }
        }
    }

    return true;
}

}  // namespace Math

}  // namespace mesh



