#pragma once

#include <variant>

#include "shulker/common/Exceptions.h"
#include "shulker/common/MacroScope.h"
#include "shulker/meta/TypeTraits.h"

#include "shulker/tags/ByteArrayTag.h"
#include "shulker/tags/ByteTag.h"
#include "shulker/tags/CompoundTag.h"
#include "shulker/tags/DoubleTag.h"
#include "shulker/tags/EndTag.h"
#include "shulker/tags/FloatTag.h"
#include "shulker/tags/IntArrayTag.h"
#include "shulker/tags/IntTag.h"
#include "shulker/tags/ListTag.h"
#include "shulker/tags/LongArrayTag.h"
#include "shulker/tags/LongTag.h"
#include "shulker/tags/ShortTag.h"
#include "shulker/tags/StringTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

class SHULKER_API TagReference {
public:
    using TagVariant = std::variant<
        EndTag,
        ByteTag,
        ShortTag,
        IntTag,
        LongTag,
        FloatTag,
        DoubleTag,
        ByteArrayTag,
        StringTag,
        ListTag,
        CompoundTag,
        IntArrayTag,
        LongArrayTag>;

    template<typename TagType, std::enable_if_t<
        std::is_base_of_v<BasicTag, TagType> && !std::is_same_v<BasicTag, TagType>, int> = 0>
    TagReference(TagType tag) : m_tag_storage(std::move(tag)) {}

    template<typename TagType, typename... Args>
    TagReference(std::in_place_type_t<TagType>, Args&&... args)
    : m_tag_storage(std::in_place_type<TagType>, std::forward<Args>(args)...) {}

    template<typename ValueType, std::enable_if_t<
        std::is_integral_v<ValueType>, int> = 0>
    TagReference(ValueType integer) : m_tag_storage(tag_from_value_size_t<ValueType>{integer}) {}

    TagReference(FloatTag::FloatType f) : m_tag_storage(FloatTag{f}) {}

    TagReference(DoubleTag::DoubleType d) : m_tag_storage(DoubleTag{d}) {}

    // 先暂时用const char*避免"xxx"无法构造，后面再改成通用的
    TagReference(const char* ch) : m_tag_storage(std::in_place_type<StringTag>, ch) {}
    TagReference(StringTag::StringType s) : m_tag_storage(std::in_place_type<StringTag>, std::move(s)) {}

    TagReference(std::nullptr_t = nullptr) : m_tag_storage(CompoundTag{}) {}
    TagReference(const CompoundTag::CompoundType& c) : m_tag_storage(CompoundTag{c}) {}
    // using InitType = std::initializer_list<std::pair<CompoundTag::CompoundType::key_type, TagReference>>;
    // TagReference(InitType init)
    //     : m_tag_storage(std::in_place_type<CompoundTag>, init) {}

    TagReference(const ListTag::ListType& l) : m_tag_storage(ListTag{l}) {}

    TagReference(std::initializer_list<TagReference> init);

    [[nodiscard]] BasicTag& getTag();

    [[nodiscard]] const BasicTag& getTag() const;

    template<typename TagType>
    [[nodiscard]] TagType& as()
    {
        auto* tag_p = std::get_if<TagType>(&m_tag_storage);
        if (!tag_p) {
            NBT_THROW(TypeError::create(101, "the type held by the tag reference is inconsistent with the expected type"));
        }
        return *tag_p;
    }

    template<typename TagType>
    [[nodiscard]] const TagType& as() const
    {
        auto* tag_p = std::get_if<TagType>(&m_tag_storage);
        if (!tag_p) {
            NBT_THROW(TypeError::create(101, "the type held by the tag reference is inconsistent with the expected type"));
        }
        return *tag_p;
    }

    [[nodiscard]] bool isEndTag() const { return std::holds_alternative<EndTag>(m_tag_storage); }
    [[nodiscard]] bool isByteTag() const { return std::holds_alternative<ByteTag>(m_tag_storage); }
    [[nodiscard]] bool isShortTag() const { return std::holds_alternative<ShortTag>(m_tag_storage); }
    [[nodiscard]] bool isIntTag() const { return std::holds_alternative<IntTag>(m_tag_storage); }
    [[nodiscard]] bool isLongTag() const { return std::holds_alternative<LongTag>(m_tag_storage); }
    [[nodiscard]] bool isFloatTag() const { return std::holds_alternative<FloatTag>(m_tag_storage); }
    [[nodiscard]] bool isDoubleTag() const { return std::holds_alternative<DoubleTag>(m_tag_storage); }
    [[nodiscard]] bool isByteArrayTag() const { return std::holds_alternative<ByteArrayTag>(m_tag_storage); }
    [[nodiscard]] bool isStringTag() const { return std::holds_alternative<StringTag>(m_tag_storage); }
    [[nodiscard]] bool isListTag() const { return std::holds_alternative<ListTag>(m_tag_storage); }
    [[nodiscard]] bool isCompoundTag() const { return std::holds_alternative<CompoundTag>(m_tag_storage); }
    [[nodiscard]] bool isIntArrayTag() const { return std::holds_alternative<IntArrayTag>(m_tag_storage); }
    [[nodiscard]] bool isLongArrayTag() const { return std::holds_alternative<LongArrayTag>(m_tag_storage); }

    [[nodiscard]] TagType getType() const noexcept { return static_cast<TagType>(m_tag_storage.index()); }

    [[nodiscard]] const char* getTypeName() const noexcept;

    [[nodiscard]] TagId getId() const noexcept { return m_tag_storage.index(); }

    [[nodiscard]] bool hold(const TagType type) const noexcept { return getType() == type; }

    TagReference& operator[](std::size_t index);

    const TagReference& operator[](std::size_t index) const;

    TagReference& operator[](CompoundTag::CompoundType::key_type key);

    const TagReference& operator[](const CompoundTag::CompoundType::key_type& key) const;

    template<typename T>
    TagReference& operator[](T* key)
    {
        return operator[](CompoundTag::CompoundType::key_type(key));
    }

    template<typename T>
    const TagReference& operator[](T* key) const
    {
        return operator[](CompoundTag::CompoundType::key_type(key));
    }

    SHULKER_API friend std::ostream& operator<<(std::ostream& os, const TagReference& tag_ref);

private:
    TagVariant m_tag_storage;
};

SHULKER_NBT_NAMESPACE_END
