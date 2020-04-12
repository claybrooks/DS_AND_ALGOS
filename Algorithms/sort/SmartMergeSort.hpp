#pragma once

#include "MergeSort.hpp"

namespace Algorithms
{
namespace Sort
{
    template<typename Container, template<typename, template<typename>typename> typename SmallSorter, template<typename> typename Compare>
    class SmartMergeSort
    {
        using MERGE_SORT = MergeSort<Container, Compare>;
        using SMALL_SORT = SmallSorter<Container, Compare>;

    public:

        static inline void Sort(
            Container& A
        )
        {
            // Convert to Container::size_type
            Sort(A, A.cbegin(), A.cend() - 1);
        }

        static inline void Sort(
            Container& A,
            const typename Container::const_iterator& start,
            const typename Container::const_iterator& end
        )
        {
            // Convert to Container::size_type
            Sort(A, start - A.cbegin(), end - A.cbegin());
        }

        static void Sort(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& end
        )
        {
            // Pre allocate playground space
            // Maximum size needed for L and R is both (end - start)/2 + 1
            Container::size_type to_reserve = ((end - start) >> 1) + 1;
            Container L;
            Container R;

            L.resize(to_reserve);
            R.resize(to_reserve);

            Sort(A, start, end, L, R);
        }

        static void Sort(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& end,
            Container& L,
            Container& R
        )
        {
            if (start >= end) { return; }

            if (end - start <= 40)
            {
                SMALL_SORT::Sort(A, start, end);
            }
            else
            {
                size_t middle = start + ((end - start) >> 1);

                Sort(A, start, middle, L, R);
                Sort(A, middle + 1, end, L, R);
                MERGE_SORT::MERGE(A, start, middle, end, L, R);
            }
        }
    };

    template<typename Container, template<typename, template<typename> typename> typename SmallSorter>
    using MaxSmartMergeSort = SmartMergeSort<Container, SmallSorter, std::greater>;

    template<typename Container, template<typename, template<typename> typename> typename SmallSorter>
    using MinSmartMergeSort = SmartMergeSort<Container, SmallSorter, std::less>;
}
}