#ifndef __TML_PLATFORM_HPP__
#define __TML_PLATFORM_HPP__


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define TORCH_PLATFORM_WINDOWS

    #if defined(_WIN64)
        #define TORCH_PLATFORM_WIN64
    #else
        #define TORCH_PLATFORM_WIN32
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #if defined(_MSVC_LANG)
        #include <sdkddkver.h>
    #endif

    #ifndef _WIN32_WINNT_WIN7
		#define _WIN32_WINNT_WIN7 0x0601
	#endif

	#ifndef _WIN32_WINNT_WINBLUE
		#define _WIN32_WINNT_WINBLUE 0x0603
	#endif

	#ifndef _WIN32_WINNT_WIN10
		#define _WIN32_WINNT_WIN10 0x0A00
	#endif

    #if (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
        #include <winapifamily.h>
        #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)
        #else
            #define TORCH_PLATFORM_WINDOWS_DESKTOP
        #endif
    #else
        #define TORCH_PLATFORM_WINDOWS_DESKTOP
    #endif

    #include <Windows.h>

    #ifndef _NTDEF_
        using NTSTATUS = LONG;
    #endif

#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#else
    #error "Unknown platform. The supported target platforms are Windows, Android, macOS, and iOS."
#endif

#ifdef TORCH_PLATFORM_WINDOWS
    #define TORCH_SYMBOL_EXPORT __declspec(dllexport)
    #define TORCH_SYMBOL_IMPORT __declspec(dllimport)
#else
    #define TORCH_SYMBOL_EXPORT __attribute__((visibility("default")))
    #define TORCH_SYMBOL_IMPORT
#endif
#endif