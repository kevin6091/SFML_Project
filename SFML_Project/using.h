#pragma once

#include <memory>

using uint = unsigned int;
using int32 = __int32;
using int64 = __int64;

template <typename T>
using uptr = std::unique_ptr<T>;
template <typename T>
using sptr = std::shared_ptr<T>;

