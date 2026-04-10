#pragma once

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API DoubleTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);

public:
    using DoubleType = double;

    DoubleTag() : BasicTag(TagValue::Double) {}

    DoubleTag(const DoubleType f) : BasicTag(TagValue::Double), m_value(f) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Double; }

    [[nodiscard]] static TagId id() noexcept { return 0x06; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const DoubleTag& double_tag);

private:
    DoubleType m_value{};
};

SHULKER_NBT_NAMESPACE_END
