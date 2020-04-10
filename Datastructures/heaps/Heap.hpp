#pragma once

#include <algorithm>
#include <vector>
#include <iostream>
#include <type_traits>

#include "Internal/HeapExceptions.hpp"

namespace Datastructures {
namespace Heaps {

    /***************************************************************************************************************
    *
    *
    ***************************************************************************************************************/
    static inline size_t PARENT(size_t i)
    {
        // Offset for 0 index
        return (i - 1) >> 1;
    }

    /***************************************************************************************************************
    *
    *
    ***************************************************************************************************************/
    static inline size_t LEFT(size_t i)
    {
        // Offset for 0 index
        return (i << 1) + 1;
    }

    /***************************************************************************************************************
    *
    *
    ***************************************************************************************************************/
    static inline size_t RIGHT(size_t i)
    {
        return LEFT(i) + 1;
    }

    template <
        typename T,
        template<typename> class Compare
    >
    class Heap {
    public:

        // Helpful using clause for external users
        using NODE_TYPE = T;

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        Heap()
        {
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        Heap(const std::vector<T>& data) : m_storage(data), m_bypass_aug_key_exception(false)
        {
            // size_t is unsigned, so cant check below 0.  Start 1 above the bound, so that way --i places us at
            // n/2 -> 0, and the while (i>0) check will fail on i == 0, but we would have already heapified 0
            size_t i = (data.size() / 2);
            do
            {
                --i;
                BubbleDown(i);
            }
            while (i > 0);
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        bool Empty() const 
        { 
            return !m_storage.size();
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        unsigned int Count() const
        {
            return m_storage.size();
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        const T& Top()
        {
            return m_storage[0];
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        T ExtractTop()
        {
            if (!m_storage.size())
            {
                throw Exceptions::UnderflowException();
            }

            // Get the top item
            T top = m_storage[0];

            // Bring the last item to the front
            m_storage[0] = m_storage.back();

            // Remove the end element
            m_storage.pop_back();

            // Heapify
            BubbleDown(0);

            return top;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void DeltaKey(size_t i, const T& val)
        {
            AugmentKey(i, m_storage[i] + val);
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void AugmentKey(size_t i, const T& val)
        {
            // Beyond our heapsize
            if (i >= m_storage.size())
            {
                throw Datastructures::Heaps::Exceptions::InvalidIndexException(i);
            } 

            // No thanks
            if (!m_bypass_aug_key_exception && !m_heap_propertys(val, m_storage[i]))
            {
                throw Datastructures::Heaps::Exceptions::InvalidKeyException<T>(val);
            }

            // reset bypass
            m_bypass_aug_key_exception = false;

            // Change the value
            m_storage[i] = val;

            // Bubble up
            BubbleUp(i);
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        size_t Insert(const T& val)
        {
            m_storage.push_back(val);
            m_bypass_aug_key_exception = true;
            AugmentKey(m_storage.size()-1, val);

            // Give back the index
            return m_storage.size() - 1;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void Remove(size_t i)
        {
            if (i >= m_storage.size())
            {
                throw Exceptions::InvalidIndexException(i);
            }

            // Trap when user calls remove on index 0
            if (i == 0)
            {
                ExtractTop();
                return;
            }
            
            // Replace element to be removed with back, and pop back
            m_storage[i] = m_storage.back();
            m_storage.pop_back();

            // We removed the back, just ignore
            if (i == m_storage.size()) { return; }
            // We don't have to do anything for empty heaps or heaps of size 1
            if (m_storage.size() < 2) { return; }

            // now, bubble it up or bubble it down
            if (m_heap_propertys(m_storage[i], m_storage[PARENT(i)]))
            {
                BubbleUp(i);
            }
            else
            {
                BubbleDown(i);
            }
        }

    protected:
    private:

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        size_t GetTarget(size_t i)
        {
            size_t target   = i;
            size_t left     = LEFT(i);
            size_t right    = RIGHT(i);

            size_t heap_size = m_storage.size();

            if (left < heap_size && m_heap_propertys(m_storage[left], m_storage[target]))
            {
                target = left;
            }
            if (right < heap_size && m_heap_propertys(m_storage[right], m_storage[target]))
            {
                target = right;
            }

            return target;
        }

        /***************************************************************************************************************
        *
        *
        ***************************************************************************************************************/
        void BubbleUp(size_t i)
        {
            while ((i != 0) && m_heap_propertys(m_storage[i], m_storage[PARENT(i)]))
            {
                std::swap(m_storage[i], m_storage[PARENT(i)]);
                i = PARENT(i);
            }
        }

        /***************************************************************************************************************
        * // Prefer iterative solution over recursion
        *
        ***************************************************************************************************************/
        void BubbleDown(size_t i)
        {
            // Recursive solution
            /*size_t target = GetTarget(i);

            if (target != i)
            {
                std::swap(m_storage[i], m_storage[target]);
                BubbleDown(target);
            }*/

            // Iterate until this condition is met
            bool done = true;

            do
            {
                // Get the target among self and two children
                size_t target = GetTarget(i);

                // If self is the target, we are in the right place
                if (target == i) 
                { 
                    done = true; 
                }
                // Swap with the target, and re assign i to target to re-run on the new i
                else
                {
                    std::swap(m_storage[i], m_storage[target]);
                    i = target;
                }

            } while (!done);
        }

        // underlying storage of the heap
        std::vector<T> m_storage;

        // Used to remove items from the list.  Because the class is templated, it doesn't make sense to change the key value of the item
        // being removed, so just flip a flag and let the remove code handle it.  Also, this allows for the key value to remain untouched,
        // which is good if the values in this heap are pointers
        bool m_bypass_aug_key_exception;

        // Comparison operator used
        // When using this like m_heap_property(A, B), on return of true, it's read like "The ordering of A and B currently satisfies heap property".
        // On return of false, it's read as "A must be swapped with B to maintain heap property"
        Compare<T> m_heap_propertys;
    };

    template<typename T>
    using MaxHeap = Heap<T, std::greater>;

    template<typename T>
    using MinHeap = Heap<T, std::less>;
}
}