#include <utility>

#include "shulker/tags/CompoundTag.h"
#include "shulker/tags/TagReference.h"
#include "shulker/io/adapters/OutputAdapters.h"
#include "shulker/io/snbt/SnbtSerializer.h"

SHULKER_NBT_NAMESPACE_BEGIN

CompoundTag::CompoundTag() : BasicTag(TagValue::Compound) {}

CompoundTag::CompoundTag(const CompoundType& map): BasicTag(TagValue::Compound), m_value(map) {}

CompoundTag::CompoundTag(CompoundType&& map) : BasicTag(TagValue::Compound), m_value(std::move(map)) {}

CompoundTag::CompoundTag(std::initializer_list<std::pair<CompoundType::key_type, TagReference>> init)
    : BasicTag(TagValue::Compound), m_value(init.begin(), init.end()) {}

TagReference& CompoundTag::operator[](CompoundType::key_type key)
{
    return m_value.emplace(std::move(key), EndTag{}).first->second;
}

const TagReference& CompoundTag::operator[](const CompoundType::key_type& key) const
{

}

std::ostream& operator<<(std::ostream& os, const CompoundTag& compound_tag)
{
    const bool pretty_print = os.width() > 0;
    const auto indentation = pretty_print ? os.width() : 0;

    os.width(0);

    SnbtSerializer serializer(OutputAdapter<char>(os), os.fill());
    serializer.dump(compound_tag, pretty_print, false, static_cast<unsigned int>(indentation));
    return os;
}

SHULKER_NBT_NAMESPACE_END
