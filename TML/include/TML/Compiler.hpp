#ifndef __TML_COMPILER_HPP__
#define __TML_COMPILER_HPP__


#if defined(_MSC_VER)

    #define TORCH_COMPILER_MSVC
    #define TORCH_COMPILER_NAME vc

    #if _MSC_VER >= 1930
        #define TORCH_COMPILER_VERSION 143
    #elif _MSC_VER >= 1920
        #define TORCH_COMPILER_VERSION 142
    #elif _MSC_VER >= 1911
        #define TORCH_COMPILER_VERSION 141
    #else
        #error "Unsupported compiler version. Please install VS2017 15.3 or up."
    #endif
    
    #if _MSVC_LANG < 201703L
        #error "/std:c++17 must be turned on."
    #endif

    #if _MSC_VER >= 1914
        #define TORCH_CXX17_LIBRARY_FILESYSTEM_SUPPORT
    #endif

    #if _MSC_VER >= 1915
        #define TORCH_CXX17_LIBRARY_CHARCONV_SUPPORT
    #endif

    #if _MSVC_LANG > 201703L
		#if _MSC_VER >= 1922
			#define TORCH_CXX20_LIBRARY_ENDIAN_SUPPORT
		#endif
		#if _MSC_VER >= 1926
			#define TORCH_CXX20_LIBRARY_SPAN_SUPPORT
		#endif
		#if _MSC_VER >= 1927
			#define TORCH_CXX20_LIBRARY_BIT_CAST_SUPPORT
		#endif
		#if _MSC_VER >= 1928
			#define TORCH_CXX20_LIBRARY_BIT_OPERATIONS_SUPPORT
			#define TORCH_CXX20_LIBRARY_INTEGRAL_POWER_OF_2_OPERATIONS_SUPPORT
		#endif
		#if _MSC_VER >= 1929
			#define TORCH_CXX20_LIBRARY_FORMAT_SUPPORT
		#endif
	#endif

    #pragma warning(disable: 4251)
    #pragma warning(disable: 4275)
    #pragma warning(disable: 4819)
    
    #define TORCH_BUILTIN_UNREACHABLE __assume(false)

#else
    #error "Unknown compiler. "
#endif

#endif