#pragma once

#include <cstdint>
#include <iostream>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API IntTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using IntType = std::int32_t;

    IntTag() : BasicTag(TagValue::SignedInt) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType> && std::is_signed_v<ValueType>, int> = 0>
    IntTag(ValueType v) : BasicTag(TagValue::SignedInt), m_value(static_cast<IntType>(v)) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType> && std::is_unsigned_v<ValueType>, int> = 0>
    IntTag(ValueType v) : BasicTag(TagValue::UnsignedInt), m_value(static_cast<IntType>(v)) {}

    [[nodiscard]] static TagType type() noexcept  { return TagType::Int; }

    [[nodiscard]] IntType get() const noexcept { return m_value; }

    friend std::ostream& operator<<(std::ostream& o, const IntTag& tag);

private:
    IntType m_value{};
};

SHULKER_NBT_NAMESPACE_END
