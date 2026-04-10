#pragma once

#include <cstdint>
#include <string>
#include <limits>

#include "shulker/detail/MacroScope.h"
#include "shulker/detail/MacroCommon.h"
#include "shulker/CompressionMode.h"
#include "shulker/detail/io/output/OutputAdapters.h"
#include "shulker/detail/meta/CppFuture.h"
#include "shulker/detail/meta/Endian.h"
#include "shulker/tags/TagReference.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

class SHULKER_API NbtWriter
{
public:
    explicit NbtWriter(
        OutputAdapterPtr<char> oa,
        bool use_big_endian = true,
        CompressionMode compression_mode = CompressionMode::Uncompressed
    )
        : m_oa(std::move(oa)),
        m_big_endian(use_big_endian),
        m_compression_mode(compression_mode)
    {
        m_need_to_swap = is_little_endian() == use_big_endian;

        m_buffer.reserve(128 * 1024);
        m_buf_oa = OutputAdapter(m_buffer);
    }

    std::size_t write(
        const TagReference& tag_ref,
        const std::string& name = "");

private:
    template<typename T, typename = std::void_t<decltype(byteswap(std::declval<T>()))>>
    [[nodiscard]] T toTargetEndian(T value) const noexcept
    {
        if (m_need_to_swap) {
            return byteswap(value);
        }
        return value;
    }

    void writeRaw(
        const TagReference& tag_ref,
        const std::string& name);

    void writePayload(const TagReference& tag_ref);

    void writeByte(std::int8_t value);

    void writeShort(std::int16_t value);

    void writeInt(std::int32_t value);

    void writeLong(std::int64_t value);

    void writeString(const std::string& value);

    template<typename ArrayType, std::enable_if_t<
        is_array_tag<ArrayType>::value, int> = 0>
    void writeArray(const ArrayType& tag)
    {
        if (tag.m_value.empty()) {
            writeInt(0x00); // array size
        }

        if (tag.m_value.size() > std::numeric_limits<std::int32_t>::max()) {
            NBT_THROW(OutOfRangeError::create(302, "the number of elements in the tag exceeds the maximum limit of " +
                std::to_string(std::numeric_limits<std::int32_t>::max())));
        }

        writeInt(tag.m_value.size());
        for (const auto& elem : tag.m_value) {
            if constexpr (std::is_same_v<ArrayType, ByteArrayTag>) {
                writeByte(elem);
            } else if constexpr (std::is_same_v<ArrayType, IntArrayTag>) {
                writeInt(elem);
            } else {
                writeLong(elem);
            }
        }
    }

private:
    OutputAdapterPtr<char> m_oa = nullptr;
    OutputAdapterPtr<char> m_buf_oa = nullptr;

    bool m_big_endian;
    bool m_need_to_swap = false;

    CompressionMode m_compression_mode;

    std::vector<char> m_buffer{};
};

}

SHULKER_NBT_NAMESPACE_END
