#pragma once

/**
 * STL Includes
 */
#include <functional>
#include <limits>
#include <math.h>
#include <string>

#include "Internal/HeapExceptions.hpp"

namespace Datastructures {
namespace Heaps {

    template <
        typename T,
        template<typename> class Compare
    >
    class FibonacciHeap {
    public:

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        typedef struct Node
        {
        public:
            /***********************************************************************************************************
            * 
            * 
            ***********************************************************************************************************/
            Node(const T& keyVal)
              : left(this), right(this), parent(nullptr), child(nullptr),
                degree(0), mark(false), key(keyVal), being_removed(false)
            {
            }

            /***********************************************************************************************************
            * 
            * 
            ***********************************************************************************************************/
            static void Clear(Node* node)
            {
                if (!node) { return; }

                node->left          = node;
                node->right         = node;
                node->parent        = nullptr;
                node->child         = nullptr;
                node->degree        = 0;
                node->mark          = false;
                node->being_removed = false;
            }

            /***********************************************************************************************************
            * 
            * 
            ***********************************************************************************************************/
            inline T Key() 
            { 
                return key; 
            }

        private:
            // Make a friend class so we can access privates directly
            friend FibonacciHeap;

            Node* left;
            Node* right;
            Node* parent;
            Node* child;
            int degree;
            bool mark;
            bool being_removed;
            T key;
            //std::string debug_name = "";

        } Node;

        // Helpful using clause for external users
        using NODE_TYPE = Node*;

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        FibonacciHeap()
            : m_count(0)
            , m_doubled_size(50)
            , m_degree_array(nullptr)
            , m_dn(0)
            , m_top(nullptr)
        {
            ResizeDegreeArray(m_doubled_size);
        }

