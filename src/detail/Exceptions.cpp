#include "shulker/detail/Exceptions.h"

#include "shulker/detail/StringUtils.h"

SHULKER_NBT_NAMESPACE_BEGIN

namespace detail
{

const char* Exception::what() const noexcept
{
    return m_err.what();
}

std::string Exception::name(const std::string& exception_name, unsigned code)
{
    return concat("[Nbt.Exception.", exception_name, ".", std::to_string(code), "]");
}

TypeError TypeError::create(unsigned code, const std::string& msg)
{
    const std::string m = concat(name("TypeError", code), " ", msg);
    return {code, m.c_str()};
}

ParseError ParseError::create(unsigned code, const std::string& msg)
{
    const std::string m = concat(name("ParseError", code), " ", msg);
    return {code, m.c_str()};
}

OutOfRangeError OutOfRangeError::create(unsigned code, const std::string& msg)
{
    const std::string m = concat(name("OutOfRangeError", code), " ", msg);
    return {code, m.c_str()};
}

OtherError OtherError::create(unsigned code, const std::string& msg)
{
    const std::string m = concat(name("OtherError", code), " ", msg);
    return {code, m.c_str()};
}

}

SHULKER_NBT_NAMESPACE_END
