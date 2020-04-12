#pragma once

namespace Algorithms
{
namespace Sort
{
    template<typename Container, template<typename, template<typename> typename> typename Heap, template<typename> typename Compare>
    class HeapSort
    {
    public:
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
            Heap<Container::value_type, Compare> sorter(Container(A.cbegin() + start, A.cbegin() + end));

            Container::size_type i = 0;
            while (!sorter.Empty())
            {
                A[i] = sorter.ExtractTop();
                ++i;
            }
        }
    };

    template<typename Container, template<typename, template<typename> typename> typename Heap>
    using MaxHeapSort = HeapSort<Container, Heap, std::greater>;

    template<typename Container, template<typename, template<typename> typename> typename Heap>
    using MinHeapSort = HeapSort<Container, Heap, std::less>;
}
}