#include "shulker/common/Exceptions.h"

#include "shulker/common/StringUtils.h"

SHULKER_NBT_NAMESPACE_BEGIN

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

SHULKER_NBT_NAMESPACE_END
