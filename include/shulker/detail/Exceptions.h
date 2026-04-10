#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include "shulker/detail/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

class SHULKER_API Exception : public std::exception
{
public:
    [[nodiscard]] const char* what() const noexcept override;

    const unsigned m_code;

protected:
    Exception(const unsigned code, const char* message) : m_code(code), m_err(message) {}

    static std::string name(const std::string& exception_name, unsigned code);

private:
    std::runtime_error m_err;
};

class SHULKER_API TypeError : public Exception
{
public:
    static TypeError create(unsigned code, const std::string& msg);

private:
    TypeError(const unsigned code, const char* msg) : Exception(code, msg) {};
};

class SHULKER_API ParseError : public Exception
{
public:
    static ParseError create(unsigned code, const std::string& msg);

private:
    ParseError(const unsigned code, const char* msg) : Exception(code, msg) {};
};

class SHULKER_API OutOfRangeError : public Exception
{
public:
    static OutOfRangeError create(unsigned code, const std::string& msg);

private:
    OutOfRangeError(const unsigned code, const char* msg) : Exception(code, msg) {};
};

class SHULKER_API OtherError : public Exception
{
public:
    static OtherError create(unsigned code, const std::string& msg);

private:
    OtherError(const unsigned code, const char* msg) : Exception(code, msg) {};
};

}

SHULKER_NBT_NAMESPACE_END
