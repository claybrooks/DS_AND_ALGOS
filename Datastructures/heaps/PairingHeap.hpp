#pragma once

#include "Heap.hpp"
#include <math.h>

namespace Datastructures {
namespace Heaps {

    template <
        typename T,
        template<typename> class Compare
    >
    class PairingHeap 
    {
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
                : left(nullptr), right(nullptr), child(nullptr), key(keyVal)
            {
            }

            /***********************************************************************************************************
            *
            *
            ***********************************************************************************************************/
            static void Clear(Node* node)
            {
                if (!node) { return; }

                node->left  = nullptr;
                node->right = nullptr;
                node->child = nullptr;
            }

            inline T Key() const
            {
                return key;
            }

        private:
            friend PairingHeap;

            Node* left;
            Node* right;
            Node* child;
            T key;

        } Node;

        // Helpful using clause for external users
        using NODE_TYPE = Node*;

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        PairingHeap()
            : m_count(0),
            m_merge_array(nullptr),
            m_merge_array_size(20),
            m_top(nullptr)
        {
            AllocateMergeArray(m_merge_array_size);
        }

        ~PairingHeap()
        {
            // Delete all nodes
            Clear(m_top);

            if (m_merge_array)
            {
                delete[] m_merge_array;
            }
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        PairingHeap(PairingHeap&) = delete;

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        PairingHeap(PairingHeap&&) = delete;

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        bool PairingHeap::Empty() const
        {
            return !m_count;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        unsigned int PairingHeap::Count() const
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
            m_top = MergeImpl(m_top, node);

            // Increment the count
            ++m_count;

            if (m_count == m_merge_array_size)
            {
                AllocateMergeArray(m_count << 1);
            }

            // Return back to caller
            return node;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void Merge(PairingHeap& heap)
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
            NODE_TYPE ret = ExtractTopImpl();
            return ret;
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
            {
                 RemoveAndMeldImpl(node);
            }
        }

    private:

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void AllocateMergeArray(unsigned int size)
        {
            if (m_merge_array)
            {
                delete[] m_merge_array;
            }

            m_merge_array_size = size;

            m_merge_array = new NODE_TYPE[m_merge_array_size];
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void RemoveFromSiblingList(NODE_TYPE node)
        {
            // Node is only thing in circular list
            if (node->right == nullptr && node->left == nullptr)
            {
                return;
            }

            // Check to see if this node is the first sibling
            if (node->left->child == node)
            {
                // ->left is parent in this case, so set parents child to the right
                node->left->child = node->right;

                // Swap this as normal
                if (node->right)
                {
                    node->right->left = node->left;
                }
            }
            else
            {
                // The node must have a left pointer
                node->left->right = node->right;

                // The node may not have a right pointer
                if (node->right)
                {
                    node->right->left = node->left;
                }
            }

            // Invalidate the left and right pointers of this node
            node->left  = nullptr;
            node->right = nullptr;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        NODE_TYPE MergeImpl(NODE_TYPE main, NODE_TYPE secondary)
        {
            // There is no main, secondary becomes main
            if (!main) { return secondary; }

            // There is no secondary, main stays untouched
            if (!secondary) { return main; }

            // Ensure that the node returned is always the max/min
            if (m_heap_property(secondary->key, main->key))
            {
                std::swap(main, secondary);
            }

            // Make secondary a main of child
            MakeChild(secondary, main);

            return main;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        NODE_TYPE ExtractTopImpl()
        {
            // Save the previous top
            NODE_TYPE top = m_top;

            // invalidate top so the function below can meld
            m_top = nullptr;

            RemoveAndMeldImpl(top);

            return top;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        NODE_TYPE TwoPassScheme(NODE_TYPE node)
        {
            // We don't have to check for !node here, because it's guaranteed that node is not null

            // This is the only node
            if (!node->right) 
            { 
                // The node can't have a left here
                node->left = nullptr;
                return node; 
            }

            // Algorithms
            // Step 1. Iterate left to right and merge pairs.  If odd number, merge last node with last pair
            int i = -1;
            NODE_TYPE iter = node;
            NODE_TYPE next = nullptr;
            NODE_TYPE right = nullptr;
            do 
            {
                // Increment to the next index
                ++i;

                // Reset the array index
                // I noticed there is a performance hit on large heaps if you manually clear this array before hand.
                // Even memset adds some over head.  Since we have to iterate across this array, why not just clear as we
                // go?
                m_merge_array[i] = nullptr;

                // Reset next to null
                next = nullptr;

                // We don't have a right, means we are odd. So merge with the previous result
                if (!iter->right)
                {
                    // Reset left pointer
                    iter->left = nullptr;

                    // Merge in to last merged pair
                    m_merge_array[i - 1] = MergeImpl(m_merge_array[i - 1], iter);
                }
                else
                {
                    // save right pointer;
                    right = iter->right;

                    // Save the node we are going to next
                    next = right->right;

                    // Reset right and left pointers for both iter and right
                    iter->right     = iter->left = nullptr;
                    right->right    = right->left = nullptr;

                    // Merge node and node's right
                    m_merge_array[i] = MergeImpl(iter, right);
                }

                // Just go to nullptr, the while condition will exit for us
                iter = next;

            } while (iter);

            // Step 2.  Iterate left to right and merge into working tree.  Working tree is the last index
            NODE_TYPE working_tree = m_merge_array[i];

            while (i > 0)
            {
                --i;
                working_tree = MergeImpl(working_tree, m_merge_array[i]);
            }

            return working_tree;
        }

        /***************************************************************************************************************
        *
        *
            ***************************************************************************************************************/
        void MakeChild(NODE_TYPE child, NODE_TYPE parent)
        {
            if (!parent->child)
            {
                parent->child   = child;
                child->left     = parent;
            }
            else
            {
                NODE_TYPE prev_child    = parent->child;
                parent->child           = child;
                child->left             = parent;
                child->right            = prev_child;
                prev_child->left        = child;
            }
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void AugmentKeyImpl(NODE_TYPE node, const T& k)
        {
            if (!m_heap_property(k, node->key))
            {
                throw Datastructures::Heaps::Exceptions::InvalidKeyException<T>(k);
            }

            // Update the nodes value
            node->key = k;

            // If we are not top, remove and merge with top
            if (node != m_top)
            {
                RemoveFromSiblingList(node);
                m_top = MergeImpl(m_top, node);
            }
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void RemoveAndMeldImpl(NODE_TYPE node)
        {
            // Don't have to check validity of node, it's guaranteed to be non null here

            NODE_TYPE melded_children;

            // Remove from the sibling list
            RemoveFromSiblingList(node);

            // node doesn't have a child, so now becomes nullptr
            if (!node->child)
            {
                melded_children = nullptr;
            }
            else
            {
                melded_children = TwoPassScheme(node->child);
            }

            // If m_top is nullptr, then just make m_top melded children
            if (!m_top)
            {
                m_top = melded_children;
            }
            // Merge the melded children into m_top
            else
            {
                m_top = MergeImpl(m_top, melded_children);
            }

            --m_count;

            // Invalidate top that is returned
            Node::Clear(node);
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

            } while (iter);
        }

        // How many things are in this heap
        unsigned int m_count;

        NODE_TYPE* m_merge_array;
        unsigned int m_merge_array_size;

        // Top of the heap
        NODE_TYPE m_top;

        // Comparison function to determine max/min heap
        Compare<T> m_heap_property;
    };

    template <typename T>
    using MaxPairingHeap = PairingHeap<T, max_heap>;

    template <typename T>
    using MinPairingHeap = PairingHeap<T, min_heap>;
}
}