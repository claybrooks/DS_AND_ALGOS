#pragma once

#include "Sorter.hpp"

namespace Algorithms
{
namespace Sort
{
    template<typename Container, template<typename> typename Compare>
    class IntroSort
    {
    public:
        static inline void Sort(
            Container& A,
            const typename Container::size_type& start,
            const typename Container::size_type& end
        )
        {
            // Convert to Container::size_type
            Sort(A, (int)start, (int)end);
        }

    private:

    };

    template<typename Container>
    using IncreasingIntroSort = IntroSort<Container, increasing>;

    template<typename Container>
    using DecreasingIntroSort = IntroSort<Container, decreasing>;
}
}