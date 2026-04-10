#pragma once

#include <string>

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API StringTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);

public:
    using StringType = std::string;

    StringTag() : BasicTag(TagValue::String) {}

    StringTag(const char* c) : BasicTag(TagValue::String), m_value(c) {}

    StringTag(const StringType& s) : BasicTag(TagValue::String), m_value(s) {}

    StringTag(StringType&& s) : BasicTag(TagValue::String), m_value(std::move(s)) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::String; }

    [[nodiscard]] static TagId id() noexcept { return 0x08; }

    [[nodiscard]] StringType get() const noexcept { return m_value; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const StringTag& string_tag);

private:
    StringType m_value{};
};

SHULKER_NBT_NAMESPACE_END
