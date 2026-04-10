#pragma once

#include <cstdint>

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API LongTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);

public:
    using LongType = std::int64_t;

    LongTag() : BasicTag(TagValue::Long) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    LongTag(ValueType v) : BasicTag(TagValue::Long), m_value(static_cast<LongType>(v)) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Long; }

    [[nodiscard]] static TagId id() noexcept { return 0x04; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const LongTag& long_tag);

private:
    LongType m_value{};
};

SHULKER_NBT_NAMESPACE_END
