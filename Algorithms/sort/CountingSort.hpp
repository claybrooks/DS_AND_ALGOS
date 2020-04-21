#pragma once

#include "Sorter.hpp"

namespace Algorithms
{
namespace Sort
{

    template<typename Container, typename Container::size_type Range, template<typename> typename Compare>
    class CountingSort
    {
    public:
        using compare = Compare<typename Container::value_type>;

        static void Sort(
            Container& A
        )
        {
            Container B;
            B.resize(A.size());

            Sort(A, B);

            // Just swap
            A.swap(B);
        }

        static void Sort(
            const Container& A,
            Container& B
        )
        {
            Container::value_type C[Range];

            // Clear array
            memset(C, 0, sizeof(Container::value_type) * Range);

            // Count how many of each element there is
            for (Container::size_type j = 1; j < A.size(); ++j)
            {
                C[A[j]] = C[A[j]] + 1;
            }

            for (Container::size_type i = 1; i < Range; ++i)
            {
                C[i] = C[i] + C[i - 1];
            }

            for (int j = (int)A.size()-1; j >= 0; --j)
            {
                B[C[A[j]]] = A[j];
                C[A[j]] = C[A[j]] - 1;
            }
        }
    };

    template<typename Container, typename Container::size_type Range>
    using IncreasingCountingSort = CountingSort<Container, Range, increasing>;

    template<typename Container, typename Container::size_type Range>
    using DecreasingCountingSort = CountingSort<Container, Range, decreasing>;
}
}
