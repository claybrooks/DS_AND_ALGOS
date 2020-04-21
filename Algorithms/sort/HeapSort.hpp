#pragma once

#include "Sorter.hpp"

namespace Algorithms
{
namespace Sort
{
    template<typename Container, template<typename, template<typename> typename> typename Heap, template<typename> typename Compare>
    class HeapSort
    {
    public:
        using compare = Compare<typename Container::value_type>;

        static inline void Sort(
            Container& A
        )
        {
            Sort(A, A.cbegin(), A.cend() - 1);
        }

        static inline void Sort(
            Container& A,
            const typename Container::const_iterator& start,
            const typename Container::const_iterator& end
        )
        {
            Sort(A, start - A.cbegin(), end - A.cbegin());
        }

        static inline void Sort(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& end
        )
        {
            Heap<Container::value_type, Compare> sorter(Container(A.cbegin() + start, A.cbegin() + end + 1));

            Container::size_type i = start;
            while (!sorter.Empty())
            {
                A[i] = sorter.ExtractTop();
                ++i;
            }
        }
    };

    template<typename Container, template<typename, template<typename> typename> typename Heap>
    using IncreasingHeapSort = HeapSort<Container, Heap, increasing>;

    template<typename Container, template<typename, template<typename> typename> typename Heap>
    using DecreasingHeapSort = HeapSort<Container, Heap, decreasing>;
}
}
