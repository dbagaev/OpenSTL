#pragma once

#ifndef OPENSTL_DATA_POINTNEIGHBOURTRIANGLES_H_
#define OPENSTL_DATA_POINTNEIGHBOURTRIANGLES_H_

#include "Attributes.h"
#include "Stl.h"
#include "Point.h"
#include "Triangle.h"
#include "PointNeighbours.h"

#include <memory>
#include <algorithm>

namespace OpenSTL {

namespace Data {

namespace Details {

template <typename NeighbourType>
class NeighbourAccessor
{
    template <typename NT>
    friend class PointNeighbourAttributeTraits;

public:
    NeighbourAccessor(NeighbourType ** _data) : _Data(_data)
    {

    }

    size_t size() const
    {
        if(_Data == nullptr)
            return 0;
        else
            return reinterpret_cast<size_t>(_Data[1]);
    }

    bool empty() const
    {
        return (size() == 0);
    }

    NeighbourType * at(size_t idx)
    {
        return _Data[idx + 2];
    }

    NeighbourType * operator[](size_t idx)
    {
        return _Data[idx + 2];
    }

protected:
    NeighbourType ** _Data;
};

}

template <typename NeighbourType>
class PointNeighbourAttributeTraits
{
    public:
        typedef Data::Point KeyType;
        typedef Details::NeighbourAccessor<NeighbourType> DataType;
        typedef NeighbourType ** StorageType;

        static DataType makeData(StorageType storage)
        {
            return DataType(storage);
        }

        static StorageType makeStorage(DataType data)
        {
            return data._Data;
        }

        static StorageType defaultValue()
        {
            return nullptr;
        }

};

class PointNeighbourTriangles : public PointNeighbours< Triangle >, public Attribute<PointNeighbourAttributeTraits<Data::Triangle>>
{
public:
	PointNeighbourTriangles(AttributesAllocMap<Point> * p_map, Stl * p_stl) :
	    PointNeighbours<Triangle>(p_stl),
	    Attribute<PointNeighbourAttributeTraits<Data::Triangle>>(p_map)
	{}

    void update();

    class NeighbourData
    {
    	//
    };
};

}  // namespace Data

}  // namespace OpenSTL


#endif /* OPENSTL_DATA_POINTNEIGHBOURTRIANGLES_H_ */
