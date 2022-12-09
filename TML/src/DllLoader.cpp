#include <TML/TML.hpp>

#ifdef TORCH_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include <TML/DllLoader.hpp>


namespace Torch
{

    DllLoader::DllLoader()
        :_dll_handle{nullptr}
    {
    }

    DllLoader::~DllLoader()
    {
        Free();
    }

    bool DllLoader::Load(std::string const &dll_name)
    {
#ifdef TORCH_PLATFORM_WINDOWS
#ifdef TORCH_PLATFORM_WINDOWS_DESKTOP
        _dll_handle = static_cast<void *>(::LoadLibraryExA(dll_name.c_str(), nullptr, 0));
#else
        std::wstring wname;
        Convert(wname, dll_name);
        _dll_handle = static_cast<void *>(::LoadPackagedLibrary(wname.c_st(), 0));
#endif
#else
        _dll_handle = ::dlopen(dll_name.c_str(), RTLD_LAZY);
#endif
        return (_dll_handle != nullptr);
    }

    void DllLoader::Free()
    {
        if(_dll_handle)
        {
#ifdef TORCH_PLATFORM_WINDOWS
            ::FreeLibrary(static_cast<HMODULE>(_dll_handle));
#else
            ::dlclose(_dll_handle);
#endif
        }
    }

    void *DllLoader::GetProcAddress(std::string const &proc_name)
    {
#ifdef TORCH_PLATFORM_WINDOWS
		return reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(_dll_handle), proc_name.c_str()));
#else
		return ::dlsym(dll_handle_, proc_name.c_str());
#endif
    }
};