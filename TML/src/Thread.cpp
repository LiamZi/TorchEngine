#include <TML/TML.hpp>
#include <TML/Thread.hpp>

namespace Torch
{

    ThreadPool::ThreadInfo::ThreadInfo(CommitData &data) noexcept
        : _data(data.shared_from_this())
    {
    }

    void ThreadPool::ThreadInfo::WakeUp(std::function<void()> func)
    {
        _func = std::move(func);

        std::lock_guard<std::mutex> lock(_wake_up_mutex);
        _wake_up = true;
        _wake_up_cond.notify_one();
    }

    void ThreadPool::ThreadInfo::Kill()
    {
        std::lock_guard<std::mutex> lock(_wake_up_mutex);
        _func = std::function<void()>();
        _wake_up = true;
        _wake_up_cond.notify_one();
    }

    ThreadPool::CommitData::CommitData(size_t num_min_cached_threads, size_t num_max_cached_threads)
        : _num_min_cached_threads(num_min_cached_threads)
        , _num_max_cached_threads(num_max_cached_threads)
        , NumMinCachedThreads{this, &ThreadPool::CommitData::getNumMinCachedThreads, &ThreadPool::CommitData::setNumMinCachedThreads}
        , NumMaxCachedThreads{this, &ThreadPool::CommitData::getNumMaxCachedThreads, &ThreadPool::CommitData::setNumMaxCachedThreads}
    {
    }

    ThreadPool::CommitData::~CommitData()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        _general_cleanup = true;

        for(auto &info : _threads)
        {
            info->Kill();
        }
        _threads.clear();
    }

    void ThreadPool::CommitData::AddWaitingThreads(size_t num)
    {
       std::lock_guard<std::mutex> lock(_mutex);
       this->AddWaitingThreadsLocked(lock, num);
    }

    void ThreadPool::CommitData::AddWaitingThreadsLocked(std::lock_guard<std::mutex> const &lock, size_t num)
    {
        (void) lock;

        for(size_t i = 0; i < num; ++i)
        {
            auto &info = _threads.emplace_back(MakeSharedPtr<ThreadInfo>(*this));
            auto thread = std::thread([info]() { WaitFunction(info); });
            thread.detach();
        }
    }


    void ThreadPool::CommitData::WaitFunction(ThreadInfoPtr const &info)
    {
        for(;;)
        {
            {
                auto data = info->_data.lock();
                if(data)
                {
                    std::unique_lock<std::mutex> lock(info->_wake_up_mutex);

                    while(!info->_wake_up && !data->_general_cleanup)
                    {
                        info->_wake_up_cond.wait(lock);
                    }

                    if(!info->_func || data->_general_cleanup)
                    {
                        return;
                    }

                    info->_wake_up = false;
                }
                else
                {
                    return;
                }
            }

            info->_func();
            info->_func = std::function<void()>();
            {
                auto data = info->_data.lock();
                if(data)
                {
                    std::lock_guard<std::mutex> lock(data->_mutex);

                    if(data->_general_cleanup)
                    {
                        return;
                    }

                    if(data->_threads.size() < data->NumMaxCachedThreads)
                    {
                        data->_threads.push_back(info);
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    return;
                }
            }
        }
    }

    ThreadPool::ThreadPool(size_t num_min_cached_threads, size_t num_max_cached_thread)
        : _data{MakeSharedPtr<CommitData>(num_min_cached_threads, num_max_cached_thread)}
        , NumMinCachedThreads{this, &ThreadPool::getNumMinCachedThreads, &ThreadPool::setNumMinCachedThreads}
        , NumMaxCachedThreads{this, &ThreadPool::getNumMaxCachedThreads, &ThreadPool::setNumMaxCachedThreads}
    {
        assert(num_max_cached_thread >= num_min_cached_threads);

        _data->AddWaitingThreads(num_min_cached_threads);
    }
};