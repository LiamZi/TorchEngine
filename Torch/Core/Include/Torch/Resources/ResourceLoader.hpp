#ifndef __TORCH_CORE_RESOURCE_LOADER_HPP__
#define __TORCH_CORE_RESOURCE_LOADER_HPP__

#pragma once

#include <istream>
#include <string>
#include <vector>

#include <TML/Thread.hpp>

namespace Torch
{
    FWD_CLASS_SPTR(ResIdentifier);
	FWD_CLASS_SPTR(ResDesc);

	class TORCH_CORE_API ResDesc
	{
	public:
		virtual ~ResDesc() noexcept {}
		virtual uint16_t Type() const { return 0; }
		virtual bool StateLess() const { return false; }
		virtual void SubThreadStage() {};
		virtual void MainThreadStage() {};
		virtual bool HasSubThreadStage() const { return false; }

		virtual std::shared_ptr<void> CreateRes()
		{
			return std::shared_ptr<void>();
		}

		
	};

	class TORCH_CORE_API ResourceLoader
	{
	private:
		static std::unique_ptr<ResourceLoader> _res_loader_instance;

		enum LoadStatus
		{
			LS_LOADING,
			LS_COMPLETE,
			LS_REMOVED
		};

		std::string _exe_path;
		std::string _local_path;
		std::vector<std::tuple<uint64_t, uint32_t, std::string, std::string>> _paths;
		std::mutex _paths_mutex;
		std::mutex _loaded_mutex;
		std::mutex _loading_mutex;

		bool _non_empty_loading_res_queue = false;
		std::condition_variable _loading_res_queue_cv;
		std::mutex _loading_res_queue_mutex;
		std::vector<std::pair<ResDescPtr, std::shared_ptr<volatile LoadStatus>>> _loading_res_queue;
		std::vector<std::pair<ResDescPtr, std::weak_ptr<void>>> _loaded_res;
		std::vector<std::pair<ResDescPtr, std::shared_ptr<volatile LoadStatus>>> _loading_res;

		std::future<void> _loading_thread;
		volatile bool _quit{ false };

	public:
		ResourceLoader();
		~ResourceLoader();

		static ResourceLoader& Instance();
		static void Destroy();

		void Pause();
		void Resume();

		void AddPath(std::string_view path);
		void DeletePath(std::string_view path);
		bool IsInPath(std::string_view paht);

        void Mount(std::string_view virtual_path, std::string_view phy_path);
        void Unmount(std::string_view virtual_path, std::string_view phy_path);

        ResIdentifierPtr Open(std::string_view path);
        std::string Locate(std::string_view name);
        uint64_t Timestamp(std::string_view name);
        std::string AbsPath(std::string_view path);

        void Update();

		const std::string& LocalFolder() const
		{
			return _local_path;
		}

    private:
        std::string RealPath(std::string_view path);
        std::string RealPath(std::string_view path, std::string &package_path, std::string &password, std::string &path_in_package);
        std::string Guid(std::string_view name);
        void LoadingThreadFunc();

	};
};

#endif // !__TORCH_CORE_RESOURCE_LOADER_HPP__
