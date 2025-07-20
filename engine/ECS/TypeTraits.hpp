#pragma once

#include <type_traits>

#include "OneFrame.hpp"

namespace x2d
{

template<class T>
struct IsOneFrame : std::false_type
{
};

template<class T>
struct IsOneFrame<OneFrame<T>> : std::true_type
{
    using Wrapped = T;
};

} // namespace x2d

