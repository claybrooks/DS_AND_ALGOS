#pragma once

namespace Algorithms
{
namespace Sort
{
    // TODO Need to find a way to preallocate memory for L and R temp vectors in MERGE function
    // Should be creating a vectors L and R, reserving memory the size of half of the input
    template<typename Container, template<typename> typename Compare>
    class MergeSort
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

            size_t middle = start + ((end - start) >> 1);

            Sort (A, start, middle, L, R);
            Sort (A, middle + 1, end, L, R);
            MERGE(A, start, middle, end, L, R);
        }

        static void MERGE(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& middle,
            const typename Container::size_type& end,
            Container& L,
            Container& R
        )
        {
            typename Container::size_type n_1 = middle - start + 1;
            typename Container::size_type n_2 = end - middle;

            // Copy data into new containers
            for (Container::size_type i = 0; i < n_1; ++i)
            {
                L[i] = A[start + i];
            }
            for (Container::size_type j = 0; j < n_2; ++j)
            {
                R[j] = A[middle + j + 1];
            }

            //std::vector<Container::value_type> L(A.cbegin() + start,        A.cbegin() + start + n_1);
            //std::vector<Container::value_type> R(A.cbegin() + middle + 1,   A.cbegin() + middle + 1 + n_2);

            Container::size_type i = 0;
            Container::size_type j = 0;

            // Do the actual comparison copies
            for (Container::size_type k = start; k <= end; ++k)
            {
                if (i >= n_1)
                {
                    A[k] = R[j++];
                }
                else if (j >= n_2)
                {
                    A[k] = L[i++];
                }
                else if (s_compare(R[j], L[i]))
                {
                    A[k] = L[i++];
                }
                else
                {
                    A[k] = R[j++];
                }
            }
        }
    };

    template<typename Container, template<typename> typename Compare>
    Compare<typename Container::value_type> MergeSort<Container, Compare>::s_compare;

    template<typename Container>
    using MaxMergeSort = MergeSort<Container, std::greater>;

    template<typename Container>
    using MinMergeSort = MergeSort<Container, std::less>;
}
}
