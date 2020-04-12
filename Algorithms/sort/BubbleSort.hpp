#pragma once

#include <algorithm>

namespace Algorithms
{
namespace Sort
{

    template<typename Container, template<typename> typename Compare>
    class BubbleSort
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
            typename Container::const_iterator& start,
            typename Container::const_iterator& end)
        {
            Sort(A, start - A.cbegin(), end - A.cbegin());
        }

        static void Sort(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& end
        )
        {
            for (Container::size_type i = start; i < end; ++i)
            {
                for (Container::size_type j = end; j >= i + 1; --j)
                {
                    if (A[j] < A[j - 1])
                    {
                        std::swap(A[j], A[j - 1]);
                    }
                }
            }
        }
    };

    template<typename Container>
    using MaxBubbleSort = BubbleSort<Container, std::greater>;

    template<typename Container>
    using MinBubbleSort = BubbleSort<Container, std::less>;
}
}
