#pragma once

#include <map>
#include <memory>

#include "shulker/detail/MacroScope.h"

#include "shulker/tags/BasicTag.h"
#include "shulker/Type.h"
#include "shulker/CompressionMode.h"

SHULKER_NBT_NAMESPACE_BEGIN
    class TagReference;

class SHULKER_API CompoundTag : public BasicTag
{
private:
    SHULKER_NBT_FRIEND_CLASS(TagReference);
    SHULKER_NBT_FRIEND_CLASS(detail::NbtWriter)
    SHULKER_NBT_FRIEND_CLASS(detail::SnbtSerializer);

public:
    using CompoundType = std::map<std::string, TagReference, std::less<>>;
    using InitType = std::initializer_list<std::pair<CompoundType::key_type, TagReference>>;

    CompoundTag();

    CompoundTag(const CompoundType& map);
    CompoundTag(CompoundType&& map);

    CompoundTag(InitType init);

    [[nodiscard]] static TagType type() noexcept { return TagType::Compound; }

    [[nodiscard]] static TagId id() noexcept { return 0x0a; }

    [[nodiscard]] std::vector<char> dumpBinary(
        bool use_big_endian = true,
        CompressionMode compression_mode = CompressionMode::Uncompressed);

    std::size_t dumpFile(
        const std::string& path,
        bool use_big_endian = true,
        CompressionMode compression_mode = CompressionMode::Uncompressed);

    TagReference& operator[](CompoundType::key_type key);

    // const TagReference& operator[](const CompoundType::key_type& key) const;

    template<typename T>
    TagReference& operator[](T* key)
    {
        return operator[](CompoundType::key_type(key));
    }

    // template<typename T>
    // const TagReference& operator[](T* key) const
    // {
    //     return operator[](CompoundType::key_type(key));
    // }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const CompoundTag& compound_tag);

private:
    CompoundType m_value{};
};

SHULKER_NBT_NAMESPACE_END
