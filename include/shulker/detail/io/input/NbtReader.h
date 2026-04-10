#pragma once

#include <cstdint>
#include <string>

#include "shulker/detail/MacroScope.h"
#include "shulker/detail/io/input/InputAdapters.h"
#include "shulker/Type.h"
#include "shulker/tags/TagReference.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

struct SHULKER_API NbtRecord
{
    std::string name;
    TagReference tag_ref;
};

class SHULKER_API NbtReader
{
public:
    explicit NbtReader(
        InputAdapterPtr<char> ia,
        bool useBigEndian = true,
        CompressionMode compression_type = CompressionMode::Unknown
    )
        : m_ia(std::move(ia)),
        m_big_endian(useBigEndian),
        m_compression_type(compression_type)
    {}

    NbtRecord read(TagType tag_type = TagType::Unknown);

    TagReference readRoot();

private:
    std::uint8_t readByte();

    std::int16_t readShort();

    std::int32_t readInt();

    std::int64_t readLong();

    float readFloat();

    double readDouble();

    std::string readString();

    TagReference readPayload(TagType type);

    TagReference readCompound();

    TagReference readList();

    TagReference readByteArray();

    TagReference readIntArray();

    TagReference readLongArray();

private:
    InputAdapterPtr<char> m_ia = nullptr;

    bool m_big_endian;

    CompressionMode m_compression_type;
};

}

SHULKER_NBT_NAMESPACE_END
