#pragma once

#include <algorithm>
#include "TreeExceptions.hpp"

namespace Datastructures
{
namespace Trees
{
    template<typename Type>
    using max_tree = std::greater_equal<Type>;

    template<typename Type>
    using min_tree = std::less_equal<Type>;
}
}