#pragma once

#include <type_traits>

#include "shulker/common/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

template<typename T>
using uncvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

SHULKER_NBT_NAMESPACE_END
