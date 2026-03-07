#pragma once

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API DoubleTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using DoubleType = double;

    DoubleTag() : BasicTag(TagValue::Double) {}

    DoubleTag(const DoubleType f) : BasicTag(TagValue::Double), m_value(f) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Double; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const DoubleTag& double_tag);

private:
    DoubleType m_value{};
};

SHULKER_NBT_NAMESPACE_END
