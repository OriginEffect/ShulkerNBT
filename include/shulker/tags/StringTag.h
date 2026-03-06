#pragma once

#include <string>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API StringTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using StringType = std::string;

    StringTag() : BasicTag(TagValue::String) {}

    StringTag(const StringType& s) : BasicTag(TagValue::String), m_value(s) {}

    StringTag(StringType&& s) : BasicTag(TagValue::String), m_value(std::move(s)) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::String; }

    [[nodiscard]] StringType get() const noexcept { return m_value; }

private:
    StringType m_value{};
};

SHULKER_NBT_NAMESPACE_END
