#pragma once

#include <type_traits>

#include "shulker/common/MacroScope.h"

#include "shulker/meta/CppFuture.h"
#include "shulker/tags/ByteArrayTag.h"
#include "shulker/tags/DoubleTag.h"
#include "shulker/tags/FloatTag.h"
#include "shulker/tags/IntArrayTag.h"
#include "shulker/tags/LongArrayTag.h"
#include "shulker/tags/ShortTag.h"

SHULKER_NBT_NAMESPACE_BEGIN
    template<typename T, typename = void>
struct is_array_tag : std::false_type {};

template<typename T>
struct is_array_tag<T, std::enable_if_t<
    std::is_same<uncvref_t<T>, ByteArrayTag>::value ||
    std::is_same<uncvref_t<T>, IntArrayTag>::value ||
    std::is_same<uncvref_t<T>, LongArrayTag>::value>>
    : std::true_type
{};

template<typename T, typename = void>
struct is_float_tag : std::false_type {};

template<typename T>
struct is_float_tag<T, std::enable_if_t<
    std::is_same<uncvref_t<T>, FloatTag>::value ||
    std::is_same<uncvref_t<T>, DoubleTag>::value>>
    : std::true_type
{};

template<typename T, typename = void>
struct is_integer_tag : std::false_type {};

template<typename T>
struct is_integer_tag<T, std::enable_if_t<
    std::is_same<uncvref_t<T>, ByteTag>::value ||
    std::is_same<uncvref_t<T>, ShortTag>::value ||
    std::is_same<uncvref_t<T>, IntTag>::value ||
    std::is_same<uncvref_t<T>, LongTag>::value>>
    : std::true_type
{};

template <typename ValueType>
struct tag_from_value_size
{
private:
    static constexpr std::size_t N = sizeof(ValueType);

public:
    using type =
        std::conditional_t<N == sizeof(ByteTag::ByteType), ByteTag,
        std::conditional_t<N == sizeof(ShortTag::ShortType), ShortTag,
        std::conditional_t<N == sizeof(IntTag::IntType), IntTag,
                           LongTag>>>;
};

template <typename ValueType>
using tag_from_value_size_t = typename tag_from_value_size<ValueType>::type;

SHULKER_NBT_NAMESPACE_END
