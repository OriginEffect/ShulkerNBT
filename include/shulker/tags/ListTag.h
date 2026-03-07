#pragma once

#include <vector>

#include "shulker/common/MacroScope.h"

#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class TagReference;

class SHULKER_API ListTag : public BasicTag
{
    SHULKER_NBT_FRIEND_CLASS(SnbtSerializer);

public:
    using ListType = std::vector<TagReference>;

    ListTag();
    ~ListTag() override;
    ListTag(const ListTag& other);
    ListTag(ListTag&& other) noexcept;
    ListTag& operator=(const ListTag& other);
    ListTag& operator=(ListTag&& other) noexcept;

    ListTag(const ListType& vec);
    ListTag(ListType&& vec);

    ListTag(std::initializer_list<TagReference> init);

    template<typename InputIt>
    ListTag(InputIt first, InputIt last) : m_value(first, last) {}

    [[nodiscard]] static TagType type() noexcept { return TagType::List; }

    [[nodiscard]] std::size_t size() const noexcept;

    TagReference& operator[](std::size_t index);

    const TagReference& operator[](std::size_t index) const;

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const ListTag& list_tag);

private:
    ListType m_value{};
};

SHULKER_NBT_NAMESPACE_END
