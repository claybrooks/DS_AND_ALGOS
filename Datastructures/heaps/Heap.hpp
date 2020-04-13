#pragma once

#include "HeapExceptions.hpp"

#include <algorithm>

namespace Datastructures
{
namespace Heaps
{
    template<typename T>
    using max_heap = std::greater<T>;

    template<typename T>
    using min_heap = std::less<T>;
}
}