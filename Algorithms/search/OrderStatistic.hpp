#pragma once

#include "../sort/QuickSort.hpp"

namespace Algorithms
{
namespace Search
{
    template<typename Container>
    class OrderStatistic
    {
    public:
 
        static inline typename Container::value_type Search(
            Container& A,
            const typename Container::size_type& i
        )
        {
            // Convert to Container::size_type
            return Search(A, i, A.cbegin(), A.cend() - 1);
        }

        static inline typename Container::value_type Search(
            Container& A,
            const typename Container::size_type& i,
            const typename Container::const_iterator& start,
            const typename Container::const_iterator& end
        )
        {
            return Search(A, i, start - A.cbegin(), end - A.cbegin());
        }

        static typename Container::value_type Search(
            Container& A,
            const typename Container::size_type& i,
            const typename Container::size_type& start,
            const typename Container::size_type& end
        )
        {
            if (start == end)
            {
                return A[start];
            }

            Container::size_type middle = IncreasingQuickSort<Container>::RANDOMIZED_PARTITION(A, (int)start, (int)end);
            Container::size_type k = middle - start + 1;

            if (i == k)
            {
                return A[middle];
            }
            else if (i < k)
            {
                return Search(A, i, start, middle - 1);
            }
            else
            {
                return Search(A, i - k, middle + 1, end);
            }
        }
    };
    }
}