#pragma once

#include <cstdint>

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API ShortTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);

public:
    using ShortType = std::int16_t;

    ShortTag() : BasicTag(TagValue::Short) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType> && std::is_signed_v<ValueType>, int> = 0>
    ShortTag(ValueType v) : BasicTag(TagValue::Short), m_value(static_cast<ShortType>(v)) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Short; }

    [[nodiscard]] static TagId id() noexcept { return 0x02; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const ShortTag& short_tag);

private:
    ShortType m_value{};
};

SHULKER_NBT_NAMESPACE_END
