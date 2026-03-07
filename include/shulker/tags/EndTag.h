#pragma once

#include <variant>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API EndTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using EndType = std::monostate;

    EndTag() = default;

    [[nodiscard]] static TagType type() noexcept { return TagType::End; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const EndTag& end_tag);

private:
    EndType m_value{};
};

SHULKER_NBT_NAMESPACE_END
