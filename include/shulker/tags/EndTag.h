#pragma once

#include <variant>

#include "../detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API EndTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);

public:
    using EndType = std::monostate;

    EndTag() = default;

    [[nodiscard]] static TagType type() noexcept { return TagType::End; }

    [[nodiscard]] static TagId id() noexcept { return 0x00; }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const EndTag& end_tag);

private:
    EndType m_value{};
};

SHULKER_NBT_NAMESPACE_END