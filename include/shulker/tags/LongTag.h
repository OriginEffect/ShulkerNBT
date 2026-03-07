#pragma once

#include <cstdint>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API LongTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using LongType = std::int64_t;

    LongTag() : BasicTag(TagValue::SignedLong) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType> && std::is_signed_v<ValueType>, int> = 0>
    LongTag(ValueType v) : BasicTag(TagValue::SignedLong), m_value(static_cast<LongType>(v)) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType> && std::is_unsigned_v<ValueType>, int> = 0>
    LongTag(ValueType v) : BasicTag(TagValue::UnsignedLong), m_value(static_cast<LongType>(v)) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Long; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const LongTag& long_tag);

private:
    LongType m_value{};
};

SHULKER_NBT_NAMESPACE_END
