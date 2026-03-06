#pragma once

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <string>

#include "shulker/common/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

// calculate the total length of all pieces
inline std::size_t concat_length()
{
    return 0;
}

template<typename... Args>
inline std::size_t concat_length(const char* cs, const Args&... rest);

template<typename StringType, typename... Args>
inline std::size_t concat_length(const StringType& str, const Args&... rest);

template<typename... Args>
inline std::size_t concat_length(const char* cs, const Args&... rest)
{
    return std::strlen(cs) + concat_length(rest...);
}

template<typename StringType, typename... Args>
inline std::size_t concat_length(const StringType& str, const Args&... rest)
{
    return str.size() + concat_length(rest...);
}

template<typename... Args>
inline std::size_t concat_length(const char c, const Args&... rest)
{
    return 1 + concat_length(rest...);
}

// append to the existing string
template<typename OutStringType>
inline void concat_into(OutStringType&)
{}

template<typename OutStringType, typename Arg, typename... Args>
inline void concat_into(OutStringType& str, Arg arg, Args... rest)
{
    str.append(arg);
    concat_into(str, rest...);
}

// concatenate all pieces
template<typename OutStringType = std::string, typename... Args>
inline OutStringType concat(Args&& ...args)
{
    OutStringType out;
    out.reserve(concat_length(args...));
    concat_into(out, std::forward<Args>(args)...);
    return out;
}

SHULKER_NBT_NAMESPACE_END
