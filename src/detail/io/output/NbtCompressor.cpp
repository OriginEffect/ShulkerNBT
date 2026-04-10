#include <zlib.h>

#include "shulker/detail/io/output/NbtCompressor.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

NbtCompressor::Status NbtCompressor::compress(std::vector<char>& src)
{
    if (m_compression_mode == CompressionMode::Uncompressed) {
        return Status::NoAction;
    }

    z_stream strm{};
    int window_bits = 0;
    switch (m_compression_mode) {
        case CompressionMode::Gzip: {
            window_bits = MAX_WBITS + 16;
            break;
        }
        case CompressionMode::Zlib: {
            window_bits = MAX_WBITS;
            break;
        }
        case CompressionMode::Raw: {
            window_bits = -MAX_WBITS;
            break;
        }
        case CompressionMode::Unknown:
        default: {
            NBT_ASSERT(false);
        }
    }

    int ret = deflateInit2(
        &strm,
        Z_DEFAULT_COMPRESSION,
        Z_DEFLATED,
        window_bits,
        8,
        Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        return Status::InitFailure;
    }

    strm.next_in = reinterpret_cast<Bytef*>(src.data());
    strm.avail_in = static_cast<uInt>(src.size());
    strm.next_out = reinterpret_cast<Bytef*>(m_buffer.data());
    strm.avail_out = static_cast<uInt>(m_buffer.size());

    do {
        if (strm.avail_out == 0) {
            m_oa->writeChars(m_buffer.data(), m_buffer.size());
            strm.next_out = reinterpret_cast<Bytef*>(m_buffer.data());
            strm.avail_out = static_cast<uInt>(m_buffer.size());
        }

        ret = deflate(&strm, strm.avail_in == 0 ? Z_NO_FLUSH : Z_FINISH);
        if (!(ret == Z_OK || ret == Z_STREAM_END)) {
            deflateEnd(&strm);
            return Status::Failure;
        }
    } while (ret != Z_STREAM_END);

    m_oa->writeChars(m_buffer.data(), m_buffer.size() - strm.avail_out);
    deflateEnd(&strm);
    return Status::Success;
}

}

SHULKER_NBT_NAMESPACE_END
