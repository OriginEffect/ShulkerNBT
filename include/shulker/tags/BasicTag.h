#pragma once

#include <string>

#include "shulker/common/MacroCommon.h"
#include "shulker/common/MacroScope.h"
#include "shulker/Type.h"

class TagReference;

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API BasicTag
{
private:
    SHULKER_NBT_FRIEND_CLASS(TagReference);
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    BasicTag() = default;
    virtual ~BasicTag() = default;

    [[nodiscard]] virtual TagValue valueType() const;

    [[nodiscard]] static TagType type() noexcept { return TagType::Unknown; }

    [[nodiscard]] std::string dumpSnbt(
        int indent = -1,
        char indent_char = ' ',
        bool keep_snbt_type = false) const;

protected:
    explicit BasicTag(const TagValue value_type) : m_value_type(value_type) {}

private:
    TagValue m_value_type = TagValue::Discarded;
};

SHULKER_NBT_NAMESPACE_END