        ~FibonacciHeap()
        {
            // Delete all nodes
            Clear(m_top);

            // Delete the degree array
            if (m_degree_array)
            {
                delete[] m_degree_array;
            }
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        FibonacciHeap(FibonacciHeap&) = delete;

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        FibonacciHeap(FibonacciHeap&&) = delete;

        /***************************************************************************************************************
         *
         *
         ***************************************************************************************************************/
        bool FibonacciHeap::Empty() const
        {
            return !m_count;
        }

        /***************************************************************************************************************
         *
         *
         ***************************************************************************************************************/
        bool FibonacciHeap::Count() const
        {
            return m_count;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        NODE_TYPE Insert(const T& value)
        {
            // Create the new node
            NODE_TYPE new_node = new Node(value);

            return Insert(new_node);
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        NODE_TYPE Insert(NODE_TYPE node)
        {
            if (!node)
            {
                return node;
            }

            // Insert into our root list
            InsertImpl(node);

            // Return back to caller
            return node;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void Merge(FibonacciHeap& heap)
        {
            // Merge the two heaps
            m_top = MergeImpl(m_top, heap.m_top);

            // Update the count
            m_count += heap.m_count;

            // Invalidate the heap that was given to us
            heap.m_top = nullptr;
            heap.m_count = 0;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        NODE_TYPE Top()
        {
            // Just return to the user
            return m_top;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        NODE_TYPE ExtractTop()
        {
            return ExtractTopImpl();
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void AugmentKey(NODE_TYPE node, const T& value)
        {
            AugmentKeyImpl(node, value);
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void Remove(NODE_TYPE node)
        {
            if (node == m_top)
            {
                ExtractTopImpl();
            }
            else
            {            // Mark the node for removal and then call augment key on it.  Then extract top.
                node->being_removed = true;
                AugmentKeyImpl(node, node->key);
                ExtractTopImpl();
            }
        }

    private:
        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void ResizeDegreeArray(unsigned int count)
        {
            // Delete m_degree_array if necessary
            if (m_degree_array)
            {
                delete[] m_degree_array;
            }

            // Recalculate m_dn
            m_dn = ((int)log2(count)) + 1;

            // Allocate a new array
            m_degree_array = new NODE_TYPE[m_dn];
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void InsertImpl(NODE_TYPE new_node)
        {
            // Combine our lists
            m_top = MergeImpl(m_top, new_node);

            // Update our count
            ++m_count;

            // Figure out if we need to resize the dn array
            if (m_count > m_doubled_size)
            {
                // Double it
                m_doubled_size *= 2;

                ResizeDegreeArray(m_doubled_size);
            }
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void RemoveFromCircularList(NODE_TYPE node)
        {
            // Node is only thing in circular list
            if (node->right == node)
            {
                return;
            }

            // Swap left and right pointers
            node->left->right = node->right;
            node->right->left = node->left;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        NODE_TYPE MergeImpl(NODE_TYPE main, NODE_TYPE secondary)
        {
            // There is no main, secondary becomes main
            if (!main)
            {
                return secondary;
            }

            // There is no secondary, main stays untouched
            if (!secondary)
            {
                return main;
            }

            // Ensure that the node returned is always the max/min
            if (compare(secondary->key, main->key))
            {
                std::swap(main, secondary);
            }

            // Store temporary pointers
            NODE_TYPE main_right = main->right;
            NODE_TYPE secondary_left = secondary->left;

            // Link the two lists
            main->right = secondary;
            secondary->left = main;

            // Fill in the gaps
            main_right->left = secondary_left;
            secondary_left->right = main_right;

            return main;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        NODE_TYPE ExtractTopImpl()
        {
            NODE_TYPE top = m_top;

            // We have no max pointer
            if (!top)
            {
                return top;
            }

            // Orphan all children of max
            OrphanAll(top->child);

            // Merge all children into top level list
            MergeImpl(top, top->child);

            // Remove max from circular list
            RemoveFromCircularList(top);

            // If top is the only thing in the list, top is now null
            if (top == top->right)
            {
                m_top = nullptr;
            }
            // Just move top to the right, and handle updating it in the consolidate function
            else
            {
                m_top = top->right;
                Consolidate();
            }

            --m_count;

            // Invalidate top that is returned
            Node::Clear(top);

            return top;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void OrphanAll(NODE_TYPE node)
        {
            Iterate(node, [this](NODE_TYPE next)
            {
                next->parent = nullptr;
            });
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void Consolidate()
        {
            // clear the m_degree_array array
            memset(m_degree_array, 0, sizeof(NODE_TYPE) * m_dn);

            NODE_TYPE parent = m_top;
            bool quit = false;

            while (!quit)
            {
                int d = parent->degree;
                while (m_degree_array[d] != nullptr) 
                {
                    NODE_TYPE child = m_degree_array[d];

                    // this is our exit condition.  All things have been consolidated
                    if (child == parent) 
                    {
                        quit = true;
                        break;
                    }

                    if (compare(child->key, parent->key)) 
                    {
                        std::swap(parent, child);
                    }

                    MakeChild(child, parent);
                    m_degree_array[d++] = nullptr;
                }
                if (!quit) 
                {
                    m_degree_array[parent->degree] = parent;
                    parent = parent->right;
                }
            }

            // Update the max node
            m_top = parent;
            UpdateMax();
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void MakeChild(NODE_TYPE child, NODE_TYPE parent)
        {
            RemoveFromCircularList(child);
            child->left = child;
            child->right = child;
            child->parent = parent;
            parent->child = MergeImpl(parent->child, child);
            child->mark = false;
            ++parent->degree;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void AugmentKeyImpl(NODE_TYPE node, const T& k)
        {
            // If the being_removed flag is set, then this guy is being removed.
            if (!node->being_removed && !compare(k, node->key))
            {
                throw Datastructures::Heaps::Exceptions::InvalidKeyException<T>(k);
            }

            // Update the nodes value
            node->key = k;

            NODE_TYPE parent = node->parent;

            // The node being updated is larger than the parent, or this node is being removed
            if (parent && (node->being_removed || compare(node->key, parent->key)))
            {
                // Do the cutting
                Cut(node, parent);
                CascadingCut(parent);
            }

            // Update the top pointer if new node is larger or being removed
            if (node->being_removed || compare(node->key, m_top->key))
            {
                m_top = node;
            }
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void Cut(NODE_TYPE child, NODE_TYPE parent)
        {
            RemoveFromCircularList(child);

            // Update the parent child pointer node
            if (child->right == child) 
            {
                parent->child = nullptr;
            } else 
            {
                parent->child = child->right;
            }

            // Decrement the degree of y
            --parent->degree;

            // Merge impl requires that the circular linked list be valid, so set left/right pointers to self
            child->left = child;
            child->right = child;

            // Add x to the root list
            MergeImpl(m_top, child);

            // Reset x's pointers
            child->parent = nullptr;
            child->mark = false;
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void CascadingCut(NODE_TYPE node)
        {
            // Get the childs parent
            NODE_TYPE parent = node->parent;

            // We have no parents, done
            if (!parent) 
            {
                return;
            }

            // If the parent is not marked, mark it
            if (!node->mark) 
            {
                node->mark = true;
            } 
            else 
            {
                // cut the child from the parent
                Cut(node, parent);
                CascadingCut(parent);
            }
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void UpdateMax()
        {
            Iterate(m_top, [this](NODE_TYPE next)
            {
                if (compare(next->key, m_top->key))
                {
                    m_top = next;
                }
            });
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void Clear(NODE_TYPE node)
        {
            if (!node)
            {
                return;
            }

            NODE_TYPE iter = node;
            do
            {
                NODE_TYPE tmp = iter;
                iter = iter->right;

                Clear(tmp->child);
                delete tmp;

            } while (iter != node);
        }

        /***************************************************************************************************************
        * 
        * 
        ***************************************************************************************************************/
        void Iterate(NODE_TYPE node, std::function<void(NODE_TYPE next)> func)
        {
            // Don't try if node is nullptr
            if (!node) {
                return;
            }

            NODE_TYPE iter = node;
            do 
            {
                func(iter);
                iter = iter->right;
            } while (iter != node);
        }

        // How many things are in this heap
        unsigned int m_count;

        // Used to dynamically re-size m_degree_array array for consolidate
        unsigned int m_doubled_size;
        NODE_TYPE* m_degree_array;
        unsigned int m_dn;

        // Top of the heap
        NODE_TYPE m_top;

        // Comparison function to determine max/min heap
        Compare<T> compare;
    };

    template <typename T>
    using MaxFibonacciHeap = FibonacciHeap<T, std::greater>;

    template <typename T>
    using MinFibonacciHeap = FibonacciHeap<T, std::less>;
}
}