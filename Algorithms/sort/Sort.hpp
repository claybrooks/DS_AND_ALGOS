#pragma once

#include <algorithm>

namespace Algorithms
{
namespace Sort
{
    template<typename T>
    using increasing = std::greater<T>;

    template<typename T>
    using decreasing = std::less<T>;
}
}