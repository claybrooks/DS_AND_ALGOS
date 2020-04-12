#pragma once

#include "MergeSort.hpp"
#include <random>

namespace Algorithms
{
namespace Sort
{
    template<typename Container, template<typename> typename Compare>
    class QuickSort
    {
        static Compare<typename Container::value_type> s_compare;

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
            Sort(A, (int)(start - A.cbegin()), (int)(end - A.cbegin()));
        }

        static inline void Sort(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& end
        )
        {
            // Convert to Container::size_type
            Sort(A, (int)start, (int)end);
        }

        static void Sort(
            Container& A,
            int start,
            int end
        )
        {
            // Be sure to catch underflow
            if (start < end)
            {
                int pivot = (int)RANDOMIZED_PARTITION(A, start, end);
                // This can cause underflow, checked in the if statement above
                Sort(A, start, pivot - 1);
                Sort(A, pivot + 1, end);
            }
        }

    private:

        static inline unsigned long x = 123456789, y = 362436069, z = 521288629;

        static unsigned long xorshf96(void) {          //period 2^96-1
            unsigned long t;
            x ^= x << 16;
            x ^= x >> 5;
            x ^= x << 1;

            t = x;
            x = y;
            y = z;
            z = t ^ x ^ y;

            return z;
        }

        static inline int my_rand(
            int low, 
            int high
        )
        {
            return low + (xorshf96() % (high - low));
        }

        static typename Container::size_type RANDOMIZED_PARTITION(
            Container& A,
            int start,
            int end
        )
        {
            // Get a uniform distribution from the random engine
            //std::uniform_int_distribution<Container::size_type> distr(start, end);
            //Container::size_type i = distr(s_eng);

            int i = my_rand(start, end);

            // Swap the pivot to the end
            std::swap(A[end], A[i]);

            // Partition like normal
            return PARTITION(A, start, end);
        }

        static typename Container::size_type PARTITION(
            Container& A,
            int start,
            int end
        )
        {
            Container::value_type pivot = A[end];
            int i = start - 1;

            // Sort all based on pivot
            for (int j = start; j <= end - 1; ++j)
            {
                if (s_compare(pivot, A[j]))
                {
                    ++i;
                    std::swap(A[i], A[j]);
                }
            }

            // Place the pivot
            std::swap(A[i+1], A[end]);
            return i+1;
        }
    };

    template<typename Container, template<typename> typename Compare>
    Compare<typename Container::value_type> QuickSort<Container, Compare>::s_compare;

    // Init randoms
    //template<typename Container, template<typename> typename Compare>
    //std::random_device QuickSort<Container, Compare>::s_random_device;

    //template<typename Container, template<typename> typename Compare>
    //std::mt19937 QuickSort<Container, Compare>::s_eng(s_random_device());

    template<typename Container>
    using MaxQuickSort = QuickSort<Container, std::greater>;

    template<typename Container>
    using MinQuickSort = QuickSort<Container, std::less>;
}
}