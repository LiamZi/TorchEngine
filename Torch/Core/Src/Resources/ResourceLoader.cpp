#include <Torch/Resources/ResourceLoader.hpp>
#include <Torch/Interfaces/Context.hpp>

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

    ResourceLoader::~ResourceLoader() noexcept = default;


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
    }

    void ResourceLoader::DeletePath(std::string_view path)
    {
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
        return std::string();
    }
    void ResourceLoader::Update()
    {
    }
    std::string ResourceLoader::RealPath(std::string_view path)
    {
        return std::string();
    }
    std::string ResourceLoader::RealPath(std::string_view path, std::string &package_path, std::string &password, std::string &path_in_package)
    {
        return std::string();
    }

    void ResourceLoader::LoadingThreadFunc()
    {
    }
};