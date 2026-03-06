#include "shulker/tags/IntArrayTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

auto IntArrayTag::operator[](std::size_t index)
    -> decltype(std::declval<std::vector<IntTag::IntType>&>()[index])
{
    return m_value[index];
}

SHULKER_NBT_NAMESPACE_END
