#include <Data/Indexer.h>

#include "gtest/gtest.h"

using namespace mesh;

class DummyIndexed : public Data::IndexedKey<DummyIndexed>
{

};

class DummyIndexer : public Data::Indexer<DummyIndexed>
{
public:
    DummyIndexed add()
    {
        DummyIndexed di;
        this->addIndex(di);
        return di;
    }

    template <typename It>
    void reindex(It b, It e)
    {
        Data::Indexer<DummyIndexed>::reindex(b, e);
    }

};


TEST(Test_Indexing, Simple)
{
    DummyIndexer indexer;

    auto i0 = indexer.add();
    ASSERT_EQ(0, i0.getIndex());

    auto i1 = indexer.add();
    ASSERT_EQ(1, i1.getIndex());

    auto i2 = indexer.add();
    ASSERT_EQ(2, i2.getIndex());

    std::vector<DummyIndexed> v{ i2, i0 };
    ASSERT_EQ(2, v[0].getIndex());
    ASSERT_EQ(0, v[1].getIndex());
    
    indexer.reindex(v.begin(), v.end());
    ASSERT_EQ(0, v[0].getIndex());
    ASSERT_EQ(1, v[1].getIndex());
}
 