#pragma once

#include <string>

#include "shulker/detail/MacroCommon.h"
#include "shulker/detail/MacroScope.h"
#include "shulker/Type.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{
    class NbtWriter;
    class SnbtSerializer;
}

class SHULKER_API BasicTag
{
private:
    SHULKER_NBT_FRIEND_CLASS(TagReference);
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter);
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);

public:
    BasicTag() = default;
    virtual ~BasicTag() = default;

    [[nodiscard]] virtual TagValue valueType() const;

    [[nodiscard]] static TagType type() noexcept { return TagType::Unknown; }

    [[nodiscard]] static TagId id() noexcept { return 0x00; }

    [[nodiscard]] std::string dumpSnbt(
        int indent = -1,
        char indent_char = ' ',
        bool keep_bool_type = false,
        bool ensure_ascii = false) const;

protected:
    explicit BasicTag(const TagValue value_type) : m_value_type(value_type) {}

private:
    TagValue m_value_type = TagValue::Discarded;
};

SHULKER_NBT_NAMESPACE_END
