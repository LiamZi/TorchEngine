#ifndef __TML_CONFIG_HPP__
#define __TML_CONFIG_HPP__

#if !defined(__cplusplus)
    #error C++ compiler required.
#endif

#if defined(DEBUG) | defined(_DEBUG)
    #define TORCH_DEBUG
#endif

#define TML_STRINGIZE(X) TML_DO_STRINGIZE(X)
#define TML_DO_STRINGIZE(X) #X

#define TML_JOIN(X, Y) TML_DO_JOIN(X, Y)
#define TML_DO_JOIN(X, Y) TML_DO_JOIN2(X, Y)
#define TML_DO_JOIN2(X, Y) X##Y


#include <TML/Compiler.hpp>
#include <TML/Platform.hpp>


#if defined(TORCH_PLATFORM_WINDOWS_DESKTOP) || defined(KLAYGE_PLATFORM_DARWIN)
    #define TORCH_IS_DEV_PLATFORM 1
#else
    #define TORCH_IS_DEV_PLATFORM 0
#endif

#endif