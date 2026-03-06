#pragma once

#include <cstdint>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API ByteTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using ByteType = std::int8_t;

    ByteTag() : BasicTag(TagValue::SignedByte) {}

    template<typename ValueType, std::enable_if_t<
        !std::is_same_v<ValueType, bool> &&
        std::is_integral_v<ValueType> &&
        std::is_signed_v<ValueType>, int> = 0>
    ByteTag(ValueType v) : BasicTag(TagValue::SignedByte), m_value(static_cast<ByteType>(v)) {}

    template<typename ValueType, std::enable_if_t<
        !std::is_same_v<ValueType, bool> &&
        std::is_integral_v<ValueType> &&
        std::is_unsigned_v<ValueType>, int> = 0>
    ByteTag(ValueType v) : BasicTag(TagValue::UnsignedByte), m_value(static_cast<ByteType>(v)) {}

    template<typename ValueType, std::enable_if_t<
        std::is_same_v<ValueType, bool>, int> = 0>
    ByteTag(ValueType v) : BasicTag(TagValue::Bool), m_value(static_cast<ByteType>(v)) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::Byte; }

    [[nodiscard]] ByteType get() const noexcept { return m_value; }

private:
    ByteType m_value{};
};

SHULKER_NBT_NAMESPACE_END
