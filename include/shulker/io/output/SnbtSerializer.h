#pragma once

#include <array>
#include <utility>

#include <fmt/xchar.h>

#include "shulker/common/MacroScope.h"

#include "shulker/io/output/OutputAdapters.h"
#include "shulker/meta/TypeTraits.h"
#include "shulker/tags/BasicTag.h"
#include "shulker/tags/StringTag.h"

SHULKER_NBT_NAMESPACE_BEGIN
    // TODO: 添加高性能输出方法、更多输出格式支持、更多编码方式支持
class SnbtSerializer
{
public:
    SnbtSerializer(OutputAdapterPtr<char> oa, const char indent_char)
        : m_oa(std::move(oa)), m_indent_char(indent_char), m_indent_string(512, indent_char) {}

    SnbtSerializer(const SnbtSerializer&) = delete;
    SnbtSerializer& operator=(const SnbtSerializer&) = delete;
    SnbtSerializer(SnbtSerializer&&) = delete;
    SnbtSerializer& operator=(SnbtSerializer&&) = delete;
    ~SnbtSerializer() = default;

    void dump(
        const BasicTag& tag,
        bool pretty_print,
        bool keep_snbt_type,
        unsigned int indent_level,
        unsigned int current_indent = 0);

private:
    void escape(const std::string& s, bool single_quotes_mode);

    void dumpKey(const std::string& s);

    template<typename ArrayType, std::enable_if_t<
        is_array_tag<ArrayType>::value, int> = 0>
    void dumpArray(
        const ArrayType& tag,
        const bool pretty_print,
        const bool keep_snbt_type,
        const unsigned int indent_level,
        const unsigned int current_indent = 0)
    {
        if (tag.m_value.empty()) {
            m_oa->writeChar('[');
            m_oa->writeChar(ArrayType::symbol());
            m_oa->writeChars(";]", 2);
            return;
        }

        char suffix = '\0';
        if constexpr (std::is_same_v<ArrayType, ByteArrayTag>) {
            suffix = 'B';
        } else if constexpr (std::is_same_v<ArrayType, LongArrayTag>) {
            suffix = 'L';
        }

        if (pretty_print) {
            m_oa->writeChar('[');
            m_oa->writeChar(ArrayType::symbol());
            m_oa->writeChars(";\n", 2);

            const auto new_indent = current_indent + indent_level;
            if (m_indent_string.size() < new_indent) {
                m_indent_string.resize(m_indent_string.size() * 2, ' ');
            }

            // first n-1 elements
            for (auto i = tag.m_value.cbegin(); i != tag.m_value.cend() - 1; ++i) {
                m_oa->writeChars(m_indent_string.c_str(), new_indent);
                dumpInteger(*i);
                if constexpr (!std::is_same_v<ArrayType, IntArrayTag>) m_oa->writeChar(suffix);
                m_oa->writeChars(",\n", 2);
            }

            // last element (no comma at the end)
            NBT_ASSERT(!tag.m_value.empty());
            m_oa->writeChars(m_indent_string.c_str(), new_indent);
            dumpInteger(tag.m_value.back());
            if constexpr (!std::is_same_v<ArrayType, IntArrayTag>) m_oa->writeChar(suffix);

            m_oa->writeChar('\n');
            m_oa->writeChars(m_indent_string.c_str(), current_indent);
            m_oa->writeChar(']');
        } else {
            m_oa->writeChar('[');
            m_oa->writeChar(ArrayType::symbol());
            m_oa->writeChar(';');

            // first n-1 elements
            for (auto i = tag.m_value.cbegin(); i != tag.m_value.cend() - 1; ++i) {
                dumpInteger(*i);
                if constexpr (!std::is_same_v<ArrayType, IntArrayTag>) m_oa->writeChar(suffix);
                m_oa->writeChar(',');
            }

            // last element (no comma at the end)
            NBT_ASSERT(!tag.m_value.empty());
            dumpInteger(tag.m_value.back());
            if constexpr (!std::is_same_v<ArrayType, IntArrayTag>) m_oa->writeChar(suffix);

            m_oa->writeChar(']');
        }
    }

    void dumpString(const StringTag& tag);

    // TODO: IEEE754标准判断与处理、更高效安全的输出算法
    void dumpFloat(double x);

    // TODO: 更高效安全的输出算法
    template<typename NumberType, std::enable_if_t<
        std::is_integral_v<NumberType>, int> = 0>
    void dumpInteger(NumberType x)
    {
        if (x == 0) {
            m_oa->writeChar('0');
            return;
        }

        auto* begin = m_number_buffer.data();
        auto cap = m_number_buffer.size();

        auto r = fmt::format_to_n(begin, cap, "{}", x);

        m_oa->writeChars(begin, r.size < cap ? r.size : cap);
    }

private:
    OutputAdapterPtr<char> m_oa = nullptr;

    const char m_indent_char;

    std::string m_indent_string;

    std::array<char, 64> m_number_buffer{{}};
    std::array<char, 512> m_string_buffer{{}};
};

SHULKER_NBT_NAMESPACE_END
