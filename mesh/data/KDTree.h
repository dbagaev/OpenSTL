#pragma once

#include "Point.h"

#include <Math/Box.h>
#include <Math/GeometryTraits.h>

#include <memory>
#include <algorithm>
#include <numeric>

namespace mesh { namespace Data {

namespace details {

template <typename Object, typename Traits = mesh::Math::GeometryTraits<Object>>
struct KDNode
{
public:
    typedef KDNode<Object, Traits> node_type;

    KDNode() {};
    KDNode(const typename Traits::box_type & box) : m_Box(box) {};

    template <typename It>
    void add(It b, It e)
    {

    }

private:
    std::unique_ptr<node_type> m_Left{ nullptr };
    std::unique_ptr<node_type> m_Right{ nullptr };

    typename Traits::box_type m_Box;

    std::vector<Object *> m_Elements;
};

}  // namespace details

template <typename Object, typename Traits = mesh::Math::GeometryTraits<Object>>
class KDTree
{
public:
    typedef details::KDNode<Object, Traits> node_type;

    KDTree() : m_Root() {};

    template <typename It>
    KDTree(It begin, It end)
    {
        auto box = std::accumulate(begin, end, Traits::box(*begin), [](const auto & b1, const auto & b2) {
            return b1 | Traits::box(b2);
        });

        m_Root = std::make_unique<node_type>(box);
        m_Root->add(begin, end);
    }

private:
    std::unique_ptr<node_type> m_Root;

};


} }