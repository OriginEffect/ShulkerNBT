#pragma once

#include "shulker/detail/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

// endian
//
// Indicates the endianness of all scalar types:
//   - If all scalar types are little-endian, `endian::native` equals `endian::little`.
//   - If all scalar types are big-endian, `endian::native` equals `endian::big`.
// Corner case platforms are also supported:
//   - If all scalar types have sizeof equal to 1, endianness does not matter and all three values, `endian::little`,
//     `endian::big`, and `endian::native` are the same.
//   - If the platform uses mixed endian, `endian::native` equals neither `endian::big` nor `endian::little`.
enum class endian
{
#if defined(_MSC_VER) && !defined(__clang__)
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

constexpr bool is_little_endian()
{
    return endian::native == endian::little;
}

constexpr bool is_big_endian()
{
    return endian::native == endian::big;
}

}

SHULKER_NBT_NAMESPACE_END
