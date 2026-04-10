#include "shulker/detail/Exceptions.h"
#include "shulker/detail/io/output/NbtCompressor.h"
#include "shulker/detail/io/output/NbtWriter.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

std::size_t NbtWriter::write(
    const TagReference& tag_ref,
    const std::string& name)
{
    writeRaw(tag_ref, name);

    if (m_compression_mode == CompressionMode::Uncompressed) {
        m_oa->writeChars(m_buffer.data(), m_buffer.size());
        return m_buffer.size();
    }

    NbtCompressor c(m_oa, m_compression_mode);
    c.compress(m_buffer);
    return m_buffer.size();
}

void NbtWriter::writeRaw(
    const TagReference& tag_ref,
    const std::string& name)
{
    writeByte(static_cast<std::int8_t>(tag_ref.getId()));
    writeString(name);
    writePayload(tag_ref);
}

void NbtWriter::writePayload(const TagReference& tag_ref)
{
    switch (tag_ref.getType()) {
        case TagType::Compound: {
            auto& tag = tag_ref.as<CompoundTag>();

            if (tag.m_value.empty()) {
                writeByte(0x00);
                return;
            }

            for (const auto& [name, payload] : tag.m_value) {
                writeRaw(payload, name);
            }
            writeByte(0x00);
            return;
        }
        case TagType::List: {
            auto& tag = tag_ref.as<ListTag>();

            if (tag.m_value.empty()) {
                writeByte(0x00); // default element id
                writeInt(0x00); // list size
                return;
            }

            if (tag.m_value.size() > std::numeric_limits<std::int32_t>::max()) {
                NBT_THROW(OutOfRangeError::create(302, "the number of elements in the tag exceeds the maximum limit of " +
                    std::to_string(std::numeric_limits<std::int32_t>::max())));
            }

            writeByte(static_cast<std::int8_t>(getTagId(tag.elementType())));
            writeInt(static_cast<std::int32_t>(tag.m_value.size()));
            if (tag.isHeterogeneous()) {
                for (const auto& elem : tag.m_value) {
                    if (elem.getType() == TagType::Compound) {
                        writePayload(elem);
                    } else {
                        writeRaw(elem, "");
                        writeByte(0x00);
                    }
                }
            } else {
                for (const auto& elem : tag.m_value) {
                    writePayload(elem);
                }
            }
            return;
        }
        case TagType::ByteArray: {
            auto& tag = tag_ref.as<ByteArrayTag>();
            writeArray(tag);
            return;
        }
        case TagType::IntArray: {
            auto& tag = tag_ref.as<IntArrayTag>();
            writeArray(tag);
            return;
        }
        case TagType::LongArray: {
            auto& tag = tag_ref.as<LongArrayTag>();
            writeArray(tag);
            return;
        }
        case TagType::String: {
            auto& tag = tag_ref.as<StringTag>();
            writeString(tag.m_value);
            return;
        }
        case TagType::Float: {
            auto& tag = tag_ref.as<FloatTag>();
            writeInt(bit_cast<std::int32_t>(tag.m_value));
            return;
        }
        case TagType::Double: {
            auto& tag = tag_ref.as<DoubleTag>();
            writeLong(bit_cast<std::int64_t>(tag.m_value));
            return;
        }
        case TagType::Byte: {
            auto& tag = tag_ref.as<ByteTag>();
            writeByte(tag.m_value);
            return;
        }
        case TagType::Short: {
            auto& tag = tag_ref.as<ShortTag>();
            writeShort(tag.m_value);
            return;
        }
        case TagType::Int: {
            auto& tag = tag_ref.as<IntTag>();
            writeInt(tag.m_value);
            return;
        }
        case TagType::Long: {
            auto& tag = tag_ref.as<LongTag>();
            writeLong(tag.m_value);
            return;
        }
        case TagType::End: {
            writeByte(0x00);
            return;
        }
        case TagType::Unknown: {
            return;
        }
        default:
            NBT_ASSERT(false);
    }
}

void NbtWriter::writeByte(std::int8_t value)
{
    m_buf_oa->writeChar(value);
}

void NbtWriter::writeShort(std::int16_t value)
{
    auto bytes = toTargetEndian(value);
    m_buf_oa->writeChars(reinterpret_cast<char*>(&bytes), 2);
}

void NbtWriter::writeInt(std::int32_t value)
{
    auto bytes = toTargetEndian(value);
    m_buf_oa->writeChars(reinterpret_cast<char*>(&bytes), 4);
}

void NbtWriter::writeLong(std::int64_t value)
{
    auto bytes = toTargetEndian(value);
    m_buf_oa->writeChars(reinterpret_cast<char*>(&bytes), 8);
}

void NbtWriter::writeString(const std::string& value)
{
    if (!value.empty()) {
        if (m_big_endian) { // je: unsigned short
            if (value.size() > std::numeric_limits<std::uint16_t>::max()) {
                NBT_THROW(OutOfRangeError::create(301, "the string exceeds the maximum size of " +
                    std::to_string(std::numeric_limits<std::uint16_t>::max()) + " bytes"));
            }
        } else { // be: signed short
            if (value.size() > std::numeric_limits<std::int16_t>::max()) {
                NBT_THROW(OutOfRangeError::create(301, "the string exceeds the maximum size of " +
                    std::to_string(std::numeric_limits<std::int16_t>::max()) + " bytes"));
            }
        }
        writeShort(static_cast<std::int16_t>(value.size()));
        m_buf_oa->writeChars(value.data(), value.size());
    } else {
        writeShort(0x00);
    }
}

}

SHULKER_NBT_NAMESPACE_END
