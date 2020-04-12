#pragma once

namespace Algorithms
{
namespace Search
{
    template<typename Container>
    struct ReturnType
    {
        typename Container::size_type start;
        typename Container::size_type end;
        typename Container::value_type sum;

        ReturnType(
            const typename Container::size_type& in_start,
            const typename Container::size_type& in_end,
            const typename Container::value_type& in_sum) :
            start(in_start),
            end(in_end),
            sum(in_sum)
        {
        }
    };

    template<typename Container>
    ReturnType<Container> MaximumSubArray(
        Container& A
    )
    {
        // Convert to Container::size_type
        return MaximumSubArray<Container>(A, A.cbegin(), A.cend() - 1);
    }

    template<typename Container>
    ReturnType<Container> MaximumSubArray(
        Container& A,
        const typename Container::const_iterator& start,
        const typename Container::const_iterator& end
    )
    {
        return MaximumSubArray<Container>(A, start - A.cbegin(), end - A.cbegin());
    }

    template<typename Container>
    ReturnType<Container> MaximumSubArray(
        Container& A,
        const typename Container::size_type& start,
        const typename Container::size_type& end
    )
    {
        using size_type = Container::size_type;
        using value_type = Container::value_type;

        // length of section to iterate over
        size_type n = end - start;

        // Booleans to simulate init to -inf
        bool init_max_sum = true;
        bool init_sum = true;

        // Default to zero, this will be overriden.  Just here to get rid of un-init warnings
        value_type max_sum = 0;
        value_type sum = 0;

        // Track the current lows and highs
        size_type current_high = 0;
        size_type current_low = 0;

        // The final best low and high
        size_type low = 0;
        size_type high = 0;

        for (size_type j = 0; j < n; ++j)
        {
            current_high = j;

            // Our sum is above 0
            if ((sum > 0) || init_sum)
            {
                if (init_sum)
                { 
                    init_sum = false;
                    sum = A[j];
                }
                else
                {
                    sum += A[j];
                }
            }
            // Sum is below zero, reset the current_low var
            else
            {
                current_low = j;
                sum = A[j];
            }

            // Our sum is now largest
            if ((sum > max_sum) || init_max_sum)
            {
                if (init_max_sum)
                {
                    init_max_sum = false;
                }

                // Store point at which our sum has become the largest
                max_sum = sum;
                low     = current_low;
                high    = current_high;
            }
        }

        return ReturnType<Container>(low, high, max_sum);
    }

    // Recursive implementation is slower and worse Complexity
    /*
    template<typename Container>
    ReturnType<Container> MaximumSubArray(
        Container& A,
        const typename Container::size_type& start,
        const typename Container::size_type& end
    )
    {
        using CReturnType = ReturnType<Container>;

        if (start == end) 
        { 
            return CReturnType(start, end, A[start]);
        }
        else
        {
            Container::size_type middle = start + ((end - start) >> 1);

            CReturnType left     = MaximumSubArray(A, start, middle);
            CReturnType right    = MaximumSubArray(A, middle + 1, end);
            CReturnType cross    = FIND_MAX_CROSSING_SUBARRAY(A, start, middle, end);

            if ((left.sum >= right.sum) && (left.sum >= cross.sum))
            {
                return left;
            }
            else if ((right.sum >= left.sum) && (right.sum >= cross.sum))
            {
                return right;
            }
            else
            {
                return cross;
            }
        }
    }

    template<typename Container>
    ReturnType<Container>FIND_MAX_CROSSING_SUBARRAY(
        Container& A,
        const typename Container::size_type& start,
        const typename Container::size_type& middle,
        const typename Container::size_type& end
    )
    {
        using CReturnType = ReturnType<Container>;

        Container::size_type    max_left    = 0;
        Container::size_type    max_right   = 0;
        Container::value_type   left_sum    = 0;
        Container::value_type   right_sum   = 0;
        Container::value_type   sum         = 0;
        bool init_left_sum = true;
        bool init_right_sum = true;

        for (Container::size_type i = middle; i > start; --i)
        {
            sum += A[i];
            if ((sum > left_sum) || init_left_sum)
            {
                left_sum = sum;
                max_left = i;

                if (init_left_sum)
                {
                    init_left_sum = false;
                }
            }
        }

        sum = 0;
        for (Container::size_type j = middle + 1; j < end; ++j)
        {
            sum += A[j];

            if ((sum > right_sum) || init_right_sum)
            {
                right_sum = sum;
                max_right = j;
            }

            if (init_right_sum)
            {
                init_right_sum = false;
            }
        }

        return CReturnType(max_left, max_right, left_sum + right_sum);
    }
    */
}
}