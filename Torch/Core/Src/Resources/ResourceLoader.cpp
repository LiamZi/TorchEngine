#include <Torch/Torch.hpp>

#include <Torch/Interfaces/Context.hpp>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <TML/Guid.hpp>
#include <TML/Hash.hpp>
#include <TML/ResIdentifier.hpp>

#include <Torch/Resources/ResourceLoader.hpp>


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
		_paths.emplace_back(0, 0, "", "");

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

    bool ResourceLoader::IsInPath(std::string_view path)
    {
        std::string_view virtual_path = "";
        
        std::lock_guard<std::mutex> lock(_paths_mutex);
        std::string real_path = this->RealPath(path);
        if(!real_path.empty())
        {
            std::string virtual_path_str(virtual_path);
            if(!virtual_path.empty() && (virtual_path.back() != '/'))
            {
                virtual_path_str.push_back('/');
            }
            
            const uint64_t virtual_path_hash = HashValue(virtual_path_str);

            bool found = false;
            for(auto const &path : _paths)
            {
                if((std::get<0>(path) == virtual_path_hash) && (std::get<2>(path) == real_path))
                {
                    found = true;
                    break;
                }
            }
            return found;
        }
        else
        {
            return false;
        }   

    }

    void ResourceLoader::Mount(std::string_view virtual_path, std::string_view phy_path)
    {
        std::lock_guard<std::mutex> lock(_paths_mutex);

        std::string package_path;
        std::string password;
        std::string path_in_package;
        std::string real_path = this->RealPath(phy_path, package_path, password, path_in_package);
        if(!real_path.empty())
        {
            std::string virtual_path_str(virtual_path);
            if(!virtual_path.empty() && (virtual_path.back() != '/'))
            {
                virtual_path_str.push_back('/');
            }

            const uint64_t virtual_path_hash = HashValue(virtual_path_str);
            bool found = false;
            for(const auto &path : _paths)
            {
                if((std::get<0>(path) == virtual_path_hash) && (std::get<2>(path) == real_path))
                {
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                _paths.emplace_back(virtual_path_hash, static_cast<uint32_t>(virtual_path_str.size()), real_path, std::move(""));
            }
        }
    }

    void ResourceLoader::Unmount(std::string_view virtual_path, std::string_view phy_path)
    {
        std::lock_guard<std::mutex> lock(_paths_mutex);
        std::string real_path = this->RealPath(phy_path);
        if(!real_path.empty())
        {
            std::string virtual_path_str(virtual_path);
            if(!virtual_path.empty() && (virtual_path.back() != '/'))
            {
                virtual_path_str.push_back('/');
            }

            const uint64_t virtual_path_hash = HashValue(virtual_path_str);

            for(auto iter = _paths.begin(); iter != _paths.end(); ++iter)
            {
                if((std::get<0>(*iter)  == virtual_path_hash) && (std::get<2>(*iter) == real_path))
                {
                    _paths.erase(iter);
                    break;
                }
            }
        }
    }

    ResIdentifierPtr ResourceLoader::Open(std::string_view name)
    {
       if(name.empty()) return ResIdentifierPtr();

#if defined(TORCH_PLATFORM_ANDROID)
#elif defined(TORCH_PLATFORM_IOS)
#else
        std::lock_guard<std::mutex> lock(_paths_mutex);
        for(const auto &path : _paths)
        {
            if((std::get<1>(path) != 0) || (HashRange(name.begin(), name.begin() + std::get<0>(path)) == std::get<0>(path)))
            {
                std::string res_name(std::get<2>(path) + std::string(name.substr(std::get<1>(path))));
#if defined TORCH_PLATFORM_WINDOWS
                std::replace(res_name.begin(), res_name.end(), '\\', '/');
#endif
                std::filesystem::path res_path(res_name);
                std::error_code ec;
                if(std::filesystem::exists(res_path, ec))
                {
                    const uint64_t timestamp = std::filesystem::last_write_time(res_path).time_since_epoch().count();
                    return MakeSharedPtr<ResIdentifier>(name, timestamp, MakeSharedPtr<std::ifstream>(res_name.c_str(), std::ios_base::binary));

                }
            }

            
            if((std::get<1>(path) == 0) && std::filesystem::path(name.begin(), name.end()).is_absolute())
            {
                break;
            }
        }

#endif
        return ResIdentifierPtr();
    }

    std::string ResourceLoader::Locate(std::string_view name)
    {
        if(name.empty()) return "";

#if defined(TORCH_PLATFORM_ANDROID)
#elif defined(TORCH_PLATFORM_IOS)
#else
        std::lock_guard<std::mutex> lock(_paths_mutex);
        for(const auto &path : _paths)
        {
            if((std::get<0>(path) != 0) || (HashRange(name.begin(), name.begin() + std::get<1>(path)) == std::get<0>(path)))
            {
                std::string res_name(std::get<2>(path) + std::string(name.substr(std::get<1>(path))));
#if defined TORCH_PLATFORM_WINDOWS
                std::replace(res_name.begin(), res_name.end(), '\\', '/');
#endif
                std::error_code ec;
                if(std::filesystem::exists(std::filesystem::path(res_name), ec))
                {
                    return res_name;
                }
            }

            if((std::get<1>(path) == 0) && std::filesystem::path(name.begin(), name.end()).is_absolute())
            {
                break;
            }
        }
#endif
        return "";
    }

    uint64_t ResourceLoader::Timestamp(std::string_view name)
    {
        uint64_t timestamp = 0;
        auto res_path = this->Locate(name);
        if(!res_path.empty())
        {
#if !defined(TORCH_PLATFORM_ANDROID)
            timestamp = std::filesystem::last_write_time(res_path).time_since_epoch().count();
#endif
        }
        return timestamp;
    }

    std::string ResourceLoader::Guid(std::string_view name)
    {
        return GUID::RandomId();
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
        // std::vector<std::pair<ResourceLoader>>
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
        while(!_quit)
        {
            std::vector<std::pair<ResDescPtr, std::shared_ptr<volatile LoadStatus>>> loading_res_queue_copy;

            std::unique_lock<std::mutex> lock(_loading_res_queue_mutex);
            _loading_res_queue_cv.wait(lock, [this] { return _non_empty_loading_res_queue; });

            loading_res_queue_copy.swap(_loading_res_queue);
            _non_empty_loading_res_queue = false;

            for(auto &res_pair : loading_res_queue_copy)
            {
                if(LS_LOADING == *res_pair.second)
                {
                    res_pair.first->SubThreadStage();
                    *res_pair.second = LS_COMPLETE;
                }
            }

            Sleep(10);
        }
    }
};