#ifndef __TORCH_CORE_RESOURCE_LOADER_HPP__
#define __TORCH_CORE_RESOURCE_LOADER_HPP__

#pragma once

#include <istream>
#include <string>
#include <vector>

#include <TML/Thread.hpp>


namespace Torch
{
	class TORCH_CORE_API ResourceDesc
	{
	public:
		virtual ~ResourceDesc() noexcept;
		virtual uint64_t Type() const = 0;
		virtual bool StateLess() const = 0;
		virtual std::shared_ptr<void> CreateResource()
		{
			return std::shared_ptr<void>();
		}

		virtual void SubThreadStage() = 0;
		virtual void MainThreadStage() = 0;
		virtual bool HasSubTreadStage() const = 0;
		virtual bool Match(ResourceDesc const& other) const = 0;
		virtual void CopyDataFrom(ResourceDesc const& other) = 0;
		virtual std::shared_ptr<void> CloneResourceFrom(std::shared_ptr<void> const& other) = 0;
		virtual std::shared_ptr<void> Resource() const = 0;
	};

	FWD_CLASS_SPTR(ResourceDesc);

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
		std::vector<std::tuple<uint64_t, uint32_t, std::string>> _paths;
		std::mutex _paths_mutex;
		std::mutex _loaded_mutex;
		std::mutex _loading_mutex;
		std::vector<std::pair<ResourceDescPtr, std::weak_ptr<void>>> _loaded_res;
		std::vector<std::pair<ResourceDescPtr, std::shared_ptr<volatile LoadStatus>>> _loading_res;
		bool _non_empty_loading_res_queue = false;
		std::condition_variable _loading_res_queue_cv;
		std::mutex _loading_res_queue_mutex;
		std::vector<std::pair<ResourceDesc, std::shared_ptr<volatile LoadStatus>>> _loading_res_queue;

		std::future<void> _loading_thread;
		volatile bool _quite{ false };
	};
};

#endif // !__TORCH_CORE_RESOURCE_LOADER_HPP__
