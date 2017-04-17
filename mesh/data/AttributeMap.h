#pragma once

#include <vector>

#include "Indexer.h"

namespace mesh { namespace Data {


template <typename ContainerType, typename ValueType = typename ContainerType::value_type>
class ContainerWrapper
{
public:
    void ensureSize(size_t size) {};
    ValueType & get(size_t index) { return m_Data[index]; }
    void set(size_t index, const ValueType & value) { m_Data[index] = value; }
private:
    ContainerType m_Data;
};


template <typename KeyType>
class ContainerWrapper<std::vector<KeyType>, KeyType>
{
public:
    void ensureSize(size_t size)
    {
        if(m_Data.size() <= size)
            m_Data.resize(size + 1);
    }

    KeyType & get(size_t index)
    {
        return m_Data[index];
    }

    const KeyType & get(size_t index) const
    {
        return m_Data[index];
    }

    void set(size_t index, const KeyType & value)
    {
        m_Data[index] = value;
    }

private:
    std::vector<KeyType> m_Data;
};


template <
    typename KeyType, 
    typename ValueType, 
    typename KeyIndexer = DefaultKeyIndexer<KeyType>, 
    typename ValueContainer = std::vector<ValueType> >
class AttributeMap
{
public:
    typedef size_t Index;

    AttributeMap() {};
    ~AttributeMap() {};

    ValueType &get(const KeyType & key)
    {
        size_t index = KeyIndexer::getIndex(key);
        m_Data.ensureSize(index);
        return m_Data.get(index);
    }

    const ValueType &get(const KeyType & key) const
    {
        size_t index = KeyIndexer::getIndex(key);
        m_Data.ensureSize(index);
        return m_Data.get(index);
    }

    void set(const KeyType & key, ValueType & value)
    {
        size_t index = KeyIndexer::getIndex(key);
        m_Data.ensureSize(index);
        m_Data.set(index, value);
    }

private:
    mutable ContainerWrapper<ValueContainer> m_Data;
};


} }



