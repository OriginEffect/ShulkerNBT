#pragma once

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API FloatTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using FloatType = float;

    FloatTag() : BasicTag(TagValue::Float) {}

    FloatTag(const FloatType f) : BasicTag(TagValue::Float), m_value(f) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Float; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const FloatTag& float_tag);

private:
    FloatType m_value{};
};

SHULKER_NBT_NAMESPACE_END
