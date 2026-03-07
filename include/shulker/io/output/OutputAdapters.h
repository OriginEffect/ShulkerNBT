#pragma once

#include <cstddef>
#include <memory>
#include <ostream>
#include <vector>

#include "shulker/common/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

template<typename CharType>
struct BasicOutputAdapter
{
    virtual void writeChar(CharType c) = 0;
    virtual void writeChars(const CharType* s, std::size_t length) = 0;
    virtual ~BasicOutputAdapter() = default;

    BasicOutputAdapter() = default;
    BasicOutputAdapter(const BasicOutputAdapter&) = default;
    BasicOutputAdapter(BasicOutputAdapter&&) noexcept = default;
    BasicOutputAdapter& operator=(const BasicOutputAdapter&) = default;
    BasicOutputAdapter& operator=(BasicOutputAdapter&&) noexcept = default;
};

template<typename CharType>
using OutputAdapterPtr = std::shared_ptr<BasicOutputAdapter<CharType>>;

template<typename CharType>
class OutputVectorAdapter : public BasicOutputAdapter<CharType>
{
public:
    explicit OutputVectorAdapter(std::vector<CharType>& vec) noexcept
        : m_vec(vec) {}

    void writeChar(CharType c) override
    {
        m_vec.push_back(c);
    }

    void writeChars(const CharType* s, std::size_t length) override
    {
        m_vec.insert(m_vec.end(), s, s + length);
    }

private:
    std::vector<CharType> m_vec;
};

template<typename CharType>
class OutputStreamAdapter : public BasicOutputAdapter<CharType>
{
public:
    explicit OutputStreamAdapter(std::basic_ostream<CharType>& s) noexcept
        : m_stream(s) {}

    void writeChar(CharType c) override
    {
        m_stream.put(c);
    }

    void writeChars(const CharType* s, std::size_t length) override
    {
        m_stream.write(s, static_cast<std::streamsize>(length));
    }

private:
    std::basic_ostream<CharType>& m_stream;
};

template<typename CharType, typename StringType = std::basic_string<CharType>>
class OutputStringAdapter : public BasicOutputAdapter<CharType>
{
public:
    explicit OutputStringAdapter(StringType& s) noexcept
        : m_str(s) {}

    void writeChar(CharType c) override
    {
        m_str.push_back(c);
    }

    void writeChars(const CharType* s, std::size_t length) override
    {
        m_str.append(s, length);
    }

private:
    StringType& m_str;
};

template<typename CharType, typename StringType = std::basic_string<CharType>>
class OutputAdapter
{
public:
    OutputAdapter(std::vector<CharType>& vec)
        : m_output_adapter(std::make_shared<OutputVectorAdapter<CharType>>(vec)) {}

    OutputAdapter(std::basic_ostream<CharType>& stream)
        : m_output_adapter(std::make_shared<OutputStreamAdapter<CharType>>(stream)) {}

    OutputAdapter(StringType& str)
        : m_output_adapter(std::make_shared<OutputStringAdapter<CharType>>(str)) {}

    operator OutputAdapterPtr<CharType>()
    {
        return m_output_adapter;
    }

private:
    OutputAdapterPtr<CharType> m_output_adapter = nullptr;
};

SHULKER_NBT_NAMESPACE_END
