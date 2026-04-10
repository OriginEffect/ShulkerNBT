#include <utility>

#include "../../include/shulker/detail/StringUtils.h"
#include "shulker/detail/io/output/OutputAdapters.h"
#include "shulker/detail/io/output/SnbtSerializer.h"
#include "shulker/tags/TagReference.h"

SHULKER_NBT_NAMESPACE_BEGIN

TagReference::TagReference(std::initializer_list<TagReference> init)
{
    bool is_compound = std::all_of(init.begin(), init.end(),
        [](const TagReference& tag_ref) {
            return tag_ref.isListTag() &&
                tag_ref.as<ListTag>().size() == 2 &&
                tag_ref.as<ListTag>()[static_cast<std::size_t>(0)].isStringTag();
        });

    if (is_compound) {
        CompoundTag compound_tag{};
        for (auto& tag_ref : init) {
            compound_tag.m_value.emplace(
                std::move(tag_ref.as<ListTag>()[0].as<StringTag>().get()),
                tag_ref.as<ListTag>()[1]);
        }
        m_tag_storage = compound_tag;
    } else {
        m_tag_storage = ListTag(init);
    }
}

BasicTag& TagReference::getTag()
{
    return std::visit([](auto& tag) -> BasicTag& {
        return static_cast<BasicTag&>(tag);
    }, m_tag_storage);
}

const BasicTag& TagReference::getTag() const
{
    return std::visit([](const auto& tag) -> const BasicTag& {
        return static_cast<const BasicTag&>(tag);
    }, m_tag_storage);
}

std::string TagReference::getTypeName() const noexcept
{
    return ::shulker::getTypeName(getType());
}

TagReference& TagReference::operator[](std::size_t index)
{
    if (isListTag()) {
        return std::get<ListTag>(m_tag_storage).operator[](index);
    }
    NBT_THROW(detail::TypeError::create(102, detail::concat("cannot use operator[] with a numeric argument with ", getTypeName())));
}

const TagReference& TagReference::operator[](std::size_t index) const
{
    if (isListTag()) {
        return std::get<ListTag>(m_tag_storage).operator[](index);
    }
    NBT_THROW(detail::TypeError::create(102, detail::concat("cannot use operator[] with a numeric argument with ", getTypeName())));
}

TagReference& TagReference::operator[](CompoundTag::CompoundType::key_type key) {
    if (isCompoundTag()) {
        return std::get<CompoundTag>(m_tag_storage).operator[](std::move(key));
    }
    NBT_THROW(detail::TypeError::create(103, detail::concat("cannot use operator[] with a string argument with ", getTypeName())));
}

const TagReference& TagReference::operator[](const CompoundTag::CompoundType::key_type& key) const {

}

std::ostream& operator<<(std::ostream& os, const TagReference& tag_ref)
{
    return std::visit([&os](auto& tag) -> std::ostream& {
        return os << tag;
    }, tag_ref.m_tag_storage);
}

SHULKER_NBT_NAMESPACE_END
