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

    typedef typename std::vector<element_type>::iterator element_iterator;

    KDNode() {};

    template <typename It>
    KDNode(It begin, It end, const typename Traits::box_type & box, node_type * parent = nullptr, size_t max_elements = 8) 
        : m_Box(box)
        , m_max_elements(max_elements)
        , m_Parent(parent)
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

    template <>
    KDNode(element_iterator begin, element_iterator end, const typename Traits::box_type & box, node_type * parent, size_t max_elements)
        : m_Box(box)
        , m_max_elements(max_elements)
        , m_Parent(parent)
    {
        std::vector<element_type> elements;
        std::for_each(begin, end, [&elements](const auto & obj) {
            elements.emplace_back(obj.first, obj.second);
        });

        if (elements.size() <= max_elements)
            m_Elements.swap(elements);
        else
            add(elements);
    };

    KDNode(std::vector<element_type> & elements, const typename Traits::box_type & box, size_t max_elements = 8)
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

        std::vector<element_type> left_elements, right_elements;
        for (const auto & element : elements)
        {
            if (intersects(element.second, boxes.first) || intersects(*element.first, boxes.first))
                left_elements.emplace_back(element);
            if (intersects(element.second, boxes.second) || intersects(*element.first, boxes.second))
                right_elements.emplace_back(element);
        }
        
        if (elements.size() == right_elements.size() && left_elements.size() == elements.size())
        {
            std::swap(m_Elements, elements);
        }
        else 
        {
            m_Left = std::make_unique<node_type>(left_elements.begin(), left_elements.end(), boxes.first, this, m_max_elements);
            m_Right = std::make_unique<node_type>(right_elements.begin(), right_elements.end(), boxes.second, this, m_max_elements);
        }        
    }

    node_type *next()
    {
        if (isLeaf())
        {
            node_type * ret = this;
            while (ret->m_Parent != nullptr && ret->m_Parent->m_Right == this)
                ret = ret->m_Parent;

            if (ret->m_Parent == nullptr)
                return nullptr;
            else
                return ret->m_Parent->m_Right;
        }
        else
            return m_Left;
    }

    template <typename F>
    node_type *next(F f)
    {

    }

    bool isLeaf() const
    {
        return (m_Left == nullptr);
    }

    size_t countNodes() const
    {
        size_t r = 1;
        if (m_Left)
            r += m_Left->countNodes();
        if (m_Right)
            r += m_Right->countNodes();
        return r;
    }

    size_t countLevels() const
    {
        size_t l = 0;
        if (m_Left)
            l = m_Left->countNodes();
        if (m_Right)
            l = std::max(l, m_Right->countLevels());
        return l + 1;
    }

    template <typename F>
    void forEach(F f)
    {
        f(this);
        if (m_Left)
            m_Left->forEach(f);
        if (m_Right)
            m_Right->forEach(f);
    }

public:
    std::unique_ptr<node_type> m_Left{ nullptr };
    std::unique_ptr<node_type> m_Right{ nullptr };

    node_type *m_Parent{ nullptr };

    typename Traits::box_type m_Box;

    std::vector<element_type> m_Elements;
    size_t m_max_elements;

    typedef typename std::vector<element_type>::iterator element_iterator;

    element_iterator begin() { return m_Elements.begin(); }
    element_iterator end() { return m_Elements.end(); }

    bool empty() const { return m_Elements.empty(); }
};

}  // namespace details

template <typename Object, typename Traits = mesh::Math::GeometryTraits<Object>>
class KDTree
{
    class TestInterface;

    friend class KDTree<Object, Traits>::TestInterface;

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

    template <typename Obj>
    class KDTreeIntersectionIterator;

    size_t countNodes() const 
    {
        if (m_Root)
            return m_Root->countNodes();
        else
            return 0;
    }

    size_t countLevels() const
    {
        if (m_Root)
            return m_Root->countLevels();
        else
            return 0;
    }

    double averageDepth() const 
    {
        if (m_Root)
        {
            std::vector<double> d;
            m_Root->forEach([&](node_type * node)
            {
                if (node->m_Elements.empty() || !node->m_Left && !node->m_Right)
                {
                    double lvl = 1;
                    while ((node = node->m_Parent) != nullptr)
                        ++lvl;
                    d.push_back(lvl);
                }
            });
            if (d.empty())
                return 0;
            else
            {
                auto b = begin(d);
                auto e = end(d);
                if (d.size() > 5)
                {
                    b += 2;
                    e -= 2;
                }
                return std::accumulate(b, e, 0) / d.size();
            }
        }
        else
            return 0;
    }

private:
    std::unique_ptr<node_type> m_Root;

};

template <typename Obj, typename TreeObj>
class IntersectionIterator : boost::iterator_facade<IntersectionIterator<Obj, TreeObj>, TreeObj, boost::forward_traversal_tag>
{
public:
    typedef details::KDNode<TreeObj> node_type;
    typedef typename details::KDNode<TreeObj>::object_iterator object_iterator;

    IntersectionIterator(const Obj * obj, node_type * root_node) 
        : m_Node(root_node)
        , m_CurObj(root_node->begin())
    {
        // Find first intersection object
    }

    void increment()
    {
        ++m_CurObj;
        
        // Find object in current node
        m_CurObj = findNextIntersection();
        if (m_CurObj != m_Node->end())
        {
            return;
        }

        // Go to the next node up
        while (m_Node->m_Parent->m_Right == this)
            up();

        // Move to the right
        do
        {
            right();

            if (!intersects(m_Obj, m_Node->m_Box))
            {
                while (m_Node->m_Parent->m_Right == this)
                    up();
                continue;
            }

            bool intr = true;
            if (m_Node->m_Left != nullptr)
            {
                m_Node = m_Node->m_Left;



                for (; m_Node->m_Left != nullptr; m_Node = m_Node->m_Left)
                {

                }
            }
            
            
            
__goLeft:   ;
            if (!intersects(m_Obj, m_Node->m_Box))
                continue;
            else
            {
                if ()
                {
                    m_CurObj = m_Node->begin();
                    m_CurObj = findNextIntersectingObject();
                    if (m_CurObj == m_Node->end())
                        continue;
                    else
                        return;
                }
                else {
                    goto __goLeft;
                }
            }
        }
        

        // Go down
        while()


        if ()
            right();
        else
            up();

    }

    typename node_type::object_iterator findNextIntersectingObject()
    {
        for (auto obj = m_CurObj; obj != m_Node->end(); ++obj)
        {
            if (intersects(obj, *m_CurObj))
                return obj;
        }
        return m_Node->end();
    }

    void up()
    {
        m_Node = m_Node->m_Parent;
    }

    void right()
    {
        m_Node = m_Node->m_Parent->m_Right;
    }

    TreeObj & dereference()
    {
        return *m_CurObj;
    }

    bool equal(const IntersectionIterator<Obj, TreeObj> & other) const
    {

    }

private:
    Obj * m_Obj;
    node_type * m_Node;
    object_iterator m_CurObj;
};


} }