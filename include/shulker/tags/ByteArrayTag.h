#pragma once

#include <vector>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/tags/ByteTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API ByteArrayTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using ByteArrayType = std::vector<ByteTag::ByteType>;

    ByteArrayTag() : BasicTag(TagValue::ByteArray) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    ByteArrayTag(std::initializer_list<ValueType> init)
        : BasicTag(TagValue::ByteArray), m_value(init.begin(), init.end()) {}

    [[nodiscard]] static TagType type() { return TagType::ByteArray; }

    [[nodiscard]] static char symbol() { return 'B'; }

    auto operator[](std::size_t index) -> decltype(std::declval<std::vector<ByteTag::ByteType>&>()[index]);

private:
    ByteArrayType m_value{};
};

SHULKER_NBT_NAMESPACE_END
