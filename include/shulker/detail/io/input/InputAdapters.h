#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <istream>
#include <iterator>
#include <string>

#include "../../MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

template<typename TargetBufferType, std::enable_if_t<
    std::is_trivially_copyable_v<TargetBufferType>, int> = 0>
struct BasicInputAdapter
{
    virtual std::char_traits<char>::int_type getChar() = 0;
    virtual std::size_t getCharsTo(TargetBufferType* dest, std::size_t cnt) = 0;

    virtual std::char_traits<char>::int_type peek()
    {
        if (!this->m_has_buffer) {
            this->m_char_buffer = getChar();
            this->m_has_buffer = true;
        }
        return this->m_char_buffer;
    }

    BasicInputAdapter() = default;
    BasicInputAdapter(const BasicInputAdapter&) = default;
    BasicInputAdapter(BasicInputAdapter&&) noexcept = default;
    BasicInputAdapter& operator=(const BasicInputAdapter&) = default;
    BasicInputAdapter& operator=(BasicInputAdapter&&) noexcept = default;

    virtual ~BasicInputAdapter() = default;

protected:
    bool m_has_buffer = false;
    char m_char_buffer{};
};

template<typename TargetBufferType = char>
using InputAdapterPtr = std::shared_ptr<BasicInputAdapter<TargetBufferType>>;

template<typename TargetBufferType>
class InputStreamAdapter : public BasicInputAdapter<TargetBufferType>
{
public:
    using CharType = char;

    explicit InputStreamAdapter(std::istream& s) noexcept
        : m_stream(s) {}

    std::char_traits<char>::int_type getChar() override
    {
        if (this->m_has_buffer) {
            this->m_has_buffer = false;
            return this->m_char_buffer;
        }

        auto ch = m_stream.get();
        if (m_stream.fail()) {
            return std::char_traits<char>::eof();
        }
        return ch;
    }

    std::size_t getCharsTo(TargetBufferType* dest, std::size_t cnt) override
    {
        if (dest == nullptr || cnt == 0) {
            return 0;
        }

        std::size_t needed = cnt * sizeof(TargetBufferType);
        std::size_t read = 0;
        auto p = reinterpret_cast<char*>(dest);

        if (this->m_has_buffer) {
            this->m_has_buffer = false;
            p[0] = this->m_char_buffer;
            if (needed == 1) {
                return 1;
            }
            ++p, ++read, --needed;
        }

        m_stream.read(p, static_cast<std::streamsize>(needed));
        read += m_stream.gcount();

        return read;
    }

private:
    std::istream& m_stream;
};

template<typename TargetBufferType>
class InputBufferAdapter : public BasicInputAdapter<TargetBufferType>
{
public:
    using CharType = char;

    InputBufferAdapter(const char* buffer, std::size_t size)
        : m_current(buffer), m_end(buffer + size) {}

    std::char_traits<char>::int_type getChar() override
    {
        if (this->m_has_buffer) {
            this->m_has_buffer = false;
            return this->m_char_buffer;
        }

        if (m_current == m_end) {
            return std::char_traits<char>::eof();
        }
        return std::char_traits<char>::to_int_type(*m_current++);
    }

    std::size_t getCharsTo(TargetBufferType* dest, std::size_t cnt) override
    {
        if (dest == nullptr || cnt == 0) {
            return 0;
        }

        std::size_t needed = cnt * sizeof(TargetBufferType);

        if (this->m_has_buffer) {
            this->m_has_buffer = false;
            reinterpret_cast<char*>(dest)[0] = this->m_char_buffer;
            if (needed == 1) {
                return 1;
            }
            --needed;
        }

        const auto left = static_cast<std::size_t>(m_end - m_current);
        const std::size_t bytes_to_copy = std::min(left, needed);

        std::memcpy(dest, m_current, bytes_to_copy);
        m_current += bytes_to_copy;

        return bytes_to_copy;
    }

private:
    const char* m_current;
    const char* m_end;
};

template<typename IteratorType, typename TargetBufferType>
class InputIteratorAdapter : public BasicInputAdapter<TargetBufferType>
{
public:
    using CharType = typename std::iterator_traits<IteratorType>::value_type;

    InputIteratorAdapter(IteratorType first, IteratorType last)
        : m_current(std::move(first)), m_end(std::move(last)) {}

    typename std::char_traits<CharType>::int_type getChar() override
    {
        if (this->m_has_buffer) {
            this->m_has_buffer = false;
            return this->m_char_buffer;
        }

        if (m_current == m_end) {
            return std::char_traits<char>::eof();
        }
        return std::char_traits<char>::to_int_type(*m_current++);
    }

    std::size_t getCharsTo(TargetBufferType* dest, std::size_t cnt) override
    {
        if (dest == nullptr || cnt == 0) {
            return 0;
        }

        std::size_t needed = cnt * sizeof(TargetBufferType);

        if (this->m_has_buffer) {
            this->m_has_buffer = false;
            reinterpret_cast<char*>(dest)[0] = this->m_char_buffer;
            if (needed == 1) {
                return 1;
            }
            --needed;
        }

        const auto out = reinterpret_cast<char*>(dest);
        std::size_t read = 0;
        for (; read < needed && m_current != m_end; ++read, ++m_current) {
            out[read] = static_cast<char>(*m_current);
        }

        return read;
    }

private:
    IteratorType m_current;
    IteratorType m_end;
};

template<typename TargetBufferType = char>
class InputAdapter
{
public:
    InputAdapter(std::basic_istream<TargetBufferType>& stream)
        : m_input_adapter(std::make_shared<InputStreamAdapter<TargetBufferType>>(stream)) {}

    InputAdapter(const char* buffer)
        : m_input_adapter(std::make_shared<InputBufferAdapter<TargetBufferType>>(buffer, sizeof(buffer))) {}

    template<typename IteratorType>
    InputAdapter(IteratorType first, IteratorType last)
        : m_input_adapter(std::make_shared<InputIteratorAdapter<IteratorType, TargetBufferType>>(first, last)) {}

    operator InputAdapterPtr<TargetBufferType>()
    {
        return m_input_adapter;
    }

private:
    InputAdapterPtr<TargetBufferType> m_input_adapter = nullptr;
};

}

SHULKER_NBT_NAMESPACE_END
