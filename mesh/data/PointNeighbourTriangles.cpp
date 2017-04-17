#include "PointNeighbourTriangles.h"

#include "TriangleSurface.h"

#include <memory>

using namespace mesh::Data;

namespace mesh { namespace Data {

//void PointNeighbourTriangles::update()
//{
//    std::for_each(_Stl->beginPoint(), _Stl->endPoint(), [this](auto & point)
//    {
//        auto & n = get(point);
//        n.clear();
//    });
//
//    std::for_each(_Stl->beginTriangle(), _Stl->endTriangle(), [this](Triangle & tri) 
//    {
//        for(size_t i = 0; i<3; ++i)
//        {
//            auto pt = tri.getPoint(i);
//
//            auto & p_data = get(*pt);
//            p_data.insert(&tri);
//        }
//    });
//}


} }



