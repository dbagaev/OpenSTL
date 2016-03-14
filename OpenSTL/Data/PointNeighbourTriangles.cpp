#include "PointNeighbourTriangles.h"

#include <memory>

using namespace OpenSTL::Data;

namespace OpenSTL
{

namespace Data {

namespace Details {

template <typename NeighbourType>
class NeighbourManager : public NeighbourAccessor<NeighbourType>
{
public:
    NeighbourManager(NeighbourType ** p_data) : NeighbourAccessor<NeighbourType>(p_data) {}

    void addNeighbour(NeighbourType * _item)
    {
        if(this->size() == allocatedSize())
            reallocate(allocatedSize() == 0 ? 4 : allocatedSize() * 2);

        this->_Data[this->size() + 2] = _item;
        this->_Data[1] = reinterpret_cast<NeighbourType *>(reinterpret_cast<size_t>(this->_Data[1]) + 1);
    }

    void clear()
    {
        if(this->_Data != nullptr)
            this->_Data[1] = reinterpret_cast<NeighbourType *>(0);
    }

    NeighbourType ** getData() { return this->_Data; }

protected:
    size_t allocatedSize() const
    {
        if(this->_Data == nullptr)
            return 0;
        else
            return reinterpret_cast<size_t>(this->_Data[0]);
    }

    void reallocate(size_t n)
    {
        if(n == 0)
        {
            delete [] this->_Data;
            this->_Data = nullptr;
            return;
        }

        std::unique_ptr<NeighbourType * []> p_new_data ( new NeighbourType *[n + 2] );

        p_new_data[0] = reinterpret_cast<NeighbourType *>(n);
        if(this->_Data != nullptr)
        {
            std::copy(p_new_data.get() + 1, p_new_data.get() + this->size() + 2, this->_Data);
        }
        else
        {
            p_new_data[1] = reinterpret_cast<NeighbourType *>(this->size());
        }

        auto _p_new_data = p_new_data.release();
        p_new_data.reset(this->_Data);
        this->_Data = _p_new_data;

        this->_Data[0] = reinterpret_cast<NeighbourType *>(n);
    }

};

}


}

void PointNeighbourTriangles::update()
{
    std::for_each(_Stl->beginPoint(), _Stl->endPoint(), [this](Point & point) {
        auto p_data = getStorageValue(&point);
        Details::NeighbourManager<Triangle> nm(p_data);
        nm.clear();
    });

    std::for_each(_Stl->beginTriangle(), _Stl->endTriangle(), [this](Triangle & tri) {
        for(size_t i = 0; i<3; ++i)
        {
            auto pt = tri.getPoint(i);

            auto p_data = getStorageValue(pt);
            Details::NeighbourManager<Triangle> nm(p_data);
            nm.addNeighbour(&tri);

            if(p_data != nm.getData())
                setStorageValue(pt, nm.getData());
        }
    });
}


}



