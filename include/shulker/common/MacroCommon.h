#pragma once

// allow disabling exceptions
#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && !defined(NBT_NOEXCEPTION)
    #define NBT_THROW(exception) throw exception
    #define NBT_TRY try
    #define NBT_CATCH(exception) catch(exception)
#else
    #include <cstdlib>
    #define NBT_THROW(exception) std::abort()
    #define NBT_TRY if(true)
    #define NBT_CATCH(exception) if(false)
#endif

// override exception macros
#if defined(NBT_THROW_USER)
    #undef NBT_THROW
    #define NBT_THROW NBT_THROW_USER
#endif
#if defined(NBT_TRY_USER)
    #undef NBT_TRY
    #define NBT_TRY NBT_TRY_USER
#endif
#if defined(NBT_CATCH_USER)
    #undef NBT_CATCH
    #define NBT_CATCH NBT_CATCH_USER
#endif

// class macros
#ifndef SHULKER_NBT_FRIEND_CLASS
    #define SHULKER_NBT_FRIEND_CLASS(FRIEND_CLASS)  \
    friend class FRIEND_CLASS;
#endif
