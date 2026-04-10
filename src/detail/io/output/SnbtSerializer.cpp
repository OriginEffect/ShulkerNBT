#include <cstdint>

#include "shulker/detail/Exceptions.h"
#include "shulker/detail/StringUtils.h"
#include "shulker/detail/io/output/SnbtSerializer.h"
#include "shulker/tags/CompoundTag.h"
#include "shulker/tags/TagReference.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

void SnbtSerializer::dump(
    const BasicTag& tag,
    const bool pretty_print,
    const bool keep_bool_type,
    const bool ensure_ascii,
    const unsigned int indent_level,
    const unsigned int current_indent)
{
    switch (tag.m_value_type) {
        case TagValue::Compound: {
            const auto& tag_t = dynamic_cast<const CompoundTag&>(tag);

            if (tag_t.m_value.empty()) {
                m_oa->writeChars("{}", 2);
                return;
            }

            if (pretty_print) {
                m_oa->writeChars("{\n", 2);

                const auto new_indent = current_indent + indent_level;
                if (m_indent_string.size() < new_indent) {
                    m_indent_string.resize(m_indent_string.size() * 2, ' ');
                }

                // first n-1 elements
                auto i = tag_t.m_value.cbegin();
                for (std::size_t cnt = 0; cnt < tag_t.m_value.size() - 1; ++cnt, ++i) {
                    m_oa->writeChars(m_indent_string.c_str(), new_indent);
                    dumpKey(i->first, ensure_ascii);
                    m_oa->writeChars(": ", 2);
                    dump(i->second.getTag(), true, keep_bool_type, ensure_ascii, indent_level, new_indent);
                    m_oa->writeChars(",\n", 2);
                }

                // last element (no comma at the end)
                NBT_ASSERT(i != tag_t.m_value.cend());
                NBT_ASSERT(std::next(i) == tag_t.m_value.cend());
                m_oa->writeChars(m_indent_string.c_str(), new_indent);
                dumpKey(i->first, ensure_ascii);
                m_oa->writeChars(": ", 2);
                dump(i->second.getTag(), true, keep_bool_type, ensure_ascii, indent_level, new_indent);

                m_oa->writeChar('\n');
                m_oa->writeChars(m_indent_string.c_str(), current_indent);
                m_oa->writeChar('}');
            } else {
                m_oa->writeChar('{');

                // first n-1 elements
                auto i = tag_t.m_value.cbegin();
                for (std::size_t cnt = 0; cnt < tag_t.m_value.size() - 1; ++cnt, ++i) {
                    dumpKey(i->first, ensure_ascii);
                    m_oa->writeChar(':');
                    dump(i->second.getTag(), false, keep_bool_type, ensure_ascii, indent_level, current_indent);
                    m_oa->writeChar(',');
                }

                // last element (no comma at the end)
                NBT_ASSERT(i != tag_t.m_value.cend());
                NBT_ASSERT(std::next(i) == tag_t.m_value.cend());
                dumpKey(i->first, ensure_ascii);
                m_oa->writeChar(':');
                dump(i->second.getTag(), false, keep_bool_type, ensure_ascii, indent_level, current_indent);

                m_oa->writeChar('}');
            }
            return;
        }
        case TagValue::List: {
            const auto& tag_t = dynamic_cast<const ListTag&>(tag);

            if (tag_t.m_value.empty()) {
                m_oa->writeChars("[]", 2);
                return;
            }

            if (pretty_print) {
                m_oa->writeChars("[\n", 2);

                const auto new_indent = current_indent + indent_level;
                if (m_indent_string.size() < new_indent) {
                    m_indent_string.resize(m_indent_string.size() * 2, ' ');
                }

                // first n-1 elements
                for (auto i = tag_t.m_value.cbegin(); i != tag_t.m_value.cend() - 1; ++i) {
                    m_oa->writeChars(m_indent_string.c_str(), new_indent);
                    dump(i->getTag(), true, keep_bool_type, ensure_ascii, indent_level, new_indent);
                    m_oa->writeChars(",\n", 2);
                }

                // last element (no comma at the end)
                NBT_ASSERT(!tag_t.m_value.empty());
                m_oa->writeChars(m_indent_string.c_str(), new_indent);
                dump(tag_t.m_value.back().getTag(), true, keep_bool_type, ensure_ascii, indent_level, new_indent);

                m_oa->writeChar('\n');
                m_oa->writeChars(m_indent_string.c_str(), current_indent);
                m_oa->writeChar(']');
            } else {
                m_oa->writeChar('[');

                // first n-1 elements
                for (auto i = tag_t.m_value.cbegin(); i != tag_t.m_value.cend() - 1; ++i) {
                    dump(i->getTag(), false, keep_bool_type, ensure_ascii, indent_level, current_indent);
                    m_oa->writeChar(',');
                }

                // last element (no comma at the end)
                NBT_ASSERT(!tag_t.m_value.empty());
                dump(tag_t.m_value.back().getTag(), false, keep_bool_type, ensure_ascii, indent_level, current_indent);

                m_oa->writeChar(']');
            }
            return;
        }
        case TagValue::ByteArray: {
            dumpArray(dynamic_cast<const ByteArrayTag&>(tag),
                pretty_print, indent_level, current_indent);
            return;
        }
        case TagValue::IntArray: {
            dumpArray(dynamic_cast<const IntArrayTag&>(tag),
                pretty_print, indent_level, current_indent);
            return;
        }
        case TagValue::LongArray: {
            dumpArray(dynamic_cast<const LongArrayTag&>(tag),
                pretty_print, indent_level, current_indent);
            return;
        }
        case TagValue::String: {
            dumpString(dynamic_cast<const StringTag&>(tag), ensure_ascii);
            return;
        }
        case TagValue::Bool: {
            if (keep_bool_type) {
                if (dynamic_cast<const ByteTag&>(tag).m_value) {
                    m_oa->writeChars("true", 4);
                } else {
                    m_oa->writeChars("false", 5);
                }
            } else {
                dumpInteger(dynamic_cast<const ByteTag&>(tag).m_value);
                m_oa->writeChar('b');
            }
            return;
        }
        case TagValue::Float: {
            dumpFloat(dynamic_cast<const FloatTag&>(tag).m_value);
            m_oa->writeChar('f');
            return;
        }
        case TagValue::Double: {
            dumpFloat(dynamic_cast<const DoubleTag&>(tag).m_value);
            m_oa->writeChar('d');
            return;
        }
        case TagValue::Byte: {
            dumpInteger(dynamic_cast<const ByteTag&>(tag).m_value);
            m_oa->writeChar('b');
            return;
        }
        case TagValue::Short: {
            dumpInteger(dynamic_cast<const ShortTag&>(tag).m_value);
            m_oa->writeChar('s');
            return;
        }
        case TagValue::Int: {
            dumpInteger(dynamic_cast<const IntTag&>(tag).m_value);
            return;
        }
        case TagValue::Long: {
            dumpInteger(dynamic_cast<const LongTag&>(tag).m_value);
            m_oa->writeChar('L');
            return;
        }
        case TagValue::End: {
            m_oa->writeChars("END", 3);
            return;
        }
        case TagValue::Discarded: {
            m_oa->writeChars("<DISCARDED>", 11);
            return;
        }
        default:
            NBT_ASSERT(false);
    }
}

