#pragma once

#include <vector>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/tags/LongTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API LongArrayTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using LongArrayType = std::vector<LongTag::LongType>;

    LongArrayTag() : BasicTag(TagValue::LongArray) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    LongArrayTag(std::initializer_list<ValueType> init)
        : BasicTag(TagValue::LongArray), m_value(init.begin(), init.end()) {}

    [[nodiscard]] static TagType type() { return TagType::LongArray; }

    [[nodiscard]] static char symbol() { return 'L'; }

    auto operator[](std::size_t index) -> decltype(std::declval<std::vector<LongTag::LongType>&>()[index]);

private:
    LongArrayType m_value{};
};

SHULKER_NBT_NAMESPACE_END
