#ifndef __TML_INCLUDE_TML_DLL_LOADER_HPP__
#define __TML_INCLUDE_TML_DLL_LOADER_HPP__

#pragma once

#include <string>

#if defined(TORCH_COMPILER_MSVC) || defined(TORCH_COMPILER_CLANGCL)
	#define DLL_PREFIX ""
#else
	#define DLL_PREFIX "lib"
#endif
#if defined(TORCH_PLATFORM_WINDOWS)
	#define DLL_EXT_NAME "dll"
#elif defined(TORCH_PLATFORM_DARWIN)
	#define DLL_EXT_NAME "dylib"
#else
	#define DLL_EXT_NAME "so"
#endif

#define DLL_SUFFIX TORCH_OUTPUT_SUFFIX "." DLL_EXT_NAME


namespace Torch
{
    class DllLoader final
    {
    private:
        void *_dll_handle;

    public:
        DllLoader();
        ~DllLoader();

        bool Load(std::string const &dll_name);
        void Free();

        void *GetProcAddress(std::string const &proc_name);
    };
};

#endif