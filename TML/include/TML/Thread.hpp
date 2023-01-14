#ifndef __TML_THREAD_HPP__
#define __TML_THREAD_HPP__

#pragma once

#include <condition_variable>
#include <functional>

#ifdef TORCH_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4355)
#endif
#include <future>
#ifdef TORCH_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <mutex>
#include <thread>

namespace Torch
{

#if defined(TORCH_PLATFORM_ANDROID)
#define INVOKE(X) typename std::result_of<X()>::type
#else
#define INVOKE(X) typename std::invoke_result<X>::type
#endif

    template <typename T>
    inline std::future<INVOKE(T)> CreateThread(T func)
    {
        using result = INVOKE(T);

        auto task = std::packaged_task<result_t()>(std::move(func));
        auto ret = task.get_future();
        auto thread = std::thread(std::move(task));
        thread.detach();

        return ret;
    }

    class ThreadPool 
    {
        class CommitData;
        FWD_CLASS_SPTR(CommitData);
        struct ThreadInfo 
        {
            std::function<void()> _func;
            bool _wake_up = false;
            std::mutex _wake_up_mutex;
            std::condition_variable _wake_up_cond;
            std::weak_ptr<CommitData> _data;

            explicit ThreadInfo(CommitData &data) noexcept;

            void WakeUp(std::function<void()> func);
            void Kill();
        };

        class CommitData final : public std::enable_shared_from_this<CommitData>
        {
        private:
            size_t _num_min_cached_threads;
            size_t _num_max_cached_threads;


        public:
            CommitData(size_t num_min_cached_threads, size_t num_max_cached_threads);
            ~CommitData();
        };

    private:
        CommitDataPtr _data;

    public:
        ThreadPool(size_t num_min_cached_threads, size_t num_max_cached_thread);

    };
};

#endif