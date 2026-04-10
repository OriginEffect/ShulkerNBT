#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "shulker/detail/Exceptions.h"
#include "shulker/detail/MacroCommon.h"
#include "shulker/detail/MacroScope.h"
#include "shulker/detail/io/output/OutputAdapters.h"
#include "../../../CompressionMode.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

class NbtCompressor
{
public:
    enum class Status : std::uint8_t
    {
        Success,
        NoAction,

        Failure,
        InitFailure,
    };

    NbtCompressor(OutputAdapterPtr<char> oa, CompressionMode compression_mode)
        : m_oa(std::move(oa))
    {
        if (compression_mode == CompressionMode::Unknown) {
            NBT_THROW(OtherError::create(401, "compression mode cannot be \"CompressionMode::Unknown\""));
        }
        m_compression_mode = compression_mode;
    }

    Status compress(std::vector<char>& src);

private:
    OutputAdapterPtr<char> m_oa = nullptr;

    CompressionMode m_compression_mode;

    std::array<char, 128 * 1024> m_buffer{{}};
};

}

SHULKER_NBT_NAMESPACE_END