// Escape the string content between surrounding quotes.
// Handles: surrounding-quote char, backslash, C0 control chars, and (optionally) non-ASCII
// codepoints. Non-ASCII input is expected to be valid UTF-8; invalid byte sequences are
// passed through unchanged.
void SnbtSerializer::escape(const std::string& s, const bool single_quotes_mode, const bool ensure_ascii)
{
    static constexpr char hex_digits[] = "0123456789ABCDEF";

    std::size_t bytes = 0;
    const char quotes = single_quotes_mode ? '\'' : '"';

    const auto* p   = reinterpret_cast<const uint8_t*>(s.data());
    const auto* end = p + s.size();

    // Flush the internal buffer and reset the write position.
    const auto flush = [&]() {
        m_oa->writeChars(m_string_buffer.data(), bytes);
        bytes = 0;
    };

    // Guarantee at least `needed` free bytes in the buffer before writing.
    // Maximum bytes written per iteration is 12 (\uHHHH\uLLLL surrogate pair).
    const auto ensure_space = [&](const std::size_t needed) {
        if (m_string_buffer.size() - bytes < needed) {
            flush();
        }
    };

    while (p < end) {
        const uint8_t byte = *p;

        if (byte == static_cast<uint8_t>(quotes)) {
            // Escape the surrounding quote character (SNBT quote rule, already present).
            ensure_space(2);
            m_string_buffer[bytes++] = '\\';
            m_string_buffer[bytes++] = quotes;
            ++p;
        } else if (byte == '\\') {
            ensure_space(2);
            m_string_buffer[bytes++] = '\\';
            m_string_buffer[bytes++] = '\\';
            ++p;
        } else if (byte < 0x20u) {
            // C0 control characters.
            ensure_space(6);
            switch (byte) {
                case '\b': m_string_buffer[bytes++] = '\\'; m_string_buffer[bytes++] = 'b'; break;
                case '\f': m_string_buffer[bytes++] = '\\'; m_string_buffer[bytes++] = 'f'; break;
                case '\n': m_string_buffer[bytes++] = '\\'; m_string_buffer[bytes++] = 'n'; break;
                case '\r': m_string_buffer[bytes++] = '\\'; m_string_buffer[bytes++] = 'r'; break;
                case '\t': m_string_buffer[bytes++] = '\\'; m_string_buffer[bytes++] = 't'; break;
                default:
                    // Remaining C0 controls as \u00XX.
                    m_string_buffer[bytes++] = '\\';
                    m_string_buffer[bytes++] = 'u';
                    m_string_buffer[bytes++] = '0';
                    m_string_buffer[bytes++] = '0';
                    m_string_buffer[bytes++] = hex_digits[(byte >> 4u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[ byte        & 0x0Fu];
                    break;
            }
            ++p;
        } else if (byte < 0x80u) {
            // Plain ASCII (0x20–0x7F).
            ensure_space(1);
            m_string_buffer[bytes++] = static_cast<char>(byte);
            ++p;
        } else {
            // Multi-byte UTF-8 sequence: decode the Unicode codepoint.
            uint32_t codepoint;
            int seq_len;

            if ((byte & 0xE0u) == 0xC0u) {
                codepoint = byte & 0x1Fu;
                seq_len = 2;
            } else if ((byte & 0xF0u) == 0xE0u) {
                codepoint = byte & 0x0Fu;
                seq_len = 3;
            } else if ((byte & 0xF8u) == 0xF0u) {
                codepoint = byte & 0x07u;
                seq_len = 4;
            } else {
                // Invalid lead byte — pass through as-is.
                ensure_space(1);
                m_string_buffer[bytes++] = static_cast<char>(byte);
                ++p;
                continue;
            }

            // Validate continuation bytes.
            bool valid = (p + seq_len <= end);
            if (valid) {
                for (int i = 1; i < seq_len; ++i) {
                    if ((p[i] & 0xC0u) != 0x80u) { valid = false; break; }
                    codepoint = (codepoint << 6u) | (p[i] & 0x3Fu);
                }
            }

            if (!valid) {
                ensure_space(1);
                m_string_buffer[bytes++] = static_cast<char>(byte);
                ++p;
                continue;
            }

            if (ensure_ascii) {
                if (codepoint <= 0xFFFFu) {
                    // BMP codepoint: \uXXXX
                    ensure_space(6);
                    m_string_buffer[bytes++] = '\\';
                    m_string_buffer[bytes++] = 'u';
                    m_string_buffer[bytes++] = hex_digits[(codepoint >> 12u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[(codepoint >>  8u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[(codepoint >>  4u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[ codepoint         & 0x0Fu];
                } else {
                    // Supplementary plane: encode as a UTF-16 surrogate pair \uHHHH\uLLLL
                    const uint32_t cp   = codepoint - 0x10000u;
                    const uint32_t high = 0xD800u + (cp >> 10u);
                    const uint32_t low  = 0xDC00u + (cp & 0x3FFu);
                    ensure_space(12);
                    m_string_buffer[bytes++] = '\\';
                    m_string_buffer[bytes++] = 'u';
                    m_string_buffer[bytes++] = hex_digits[(high >> 12u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[(high >>  8u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[(high >>  4u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[ high         & 0x0Fu];
                    m_string_buffer[bytes++] = '\\';
                    m_string_buffer[bytes++] = 'u';
                    m_string_buffer[bytes++] = hex_digits[(low >> 12u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[(low >>  8u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[(low >>  4u) & 0x0Fu];
                    m_string_buffer[bytes++] = hex_digits[ low         & 0x0Fu];
                }
            } else {
                // Pass through the raw UTF-8 bytes unchanged.
                ensure_space(static_cast<std::size_t>(seq_len));
                for (int i = 0; i < seq_len; ++i) {
                    m_string_buffer[bytes++] = static_cast<char>(p[i]);
                }
            }
            p += seq_len;
        }
    }

    m_oa->writeChars(m_string_buffer.data(), bytes);
}

void SnbtSerializer::dumpKey(const std::string& s, const bool ensure_ascii)
{
    if (s.empty()) {
        m_oa->writeChars("\"\"", 2);
    }

    // Keys consisting only of [A-Za-z0-9._+-] can be written without surrounding quotes.
    bool is_unquoted = true;
    for (const char ch : s) {
        if (!std::isalnum(ch) && ch != '.' && ch != '_' && ch != '+' && ch != '-') {
            is_unquoted = false;
            break;
        }
    }

    if (is_unquoted) {
        m_oa->writeChars(s.c_str(), s.size());
        return;
    }

    // Determine quote style: prefer double quotes; switch to single if a double quote is
    // encountered before any single quote, so the double quote does not need escaping.
    char quotes = '"';
    bool single_quotes_mode = false;
    for (const char c : s) {
        if (c == '"') { quotes = '\''; single_quotes_mode = true; break; }
        if (c == '\'') break;
    }

    m_oa->writeChar(quotes);
    escape(s, single_quotes_mode, ensure_ascii);
    m_oa->writeChar(quotes);
}

void SnbtSerializer::dumpString(const StringTag& tag, const bool ensure_ascii)
{
    if (tag.m_value.empty()) {
        m_oa->writeChars("\"\"", 2);
        return;
    }

    // Determine quote style: prefer double quotes; switch to single if a double quote is
    // encountered before any single quote.
    char quotes = '"';
    bool single_quotes_mode = false;
    for (const char c : tag.m_value) {
        if (c == '"') { quotes = '\''; single_quotes_mode = true; break; }
        if (c == '\'') break;
    }

    m_oa->writeChar(quotes);
    escape(tag.m_value, single_quotes_mode, ensure_ascii);
    m_oa->writeChar(quotes);
}

}

SHULKER_NBT_NAMESPACE_END
