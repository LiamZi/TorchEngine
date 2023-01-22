#include <Torch/Resources/ResourceLoader.hpp>
#include <Torch/Interfaces/Context.hpp>
#include <filesystem>
#include <algorithm>
namespace 
{
	std::mutex singleton_mutex;
};

namespace Torch
{
	std::unique_ptr<ResourceLoader> ResourceLoader::_res_loader_instance = nullptr;

    ResourceLoader::ResourceLoader()
    {
#if defined TORCH_PLATFORM_WINDOWS
		char buf[MAX_PATH];
		::GetModuleFileNameA(nullptr, buf, sizeof(buf));
		_exe_path = buf;
		_exe_path = _exe_path.substr(0, _exe_path.rfind("\\"));
		_local_path = _exe_path + "/";
#elif defined TORCH_PLATFORM_DARWIN
		uint32_t size = 0;
		_NSGetExecutablePath(nullptr, &size);
		auto buffer = MakeUniquePtr<char[]>(size + 1);
		_NSGetExecutablePath(buffer.get(), &size);
		buffer[size] = '\0';
		_exe_path = buffer.get();
		_exe_path = _exe_path.substr(0, _exe_path.find_last_of("/") + 1);
		_local_path = _exe_path;
#endif
		_paths.emplace_back(0, 0, "");

		this->AddPath("");

#if defined TORCH_PLATFORM_WINDOWS_DESKTOP
		::GetCurrentDirectoryA(sizeof(buf), buf);
		char *colon = std::find(buf, buf + sizeof(buf), ':');
		assert(colon != buf + sizeof(buf));
		colon[1] = '/';
		colon[2] = '\0';
		this->AddPath(buf);
#endif

		this->AddPath("..");

		_loading_thread = Context::Instance().ThreadPools().QueueThread([this] {
			this->LoadingThreadFunc();
		});


    }

    ResourceLoader::~ResourceLoader()
    {
        _quit = true;

        std::unique_lock<std::mutex> lock(_loading_res_queue_mutex, std::try_to_lock);
        _non_empty_loading_res_queue = true;
        _loading_res_queue_cv.notify_one();

        _loading_thread.wait();
    }


    ResourceLoader &ResourceLoader::Instance()
    {
        if(!_res_loader_instance)
		{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if(!_res_loader_instance)
			{
				_res_loader_instance = MakeUniquePtr<ResourceLoader>();
			}
		}

		return *_res_loader_instance;
    }
    void ResourceLoader::Destroy()
    {
		_res_loader_instance.reset();
    }

    void ResourceLoader::Pause()
    {
    }

    void ResourceLoader::Resume()
    {
    }

    void ResourceLoader::AddPath(std::string_view path)
    {
        this->Mount("", path);
    }

    void ResourceLoader::DeletePath(std::string_view path)
    {
        this->Unmount("", path);
    }

    void ResourceLoader::IsInPath(std::string_view paht)
    {
    }

    void ResourceLoader::Mount(std::string_view virtual_path, std::string_view phy_path)
    {
    }

    void ResourceLoader::Unmount(std::string_view virtual_path, std::string_view phy_path)
    {
    }

    ResIdentifierPtr ResourceLoader::Open(std::string_view path)
    {
        return ResIdentifierPtr();
    }

    std::string ResourceLoader::Locate(std::string_view name)
    {
        return std::string();
    }

    uint64_t ResourceLoader::Timestamp(std::string_view name)
    {
        return 0;
    }

    std::string ResourceLoader::Guid(std::string_view name)
    {
        return std::string();
    }

    std::string ResourceLoader::AbsPath(std::string_view path)
    {
        std::string path_str(path);
        std::filesystem::path new_path(path_str);
        if(!new_path.is_absolute())
        {
            std::filesystem::path full_path = std::filesystem::path(_exe_path) / new_path;
            std::error_code error_code;
            if(!std::filesystem::exists(full_path, error_code))
            {
                try
                {
                    full_path = std::filesystem::current_path() / new_path;
                }
                catch(...)
                {
                    full_path = new_path;
                }

                if(!std::filesystem::exists(full_path, error_code))
                {
                    return "";
                }
            }
            new_path = full_path;
        }
        std::string ret = new_path.string();
    
#if defined TORCH_PLATFORM_WINDOWS
        std::replace(ret.begin(), ret.end(), '\\', '/');
#endif
        return ret;
    }

    void ResourceLoader::Update()
    {
    }
    std::string ResourceLoader::RealPath(std::string_view path)
    {
       std::string package_path;
       std::string password;
       std::string path_in_package;
       return this->RealPath(path, package_path, password, path_in_package);
    }
    std::string ResourceLoader::RealPath(std::string_view path, std::string &package_path, std::string &password, std::string &path_in_package)
    {
        package_path = "";
        password = "";
        path_in_package = "";

        std::string abs_path = this->AbsPath(path);
        if (abs_path.back() != '/')
        {
            abs_path.push_back('/');
        }

        return abs_path;
    }

    void ResourceLoader::LoadingThreadFunc()
    {
    }
};