#pragma once

#include <vector>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/tags/IntTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API IntArrayTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using IntArrayType = std::vector<IntTag::IntType>;

    IntArrayTag() : BasicTag(TagValue::IntArray) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    IntArrayTag(std::initializer_list<ValueType> init)
        : BasicTag(TagValue::IntArray), m_value(init.begin(), init.end()) {}

    [[nodiscard]] static TagType type() { return TagType::IntArray; }

    [[nodiscard]] static char symbol() { return 'I'; }

private:
    IntArrayType m_value{};
};

SHULKER_NBT_NAMESPACE_END
