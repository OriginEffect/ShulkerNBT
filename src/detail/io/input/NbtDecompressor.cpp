#include <zlib.h>

#include "shulker/detail/io/input/NbtDecompressor.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

bool NbtDecompressor::isGzip(const unsigned char* buf, std::size_t len)
{
    return len >= 2 && buf[0] == 0x1f && buf[1] == 0x8b;
}

bool NbtDecompressor::isZlib(const unsigned char* buf, std::size_t len)
{
    if (len < 2) return false;

    auto cmf = buf[0];
    auto flg = buf[1];

    if ((cmf & 0x0f) != 8) return false;
    if (((cmf << 8) + flg) % 31 != 0) return false;

    return true;
}

CompressionMode NbtDecompressor::detect(const unsigned char* buf, std::size_t len)
{
    if (isGzip(buf, len)) {
        return CompressionMode::Gzip;
    }
    if (isZlib(buf, len)) {
        return CompressionMode::Zlib;
    }
    return CompressionMode::Unknown;
}

NbtDecompressor::Status NbtDecompressor::decompress(
    std::vector<char>& dest,
    std::size_t max_output)
{
    if (m_compression_type == CompressionMode::Uncompressed) {
        return Status::NoAction;
    }

    dest.clear();
    z_stream strm{};
    int window_bits = 0;
    switch (m_compression_type) {
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
            window_bits = MAX_WBITS + 32;
            break;
        }
    }

    int ret = inflateInit2(&strm, window_bits);
    if (ret != Z_OK) {
        return Status::InitFailure;
    }

    do {
        if (strm.avail_in == 0) {
            const std::size_t bytes_read = m_ia->getCharsTo(m_in_buffer.data(), m_in_buffer.size());

            if (bytes_read == 0) {
                inflateEnd(&strm);
                dest.clear();
                return Status::BufferError;
            }

            strm.next_in = reinterpret_cast<Bytef*>(m_in_buffer.data());
            strm.avail_in = static_cast<uInt>(bytes_read);
        }

        strm.next_out = reinterpret_cast<Bytef*>(m_out_buffer.data());
        strm.avail_out = static_cast<uInt>(m_out_buffer.size());

        ret = inflate(&strm, Z_NO_FLUSH);

        const std::size_t produced = m_out_buffer.size() - strm.avail_out;

        if (produced != 0) {
            if (produced > max_output - dest.size()) {
                inflateEnd(&strm);
                dest.clear();
                return Status::OutputExceedsLimit;
            }
            dest.insert(dest.end(), m_out_buffer.begin(), m_out_buffer.begin() + produced);
        }

        if (ret == Z_NEED_DICT || ret < 0) {
            inflateEnd(&strm);
            dest.clear();
            return Status::DecompressionFailure;
        }
    } while (ret != Z_STREAM_END);

    ret = inflateEnd(&strm);
    if (ret != Z_OK) {
        dest.clear();
        return Status::Failure;
    }
    return Status::Success;
}

}

SHULKER_NBT_NAMESPACE_END
