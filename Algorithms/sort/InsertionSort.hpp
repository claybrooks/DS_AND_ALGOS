#pragma once

#include "Sort.hpp"

namespace Algorithms
{
namespace Sort
{
    template<typename Container, template<typename> typename Compare>
    class InsertionSort
    {
        static Compare<typename Container::value_type> s_compare;

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
            // Convert to Container::size_type
            Sort(A, start - A.cbegin(), end - A.cbegin());
        }

        static void Sort(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& end
        )
        {
            Container::size_type max_size = end - start + 1;

            for (Container::size_type i = start + 1; i <= end; ++i)
            {
                Container::value_type key = A[i];

                Container::size_type j = i - 1;
                while ((j < max_size && j >= 0) && s_compare(A[j], key))
                {
                    A[j + 1] = A[j];
                    // This causes underflow, catch it above
                    --j;
                }

                A[j + 1] = key;
            }
        }
    };

    template<typename Container, template<typename> typename Compare>
    Compare<typename Container::value_type> InsertionSort<Container, Compare>::s_compare;

    template<typename Container>
    using IncreasingInsertionSort = InsertionSort<Container, increasing>;

    template<typename Container>
    using DecreasingInsertionSort = InsertionSort<Container, decreasing>;
}
}
