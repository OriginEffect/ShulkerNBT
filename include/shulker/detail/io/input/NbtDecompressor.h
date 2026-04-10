#pragma once

#include <cstdint>
#include <vector>

#include "shulker/detail/MacroScope.h"

#include "shulker/detail/io/input/InputAdapters.h"
#include "../../../CompressionMode.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

class NbtDecompressor
{
public:
    enum class Status : std::uint8_t
    {
        Success,
        NoAction,

        Failure,
        InitFailure,
        DecompressionFailure,
        BufferError,
        OutputExceedsLimit
    };

    NbtDecompressor(
        InputAdapterPtr<char> ia,
        CompressionMode compression_type = CompressionMode::Unknown)
        : m_ia(std::move(ia)), m_compression_type(compression_type)
    {}

    NbtDecompressor(const NbtDecompressor &) = delete;
    NbtDecompressor& operator=(const NbtDecompressor&) = delete;
    NbtDecompressor(NbtDecompressor &&) = delete;
    NbtDecompressor& operator=(NbtDecompressor&&) = delete;

    ~NbtDecompressor() = default;

    [[nodiscard]] static bool isGzip(const unsigned char* buf, std::size_t len);

    [[nodiscard]] static bool isZlib(const unsigned char* buf, std::size_t len);

    [[nodiscard]] static CompressionMode detect(const unsigned char* buf, std::size_t len);

    // TODO: 目前设定默认解压数据超过64MB就拒绝解析，后续可能添加流式解析接口
    Status decompress(
        std::vector<char>& dest,
        std::size_t max_output = 64 * 1024 * 1024);

private:
    InputAdapterPtr<char> m_ia = nullptr;

    CompressionMode m_compression_type;

    std::array<char, 128 * 1024> m_in_buffer{{}};
    std::array<char, 128 * 1024> m_out_buffer{{}};
};

}

SHULKER_NBT_NAMESPACE_END
