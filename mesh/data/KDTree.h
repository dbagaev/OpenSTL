#pragma once

#include "Point.h"

#include <Math/Box.h>
#include <Math/GeometryTraits.h>

#include <memory>
#include <algorithm>
#include <numeric>

#include <boost/iterator/iterator_facade.hpp>

namespace mesh { namespace Data {

namespace details {

template<typename Iterator>
class BoxIterator : public boost::iterator_facade<BoxIterator<Iterator>, typename Iterator::value_type, boost::forward_traversal_tag, typename Iterator::reference>
{
public:
    typedef Math::GeometryTraits<typename Iterator::value_type> Traits;
    
    BoxIterator(Iterator it) : m_it(it), m_end(it) { }
    BoxIterator(Iterator it, Iterator end, const typename Traits::box_type box) : m_it(it), m_end(end), m_box(box) { }

    void increment()
    {
        do {
            ++it;
            if (it == end)
                return;
        } while (!(Traits::box(*it) && m_Box));
    }

    typename Iterator::value_type & dereference()
    {
        return *it;
    }

    bool equal(const BoxIterator<Iterator> * o)
    {
        return (o->it == it);
    }

private:
    Iterator m_it;
    Iterator m_end;

    typename Traits::box_type m_box;
};

template <typename Object, typename Traits = mesh::Math::GeometryTraits<Object>>
struct KDNode
{
public:
    typedef KDNode<Object, Traits> node_type;
    typedef std::pair<const Object *, typename Traits::box_type> element_type;

    KDNode() {};

    template <typename It>
    KDNode(It begin, It end, const typename Traits::box_type & box, size_t max_elements = 1) 
        : m_Box(box)
        , m_max_elements(max_elements)
    {
        std::vector<element_type> elements;
        std::for_each(begin, end, [&elements](const auto & obj) {
            elements.emplace_back(&obj, Traits::box(obj));
        });

        if (elements.size() <= max_elements)
            m_Elements.swap(elements);
        else
            add(elements);
    };

    KDNode(std::vector<element_type> & elements, const typename Traits::box_type & box, size_t max_elements = 1)
        : m_Box(box)
        , m_max_elements(max_elements)
    {
        if (elements.size() <= max_elements)
            m_Elements.swap(elements);
        else
            add(elements);
    };

    void add(std::vector<element_type> & elements)
    {
        auto max_size = m_Box.size(0);
        size_t split_dim = 0;
        for (size_t i = 1; i < Traits::box_type::numDimensions(); ++i)
        {
            const auto sz = m_Box.size(i);
            if (max_size < sz)
            {
                max_size = sz;
                split_dim = i;
            }
        }
        auto boxes = m_Box.split(split_dim);
        auto split_pos = boxes.first.maxPosition(split_dim);

        std::vector<element_type> left_elements, right_elements;
        for (const auto & element : elements)
        {
            if (element.second.minPosition(split_dim) <= split_pos)
                left_elements.emplace_back(element);
            if (element.second.maxPosition(split_dim) >= split_pos)
                right_elements.emplace_back(element);
        }
        
        m_Left = std::make_unique<node_type>(left_elements, boxes.first, m_max_elements);
        m_Right = std::make_unique<node_type>(right_elements, boxes.second, m_max_elements);
    }

private:
    std::unique_ptr<node_type> m_Left{ nullptr };
    std::unique_ptr<node_type> m_Right{ nullptr };

    typename Traits::box_type m_Box;

    std::vector<element_type> m_Elements;
    size_t m_max_elements;
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

        m_Root = std::make_unique<node_type>(begin, end, box);
    }

private:
    std::unique_ptr<node_type> m_Root;

};


} }