#include "shulker/tags/LongArrayTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

auto LongArrayTag::operator[](std::size_t index)
    -> decltype(std::declval<std::vector<LongTag::LongType>&>()[index])
{
    return m_value[index];
}

SHULKER_NBT_NAMESPACE_END
