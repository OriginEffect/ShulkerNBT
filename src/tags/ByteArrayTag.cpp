#include "shulker/tags/ByteArrayTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

auto ByteArrayTag::operator[](std::size_t index)
    -> decltype(std::declval<std::vector<ByteTag::ByteType>&>()[index])
{
    return m_value[index];
}

SHULKER_NBT_NAMESPACE_END
