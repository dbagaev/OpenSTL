#pragma once

#include <stdexcept>
#include <vector>

namespace OpenSTL {

namespace Data {

// This class holds attributes and allows easy access to attribute data
class AttributeOwner
{
    template <typename TraitsType>
    friend class Attribute;

    template <typename Key>
    friend class AttributesAllocMap;

    template <typename DataType>
    DataType getData(const size_t i_offset) const
    {
        static const size_t size_of_data = static_cast<size_t>(sizeof(DataType));
        const auto ptr = const_cast<AttributeOwner *>(this)->getPtr(i_offset, size_of_data);
        if (ptr != nullptr)
          return * reinterpret_cast<DataType *>(ptr);
        else
          return DataType();
    }

    template <typename DataType>
    void setData(size_t i_offset, DataType i_value)
    {
        static const size_t size_of_data = static_cast<size_t>(sizeof(DataType));
        auto ptr = getPtr(i_offset, size_of_data);
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

        *reinterpret_cast<DataType *>(ptr) = i_value;
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
        for (size_t i = st_p; i < st_p + i_size_bytes; ++i)
            _FreeFlags[i] = true;

        return st_p;
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

/**
 * Attribute traits describes types of attribute
 */

template <typename _KeyType, typename _DataType, typename _StorageType = _DataType>
class AttributeTraits
{
public:
	typedef _KeyType        KeyType;
	typedef _DataType       DataType;
	typedef _StorageType    StorageType;

	static DataType makeData(StorageType storage)
	{
		return static_cast<DataType>(storage);
	}
	static StorageType makeStorage(DataType data)
	{
		return static_cast<StorageType>(data);
	}

	static StorageType defaultValue()
	{
	    return 0;
	}
};

// This class is attribute implementation
template <typename TraitsType>
class Attribute
{
public:
	typedef typename TraitsType::KeyType KeyType;
	typedef typename TraitsType::DataType DataType;

    Attribute(AttributesAllocMap<KeyType> * i_mgr) : _AllocMap(*i_mgr)
    {
        _Offset = _AllocMap.allocate(dataSize());
    }

    ~Attribute()
    {
        _AllocMap.free(_Offset, dataSize());
    }

    DataType getValue(const KeyType * p_key)
    {
        return TraitsType::makeData(getStorageValue(p_key));
    }

    void setValue(KeyType * p_key, DataType value)
    {
        setStorageValue(p_key, TraitsType::makeStorage(value));
    }

    void setDefaultValue(KeyType * p_key)
    {
        setStorageValue(p_key, TraitsType::defaultValue());
    }

    static size_t dataSize()
    {
        return sizeof(typename TraitsType::StorageType);
    }

protected:
    Attribute(size_t i_offset) : _Offset(i_offset) {}

    typedef typename TraitsType::StorageType StorageType;

    void setStorageValue(KeyType * p_key, StorageType value)
    {
        static_cast<AttributeOwner *>(p_key)->setData(_Offset, value);
    }

    StorageType getStorageValue(const KeyType * p_key)
    {
        return static_cast<const AttributeOwner *>(p_key)->getData<StorageType>(_Offset);
    }


private:
    size_t _Offset;
    AttributesAllocMap<KeyType> & _AllocMap;
};

}  // namespace Data

}  // namespace OpenSTL
