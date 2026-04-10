#pragma once

#include <vector>

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/tags/ByteTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{
    class SnbtParser;
    class NbtReader;
}

class SHULKER_API ByteArrayTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtParser);
    SHULKER_NBT_FRIEND_CLASS(detail::NbtReader);

public:
    using ByteArrayType = std::vector<ByteTag::ByteType>;

    ByteArrayTag() : BasicTag(TagValue::ByteArray) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    ByteArrayTag(std::initializer_list<ValueType> init)
        : BasicTag(TagValue::ByteArray), m_value(init.begin(), init.end()) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::ByteArray; }

    [[nodiscard]] static TagId id() noexcept { return 0x07; }

    [[nodiscard]] static char symbol() noexcept { return 'B'; }

    auto operator[](std::size_t index) -> decltype(std::declval<std::vector<ByteTag::ByteType>&>()[index]);

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const ByteArrayTag& byte_array_tag);

private:
    ByteArrayType m_value{};
};

SHULKER_NBT_NAMESPACE_END
