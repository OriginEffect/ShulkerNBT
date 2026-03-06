#pragma once

#include <map>
#include <memory>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/Type.h"

SHULKER_NBT_NAMESPACE_BEGIN

class TagReference;

class SHULKER_API CompoundTag : public BasicTag
{
private:
    SHULKER_NBT_FRIEND_CLASS(TagReference);
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using CompoundType = std::map<std::string, TagReference, std::less<>>;
    using InitType = std::initializer_list<std::pair<CompoundType::key_type, TagReference>>;

    CompoundTag();

    CompoundTag(const CompoundType& map);
    CompoundTag(CompoundType&& map);

    CompoundTag(InitType init);

    [[nodiscard]] static TagType type() { return TagType::Compound; }

    TagReference& operator[](CompoundType::key_type key);

    const TagReference& operator[](const CompoundType::key_type& key) const;

    template<typename T>
    TagReference& operator[](T* key)
    {
        return operator[](CompoundType::key_type(key));
    }

    template<typename T>
    const TagReference& operator[](T* key) const
    {
        return operator[](CompoundType::key_type(key));
    }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const CompoundTag& compound_tag);

private:
    CompoundType m_value{};
};

SHULKER_NBT_NAMESPACE_END
