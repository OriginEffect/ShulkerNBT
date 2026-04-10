#pragma once

#include <cstring>
#include <type_traits>

#include "shulker/detail/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

template<typename T>
using uncvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
constexpr T byteswap(T value) noexcept
{
    using U = typename std::make_unsigned<T>::type;

    U u = static_cast<U>(value);
    U result = 0;

    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        result <<= 8;
        result |= static_cast<U>(u & static_cast<U>(0xFF));
        u >>= 8;
    }

    return static_cast<T>(result);
}

template<class To, class From>
std::enable_if_t<
    sizeof(To) == sizeof(From) &&
    std::is_trivially_copyable_v<From> &&
    std::is_trivially_copyable_v<To>,
    To>
// constexpr support needs compiler magic
bit_cast(const From& src) noexcept
{
    static_assert(std::is_trivially_constructible_v<To>,
        "This implementation additionally requires "
        "destination type to be trivially constructible");

    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

}

SHULKER_NBT_NAMESPACE_END
