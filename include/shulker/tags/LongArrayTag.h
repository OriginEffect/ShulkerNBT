#pragma once

#include <vector>

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/tags/LongTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{
    class SnbtParser;
    class NbtReader;
}

class SHULKER_API LongArrayTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtParser);
    SHULKER_NBT_FRIEND_CLASS(detail::NbtReader);

public:
    using LongArrayType = std::vector<LongTag::LongType>;

    LongArrayTag() : BasicTag(TagValue::LongArray) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    LongArrayTag(std::initializer_list<ValueType> init)
        : BasicTag(TagValue::LongArray), m_value(init.begin(), init.end()) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::LongArray; }

    [[nodiscard]] static TagId id() noexcept { return 0x0c; }

    [[nodiscard]] static char symbol() noexcept { return 'L'; }

    auto operator[](std::size_t index) -> decltype(std::declval<std::vector<LongTag::LongType>&>()[index]);

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const LongArrayTag& long_array_tag);

private:
    LongArrayType m_value{};
};

SHULKER_NBT_NAMESPACE_END
