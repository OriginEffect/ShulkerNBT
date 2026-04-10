#pragma once

#include <cstdint>

#include "shulker/detail/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

enum class CompressionMode : std::uint8_t
{
    Zlib, // Bedrock edition
    Gzip, // Java edition
    Raw,
    Uncompressed,

    Unknown = 0xff,
};

SHULKER_NBT_NAMESPACE_END
