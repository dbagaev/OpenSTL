#pragma once

#include <stdexcept>
#include <vector>

namespace OpenSTL {

namespace Data {

// This class holds attributes and allows easy access to attribute data
class AttributeOwner
{
    friend class StlAttributes;

    template <typename DataType>
    DataType getData(size_t i_offset) const
    {
        static auto ptr = getPtr(i_offset, sizeof(DataType));
        if (ptr != nullptr)
          return *static_cast<DataType *>();
        else
          return DataType();
    }

    template <typename DataType>
    void setData(size_t i_offset, DataType i_value)
    {
        static auto ptr = getPtr(i_offset, sizeof(DataType));
        if (ptr == nullptr)
        {
            // Allocate one more block
            AttributeBlock * p_new_block = new AttributeBlock;
            AttributeBlock * p_last_block = &_First;
            for (; p_last_block->_Next != nullptr; p_last_block = p_last_block->_Next) 
                ;
            p_last_block->_Next = p_last_block;

            ptr = getPtr(i_offset, sizeof(DataType));

            if (ptr == nullptr)
                return;
          }

        *static_cast<DataType *>(ptr) = i_value;
    }

    char * getPtr(size_t i_offset, size_t i_desired_size = 0)
    {
        size_t t = i_offset / BlockSize;
        size_t o = i_offset % BlockSize;

        if (BlockSize - o < i_desired_size)
          throw std::range_error("Desired data position goes outside data block");

        AttributeBlock * p_block = &_First;
        for (size_t i = 0; i < t && p_block != nullptr; ++i)
            p_block = p_block->_Next;

        if (p_block == nullptr)
          return nullptr;

        return (p_block->_Data + o);
    }

    static const size_t BlockSize = 64;

    // Basic structure to store attributes
    struct AttributeBlock
    {
        AttributeBlock * _Next;
        char _Data[BlockSize];
    };

    AttributeBlock _First;
};

template <typename Key>
class AttributesAllocMap
{
public:
    AttributesAllocMap()
    {
        _FreeFlags.resize(AttributeOwner::BlockSize, false);
    }
    //virtual addAttributes(StlAttributes * p_attributes);
    //virtual removeAttributes(StlAttributes * p_attributes);

    size_t allocate(size_t i_size_bytes)
    {
        // Find any free block with desired size in
        const size_t sz = _FreeFlags.size();
        size_t st_p = 0;
        size_t blk = 0;
        bool cnt = false;
        for (size_t i = 0; i < sz; ++i)
        { 
            if (_FreeFlags[i] == 0)
            {
                if (!cnt)
                {
                    st_p = i;
                    blk = i / AttributeOwner::BlockSize;
                    cnt = true;
                }

                if (cnt)
                {
                    size_t n_blk = i / AttributeOwner::BlockSize;
                    if (blk != n_blk)
                    { 
                        // Value can't be split between two blocks. If so - start count from scratch
                        st_p = i;
                        blk = n_blk;
                    }
                    else if (i - st_p + 1 >= i_size_bytes)
                        break;
                }            
            }
            else if (cnt)
                cnt = false;
        }

        // Add more bits
        if (!cnt)
        { 
            _FreeFlags.resize(sz + AttributeOwner::BlockSize, false);
            st_p = sz;
        }

        // Mark bits as filled
        for (size_t i = sp_t; i < sp_t + i_size_bytes; ++i)
            _FreeFlags[i] = true;

        return sp_t;
    }

    void free(size_t i_offset, size_t i_size_bytes)
    {
        // Mark bits as free
        for (size_t i = i_offset; i < i_offset + i_size_bytes; ++i)
            _FreeFlags[i] = false;

        // std::vector should carry about range overflow
    }

protected:

private:
    std::vector<bool> _FreeFlags;
};


// This class is concrete
template <typename DataType, typename Key>
class AttributesImpl
{
public:
    AttributesImpl(AttributesAllocMap * i_mgr) : _AllocMap(i_mgr)
    {
        _Offset = _AllocMap->allocate(dataSize());
    }

    ~AttributesImpl()
    {
        _AllocMap->free(_Offset, dataSize());
    }

    DataType getValue(const Key * p_key)
    {
        static_cast<AttributeOwner *>(p_key)->getData(_Offset);
    }

    void setValue(const Key * p_key, DataType value)
    {
        static_cast<AttributeOwner *>(p_key)->setData(_Offset, i_value);
    }

    static size_t dataSize()
    {
        return sizeof(DataSize);
    }

protected:
    AttributesImpl(size_t i_offset) : _Offset(i_offset) {}

private:
    size_t _Offset;
    AttributesAllocMap _AllocMap;
};

}  // namespace Data

}  // namespace OpenSTL