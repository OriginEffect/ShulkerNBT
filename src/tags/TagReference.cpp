#include <utility>

#include "shulker/common/StringUtils.h"
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

const char* TagReference::getTypeName() const noexcept
{
    switch (getType()) {
        case TagType::End:
            return "end tag";
        case TagType::Byte:
            return "byte tag";
        case TagType::Short:
            return "short tag";
        case TagType::Int:
            return "int tag";
        case TagType::Long:
            return "long tag";
        case TagType::Float:
            return "float tag";
        case TagType::Double:
            return "double tag";
        case TagType::ByteArray:
            return "byte array tag";
        case TagType::String:
            return "string tag";
        case TagType::List:
            return "list tag";
        case TagType::Compound:
            return "compound tag";
        case TagType::IntArray:
            return "int array tag";
        case TagType::LongArray:
            return "long array tag";
        case TagType::Unknown:
        default:
            return "unknown tag";
    }
}

TagReference& TagReference::operator[](std::size_t index)
{
    if (isListTag()) {
        return std::get<ListTag>(m_tag_storage).operator[](index);
    }
    NBT_THROW(TypeError::create(102, concat("cannot use operator[] with a numeric argument with ", getTypeName())));
}

const TagReference& TagReference::operator[](std::size_t index) const
{
    if (isListTag()) {
        return std::get<ListTag>(m_tag_storage).operator[](index);
    }
    NBT_THROW(TypeError::create(102, concat("cannot use operator[] with a numeric argument with ", getTypeName())));
}

TagReference& TagReference::operator[](CompoundTag::CompoundType::key_type key) {
    if (isCompoundTag()) {
        return std::get<CompoundTag>(m_tag_storage).operator[](std::move(key));
    }
    NBT_THROW(TypeError::create(103, concat("cannot use operator[] with a string argument with ", getTypeName())));
}

const TagReference& TagReference::operator[](const CompoundTag::CompoundType::key_type& key) const {

}

SHULKER_NBT_NAMESPACE_END
