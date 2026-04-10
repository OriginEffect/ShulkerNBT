#include <cstring>

#include "shulker/detail/Exceptions.h"
#include "shulker/detail/MacroCommon.h"
#include "shulker/detail/io/input/NbtDecompressor.h"
#include "shulker/detail/io/input/NbtReader.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

NbtRecord NbtReader::read(TagType tag_type)
{
    unsigned char first_byte = m_ia->peek();
    std::vector<char> nbt_data{};

    if (m_compression_type == CompressionMode::Zlib ||
        m_compression_type == CompressionMode::Gzip ||
        m_compression_type == CompressionMode::Raw ||
        (m_compression_type == CompressionMode::Unknown && first_byte > 0x0c))
    {
        NbtDecompressor dc(m_ia, m_compression_type);

        NbtDecompressor::Status ret = dc.decompress(nbt_data);
        if (ret != NbtDecompressor::Status::Success) {
            NBT_THROW(ParseError::create(203, "decompression failed, decompressor status: " + std::to_string(static_cast<std::uint8_t>(ret))));
        }
        m_ia = std::move(InputAdapter(nbt_data.cbegin(), nbt_data.cend()));
    }

    auto tag_id = readByte();

    if (tag_id > 0x0c) {
        NBT_THROW(ParseError::create(201, "unknown tag type with id: " + std::to_string(tag_id)));
    }
    if (tag_type != TagType::Unknown && static_cast<TagType>(tag_id) != tag_type) {
        NBT_THROW(ParseError::create(202,
            "unexpected root type with id: " + std::to_string(tag_id) + ", expected: " + getTypeName(tag_type)));
    }

    return { readString(), readPayload(static_cast<TagType>(tag_id)) };
}

TagReference NbtReader::readRoot()
{
    return read(TagType::Compound).tag_ref;
}

std::uint8_t NbtReader::readByte()
{
    auto ch = m_ia->getChar();
    if (ch == std::char_traits<char>::eof()) {
        NBT_THROW(ParseError::create(203, "unexpected end of NBT data"));
    }
    return static_cast<std::uint8_t>(ch);
}

std::int16_t NbtReader::readShort()
{
    std::uint8_t buf[2];
    buf[0] = readByte();
    buf[1] = readByte();
    if (m_big_endian) {
        return static_cast<std::int16_t>(
            (static_cast<std::uint16_t>(buf[0]) << 8u) |
            (static_cast<std::uint16_t>(buf[1])));
    } else {
        return static_cast<std::int16_t>(
            (static_cast<std::uint16_t>(buf[0])) |
            (static_cast<std::uint16_t>(buf[1]) << 8u));
    }
}

std::int32_t NbtReader::readInt()
{
    std::uint8_t buf[4];
    for (auto& b : buf) b = readByte();
    if (m_big_endian) {
        return static_cast<std::int32_t>(
            (static_cast<std::uint32_t>(buf[0]) << 24u) |
            (static_cast<std::uint32_t>(buf[1]) << 16u) |
            (static_cast<std::uint32_t>(buf[2]) << 8u)  |
            (static_cast<std::uint32_t>(buf[3])));
    } else {
        return static_cast<std::int32_t>(
            (static_cast<std::uint32_t>(buf[0])) |
            (static_cast<std::uint32_t>(buf[1]) << 8u)  |
            (static_cast<std::uint32_t>(buf[2]) << 16u) |
            (static_cast<std::uint32_t>(buf[3]) << 24u));
    }
}

std::int64_t NbtReader::readLong()
{
    std::uint8_t buf[8];
    for (auto& b : buf) b = readByte();
    if (m_big_endian) {
        return static_cast<std::int64_t>(
            (static_cast<std::uint64_t>(buf[0]) << 56u) |
            (static_cast<std::uint64_t>(buf[1]) << 48u) |
            (static_cast<std::uint64_t>(buf[2]) << 40u) |
            (static_cast<std::uint64_t>(buf[3]) << 32u) |
            (static_cast<std::uint64_t>(buf[4]) << 24u) |
            (static_cast<std::uint64_t>(buf[5]) << 16u) |
            (static_cast<std::uint64_t>(buf[6]) << 8u)  |
            (static_cast<std::uint64_t>(buf[7])));
    } else {
        return static_cast<std::int64_t>(
            (static_cast<std::uint64_t>(buf[0])) |
            (static_cast<std::uint64_t>(buf[1]) << 8u)  |
            (static_cast<std::uint64_t>(buf[2]) << 16u) |
            (static_cast<std::uint64_t>(buf[3]) << 24u) |
            (static_cast<std::uint64_t>(buf[4]) << 32u) |
            (static_cast<std::uint64_t>(buf[5]) << 40u) |
            (static_cast<std::uint64_t>(buf[6]) << 48u) |
            (static_cast<std::uint64_t>(buf[7]) << 56u));
    }
}

