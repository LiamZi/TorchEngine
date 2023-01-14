#ifndef __TML_THREAD_HPP__
#define __TML_THREAD_HPP__

#pragma once

#include <TML/Util.hpp>

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
#include <vector>

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

        auto task = std::packaged_task<result()>(std::move(func));
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

        FWD_CLASS_SPTR(ThreadInfo);

        class CommitData final : public std::enable_shared_from_this<CommitData>
        {
        private:
            size_t _num_min_cached_threads;
            size_t _num_max_cached_threads;
            std::mutex _mutex;
            bool _general_cleanup = false;
            std::vector<ThreadInfoPtr> _threads;


        public:
            CommitData(size_t num_min_cached_threads, size_t num_max_cached_threads);
            ~CommitData();

            Property<CommitData, size_t> NumMinCachedThreads;
            Property<CommitData, size_t> NumMaxCachedThreads;

            void AddWaitingThreads(size_t num);

            template<typename T>
            std::future<INVOKE(T)> QueueThread(T func)
            {
                using result_t = INVOKE(T);

                auto task = MakeSharedPtr<std::packaged_task<result_t()>>(std::move(func));
                auto ret = task->get_futrue();
                
                std::lock_guard<std::mutex> lock(_mutex);

                if(_threads.empty())
                {
                    this->AddWaitingThreadsLocked(lock, 1);
                }

                auto info = std::move(_threads.front());
                _threads.erase(_threads.begin());
                info->WakeUp([task]() {(*task)(); });

                return ret;
            }
            

        private:
            size_t getNumMinCachedThreads() const noexcept
            {
                return _num_min_cached_threads;
            }

            void setNumMinCachedThreads(size_t num) noexcept
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if(num > NumMinCachedThreads)
                {
                    this->AddWaitingThreadsLocked(lock, num - NumMinCachedThreads);
                }
                else
                {
                    for(size_t i = 0; i < NumMinCachedThreads - num; ++i)
                    {
                        _threads.back()->Kill();
                        _threads.pop_back();
                    }
                }
                _num_min_cached_threads = num;
            }

            size_t getNumMaxCachedThreads() const noexcept
            {
                return _num_max_cached_threads;
            }

            void setNumMaxCachedThreads(size_t num) noexcept
            {
                _num_max_cached_threads = num;
            }

            void AddWaitingThreadsLocked(std::lock_guard<std::mutex> const &lock, size_t num);

            static void WaitFunction(ThreadInfoPtr const &info);
        };

    private:
        CommitDataPtr _data;

    public:
        ThreadPool(size_t num_min_cached_threads, size_t num_max_cached_thread);

        template <typename T>
        std::future<INVOKE(T)> QueueThread(T func)
        {
            return _data->QueueThread(func);
        }

        Property<ThreadPool, size_t> NumMinCachedThreads;
        Property<ThreadPool, size_t> NumMaxCachedThreads;

    private:
        size_t getNumMinCachedThreads() const noexcept
        {
            return _data->NumMinCachedThreads;
        }

        void setNumMinCachedThreads(size_t num) noexcept
        {
            _data->NumMinCachedThreads = num;
        }

        size_t getNumMaxCachedThreads() const noexcept
        {
            return _data->NumMaxCachedThreads;
        }

        void setNumMaxCachedThreads(size_t num) noexcept
        {
            _data->NumMaxCachedThreads = num;
        }
        
    };

    
};

#endif