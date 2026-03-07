#pragma once

#include <cstdint>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API ShortTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using ShortType = std::int16_t;

    ShortTag() : BasicTag(TagValue::SignedShort) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType> && std::is_signed_v<ValueType>, int> = 0>
    ShortTag(ValueType v) : BasicTag(TagValue::SignedShort), m_value(static_cast<ShortType>(v)) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType> && std::is_unsigned_v<ValueType>, int> = 0>
    ShortTag(ValueType v) : BasicTag(TagValue::UnsignedShort), m_value(static_cast<ShortType>(v)) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Short; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const ShortTag& short_tag);

private:
    ShortType m_value{};
};

SHULKER_NBT_NAMESPACE_END
