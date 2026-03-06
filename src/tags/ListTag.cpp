#include <utility>

#include "shulker/tags/ListTag.h"
#include "shulker/tags/TagReference.h"

SHULKER_NBT_NAMESPACE_BEGIN

ListTag::ListTag() : BasicTag(TagValue::List) {}
ListTag::~ListTag() = default;
ListTag::ListTag(const ListTag& other) = default;
ListTag::ListTag(ListTag&& other) noexcept = default;
ListTag& ListTag::operator=(const ListTag& other) = default;
ListTag& ListTag::operator=(ListTag&& other) noexcept = default;

ListTag::ListTag(const ListType& vec) : BasicTag(TagValue::List), m_value(vec) {}

ListTag::ListTag(ListType&& vec) : BasicTag(TagValue::List), m_value(std::move(vec)) {}

ListTag::ListTag(std::initializer_list<TagReference> init) : BasicTag(TagValue::List), m_value(init) {}

std::size_t ListTag::size() const noexcept
{
    return m_value.size();
}

TagReference& ListTag::operator[](std::size_t index)
{
    if (index >= m_value.size()) {
        m_value.resize(index + 1);
    }
    return m_value.operator[](index);
}

const TagReference& ListTag::operator[](std::size_t index) const
{
    return m_value.operator[](index);
}

SHULKER_NBT_NAMESPACE_END
