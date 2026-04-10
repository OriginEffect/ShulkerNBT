#pragma once

#include <vector>

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/tags/IntTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{
    class SnbtParser;
    class NbtReader;
}

class SHULKER_API IntArrayTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtParser);
    SHULKER_NBT_FRIEND_CLASS(detail::NbtReader);

public:
    using IntArrayType = std::vector<IntTag::IntType>;

    IntArrayTag() : BasicTag(TagValue::IntArray) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    IntArrayTag(std::initializer_list<ValueType> init)
        : BasicTag(TagValue::IntArray), m_value(init.begin(), init.end()) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::IntArray; }

    [[nodiscard]] static TagId id() noexcept { return 0x0b; }

    [[nodiscard]] static char symbol() noexcept { return 'I'; }

    auto operator[](std::size_t index) -> decltype(std::declval<std::vector<IntTag::IntType>&>()[index]);

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const IntArrayTag& int_array_tag);

private:
    IntArrayType m_value{};
};

SHULKER_NBT_NAMESPACE_END
