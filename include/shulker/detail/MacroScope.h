#pragma once

// namespace
#ifndef SHULKER_NBT_NAMESPACE
#define SHULKER_NBT_NAMESPACE \
    shulker
#endif

#ifndef SHULKER_NBT_NAMESPACE_BEGIN
#define SHULKER_NBT_NAMESPACE_BEGIN	\
    namespace shulker {
#endif

#ifndef SHULKER_NBT_NAMESPACE_END
#define SHULKER_NBT_NAMESPACE_END	\
    }
#endif

// dll export/import
#if defined(_WIN32)
    #if defined(SHULKER_NBT_EXPORTS)
        #define SHULKER_API __declspec(dllexport)
    #else
        #define SHULKER_API __declspec(dllimport)
    #endif
#else
    #define SHULKER_API
#endif

// assert
#ifndef NBT_ASSERT
    #include <cassert>
    #define NBT_ASSERT(x) assert(x)
#endif
