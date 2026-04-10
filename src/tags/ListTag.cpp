#include <utility>

#include "shulker/detail/io/output/OutputAdapters.h"
#include "shulker/detail/io/output/SnbtSerializer.h"
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

bool ListTag::isHeterogeneous() const
{
    auto it = m_value.cbegin();
    TagType elem_type = it->getType();
    for (++it; it != m_value.cend(); ++it) {
        if (it->getType() != elem_type) {
            return true;
        }
    }
    return false;
}

TagType ListTag::elementType() const
{
    if (isHeterogeneous()) {
        return TagType::Compound;
    }
    return m_value[0].getType();
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

std::ostream& operator<<(std::ostream& os, const ListTag& list_tag)
{
    const bool pretty_print = os.width() > 0;
    const auto indentation = pretty_print ? os.width() : 0;

    os.width(0);

    detail::SnbtSerializer serializer(detail::OutputAdapter<char>(os), os.fill());
    serializer.dump(list_tag, pretty_print, false, false, static_cast<unsigned int>(indentation));
    return os;
}

SHULKER_NBT_NAMESPACE_END
