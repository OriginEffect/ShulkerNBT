#include "shulker/common/Exceptions.h"
#include "shulker/common/StringUtils.h"
#include "shulker/io/output/SnbtSerializer.h"
#include "shulker/tags/CompoundTag.h"
#include "shulker/tags/TagReference.h"

SHULKER_NBT_NAMESPACE_BEGIN

void SnbtSerializer::dump(
    const BasicTag& tag,
    const bool pretty_print,
    const bool keep_snbt_type,
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
                    dumpKey(i->first);
                    m_oa->writeChars(": ", 2);
                    dump(i->second.getTag(), true, keep_snbt_type, indent_level, new_indent);
                    m_oa->writeChars(",\n", 2);
                }

                // last element (no comma at the end)
                NBT_ASSERT(i != tag_t.m_value.cend());
                NBT_ASSERT(std::next(i) == tag_t.m_value.cend());
                m_oa->writeChars(m_indent_string.c_str(), new_indent);
                dumpKey(i->first);
                m_oa->writeChars(": ", 2);
                dump(i->second.getTag(), true, keep_snbt_type, indent_level, new_indent);

                m_oa->writeChar('\n');
                m_oa->writeChars(m_indent_string.c_str(), current_indent);
                m_oa->writeChar('}');
            } else {
                m_oa->writeChar('{');

                // first n-1 elements
                auto i = tag_t.m_value.cbegin();
                for (std::size_t cnt = 0; cnt < tag_t.m_value.size() - 1; ++cnt, ++i) {
                    dumpKey(i->first);
                    m_oa->writeChar(':');
                    dump(i->second.getTag(), false, keep_snbt_type, indent_level, current_indent);
                    m_oa->writeChar(',');
                }

                // last element (no comma at the end)
                NBT_ASSERT(i != tag_t.m_value.cend());
                NBT_ASSERT(std::next(i) == tag_t.m_value.cend());
                dumpKey(i->first);
                m_oa->writeChar(':');
                dump(i->second.getTag(), false, keep_snbt_type, indent_level, current_indent);

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
                    dump(i->getTag(), true, keep_snbt_type, indent_level, new_indent);
                    m_oa->writeChars(",\n", 2);
                }

                // last element (no comma at the end)
                NBT_ASSERT(!tag_t.m_value.empty());
                m_oa->writeChars(m_indent_string.c_str(), new_indent);
                dump(tag_t.m_value.back().getTag(), true, keep_snbt_type, indent_level, new_indent);

                m_oa->writeChar('\n');
                m_oa->writeChars(m_indent_string.c_str(), current_indent);
                m_oa->writeChar(']');
            } else {
                m_oa->writeChar('[');

                // first n-1 elements
                for (auto i = tag_t.m_value.cbegin(); i != tag_t.m_value.cend() - 1; ++i) {
                    dump(i->getTag(), false, keep_snbt_type, indent_level, current_indent);
                    m_oa->writeChar(',');
                }

                // last element (no comma at the end)
                NBT_ASSERT(!tag_t.m_value.empty());
                dump(tag_t.m_value.back().getTag(), false, keep_snbt_type, indent_level, current_indent);

                m_oa->writeChar(']');
            }
            return;
        }
        case TagValue::ByteArray: {
            dumpArray(dynamic_cast<const ByteArrayTag&>(tag),
                pretty_print, keep_snbt_type, indent_level, current_indent);
            return;
        }
        case TagValue::IntArray: {
            dumpArray(dynamic_cast<const IntArrayTag&>(tag),
                pretty_print, keep_snbt_type, indent_level, current_indent);
            return;
        }
        case TagValue::LongArray: {
            dumpArray(dynamic_cast<const LongArrayTag&>(tag),
                pretty_print, keep_snbt_type, indent_level, current_indent);
            return;
        }
        case TagValue::String: {
            dumpString(dynamic_cast<const StringTag&>(tag));
            return;
        }
        case TagValue::Bool: {
            if (keep_snbt_type) {
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
        case TagValue::UnsignedByte: {
            dumpInteger(dynamic_cast<const ByteTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChar('u');
            m_oa->writeChar('b');
            return;
        }
        case TagValue::SignedByte: {
            dumpInteger(dynamic_cast<const ByteTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChar('s');
            m_oa->writeChar('b');
            return;
        }
        case TagValue::UnsignedShort: {
            dumpInteger(dynamic_cast<const ShortTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChar('u');
            m_oa->writeChar('s');
            return;
        }
        case TagValue::SignedShort: {
            dumpInteger(dynamic_cast<const ShortTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChar('s');
            m_oa->writeChar('s');
            return;
        }
        case TagValue::UnsignedInt: {
            dumpInteger(dynamic_cast<const IntTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChars("ui", 2);
            return;
        }
        case TagValue::SignedInt: {
            dumpInteger(dynamic_cast<const IntTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChars("si", 2);
            return;
        }
        case TagValue::UnsignedLong: {
            dumpInteger(dynamic_cast<const LongTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChars("ul", 2);
            else m_oa->writeChar('L');
            return;
        }
        case TagValue::SignedLong: {
            dumpInteger(dynamic_cast<const LongTag&>(tag).m_value);
            if (keep_snbt_type) m_oa->writeChars("sl", 2);
            else m_oa->writeChar('L');
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

// TODO: 支持输出ASCII；支持更多字符转义（目前只对引号做了处理，因为引号比较特殊）；对非法UTF-8字节序列的处理
// 遇到未做处理的字符（非ASCII字符）会出问题，暂时不要尝试
void SnbtSerializer::escape(const std::string& s, bool single_quotes_mode)
{
    std::size_t bytes = 0;
    char quotes = '"';
    if (single_quotes_mode) {
        quotes = '\'';
    }

    for (const char c : s) {
        if (c == quotes) {
            m_string_buffer[bytes++] = '\\';
            m_string_buffer[bytes++] = quotes;
        } else {
            // 先暂时这么用着
            m_string_buffer[bytes++] = c;
        }

        // 清空缓冲区；一个码点最大写入为13字节（"\uxxxx\uxxxx\0"），虽然上面根本还没写处理
        if (m_string_buffer.size() - bytes < 13) {
            m_oa->writeChars(m_string_buffer.data(), bytes);
            bytes = 0;
        }
    }

    m_oa->writeChars(m_string_buffer.data(), bytes);
}

void SnbtSerializer::dumpKey(const std::string& s)
{
    // this function is only used for internal key dumping, and the key must not be empty
    NBT_ASSERT(!s.empty());

    auto it = s.cbegin();
    bool is_unquoted = true;
    while (it != s.cend()) {
        const auto ch = *it;
        if (std::isalnum(ch) || ch == '.' || ch == '_' || ch == '+' || ch == '-') { // [A-Za-z0-9._+-]*
            ++it;
            continue;
        }
        is_unquoted = false;
        break;
    }

    if (is_unquoted) {
        m_oa->writeChars(s.c_str(), s.size());
        return;
    }
    NBT_ASSERT(it != s.cend());

    char quotes = '"'; // default the double quotes
    bool single_quotes_mode = false;
    // try to find the first quote
    while (it != s.cend()) {
        if (*it == '"') {
            quotes = '\'';
            single_quotes_mode = true;
            break;
        }
        if (*it == '\'') break;
        ++it;
    }
    if (it == s.cend()) { // the string does not contain quotes
        m_oa->writeChar(quotes);
        m_oa->writeChars(s.c_str(), s.size());
        m_oa->writeChar(quotes);
        return;
    }

    m_oa->writeChar(quotes);
    NBT_ASSERT(it != s.cend());
    const auto next_pos = static_cast<std::size_t>(it - s.cbegin() + 1);
    m_oa->writeChars(s.substr(0, next_pos).c_str(), next_pos);
    escape(s.substr(next_pos), single_quotes_mode);
    m_oa->writeChar(quotes);
}

void SnbtSerializer::dumpString(const StringTag& tag)
{
    if (tag.m_value.empty()) {
        m_oa->writeChars("\"\"", 2);
        return;
    }

    char quotes = '"'; // default the double quotes
    bool single_quotes_mode = false;
    auto it = tag.m_value.cbegin();
    NBT_ASSERT(it != tag.m_value.cend());
    // try to find the first quote
    while (it != tag.m_value.cend()) {
        if (*it == '"') {
            quotes = '\'';
            single_quotes_mode = true;
            break;
        }
        if (*it == '\'') break;
        ++it;
    }
    if (it == tag.m_value.cend()) { // the string does not contain quotes
        m_oa->writeChar(quotes);
        m_oa->writeChars(tag.m_value.c_str(), tag.m_value.size());
        m_oa->writeChar(quotes);
        return;
    }

    if (*it == '"') quotes = '\'';
    m_oa->writeChar(quotes);
    NBT_ASSERT(it != tag.m_value.cend());
    const auto next_pos = static_cast<std::size_t>(it - tag.m_value.cbegin() + 1);
    m_oa->writeChars(tag.m_value.substr(0, next_pos).c_str(), next_pos);
    escape(tag.m_value.substr(next_pos), single_quotes_mode);
    m_oa->writeChar(quotes);
}

void SnbtSerializer::dumpFloat(double x)
{
    if (!std::isfinite(x)) {
        // ensure the tag type remains unchanged
        m_oa->writeChars("0.0f", 4);
    }

    auto* begin = m_number_buffer.data();
    auto cap = m_number_buffer.size();

    auto r = fmt::format_to_n(begin, cap, "{}", x);

    m_oa->writeChars(begin, r.size < cap ? r.size : cap);
}

SHULKER_NBT_NAMESPACE_END