float NbtReader::readFloat()
{
    std::int32_t bits = readInt();
    float result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

double NbtReader::readDouble()
{
    std::int64_t bits = readLong();
    double result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

std::string NbtReader::readString()
{
    if (m_big_endian) { // je: unsigned short
        auto length = static_cast<std::uint16_t>(readShort());
        std::string s(length, '\0');
        for (std::uint16_t i = 0; i < length; ++i) {
            s[i] = static_cast<char>(readByte());
        }
        return s;
    } else { // be: signed short
        auto length = readShort();
        std::string s(length, '\0');
        for (std::int16_t i = 0; i < length; ++i) {
            s[i] = static_cast<char>(readByte());
        }
        return s;
    }
}

TagReference NbtReader::readPayload(TagType type)
{
    switch (type) {
        case TagType::End:
            return TagReference(std::in_place_type<EndTag>);
        case TagType::Byte:
            return TagReference(std::in_place_type<ByteTag>, static_cast<std::int8_t>(readByte()));
        case TagType::Short:
            return TagReference(std::in_place_type<ShortTag>, readShort());
        case TagType::Int:
            return TagReference(std::in_place_type<IntTag>, readInt());
        case TagType::Long:
            return TagReference(std::in_place_type<LongTag>, readLong());
        case TagType::Float:
            return TagReference(std::in_place_type<FloatTag>, readFloat());
        case TagType::Double:
            return TagReference(std::in_place_type<DoubleTag>, readDouble());
        case TagType::ByteArray:
            return readByteArray();
        case TagType::String:
            return TagReference(std::in_place_type<StringTag>, readString());
        case TagType::List:
            return readList();
        case TagType::Compound:
            return readCompound();
        case TagType::IntArray:
            return readIntArray();
        case TagType::LongArray:
            return readLongArray();
    default:
            NBT_THROW(ParseError::create(203, "unknown tag type with id: " + std::to_string(static_cast<TagId>(type))));
    }
}

TagReference NbtReader::readCompound()
{
    CompoundTag::CompoundType map;

    while (true) {
        auto type_id = readByte();
        auto type = static_cast<TagType>(type_id);

        if (type == TagType::End) {
            break;
        }

        auto name = readString();
        auto value = readPayload(type);
        map.emplace(std::move(name), std::move(value));
    }

    return TagReference(std::in_place_type<CompoundTag>, std::move(map));
}

TagReference NbtReader::readList()
{
    auto element_type = static_cast<TagType>(readByte());
    auto length = readInt();

    if (length <= 0) {
        return TagReference(std::in_place_type<ListTag>);
    }

    ListTag::ListType elements;
    elements.reserve(static_cast<std::size_t>(length));

    for (std::int32_t i = 0; i < length; ++i) {
        elements.push_back(readPayload(element_type));
    }

    return TagReference(std::in_place_type<ListTag>, std::move(elements));
}

TagReference NbtReader::readByteArray()
{
    auto length = readInt();
    ByteArrayTag tag;
    tag.m_value.resize(static_cast<std::size_t>(length));
    for (std::int32_t i = 0; i < length; ++i) {
        tag.m_value[i] = static_cast<std::int8_t>(readByte());
    }
    return TagReference(std::move(tag));
}

TagReference NbtReader::readIntArray()
{
    auto length = readInt();
    IntArrayTag tag;
    tag.m_value.resize(static_cast<std::size_t>(length));
    for (std::int32_t i = 0; i < length; ++i) {
        tag.m_value[i] = readInt();
    }
    return TagReference(std::move(tag));
}

TagReference NbtReader::readLongArray()
{
    auto length = readInt();
    LongArrayTag tag;
    tag.m_value.resize(static_cast<std::size_t>(length));
    for (std::int32_t i = 0; i < length; ++i) {
        tag.m_value[i] = readLong();
    }
    return TagReference(std::move(tag));
}

}

SHULKER_NBT_NAMESPACE_END
