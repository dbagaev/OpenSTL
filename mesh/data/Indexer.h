#pragma once

namespace mesh { namespace Data {

template <typename KeyType>
class DefaultKeyIndexer
{
public:
    static size_t getIndex(const KeyType & key)
    {
        return key.getIndex();
    }
};

template <typename KeyType>
class Indexer;

template <typename KeyType>
class IndexedKey
{
    friend class Indexer<KeyType>;

public:
    size_t getIndex() const { return m_Index; }

protected:
    void setIndex(size_t value) { m_Index = value; }

private:
    size_t m_Index{ std::numeric_limits<size_t>::max() };
};

template <typename KeyType>
class Indexer
{
public:
    size_t addIndex(KeyType & value) 
    { 
        auto idx = allocateIndex();
        value.setIndex(idx);
        return idx;
    }

    template <typename Iterator>
    void reindex(Iterator begin, Iterator end)
    {
        max_index = 0;
        std::vector<std::pair<size_t, size_t>> index_map;
        index_map.reserve(max_index+1);

        for (auto p = begin; p != end; ++p)
        {
            index_map.emplace_back(p->getIndex(), addIndex(*p));
        }
    }

private:
    size_t allocateIndex()
    {
        size_t idx = max_index;
        ++max_index;
        return idx;
    }

    size_t max_index{ 0 };
};


} }

