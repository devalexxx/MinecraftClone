//
// Created by Alex Clorennec on 12/07/2025.
//

#ifndef MCC_COMMON_UTILS_THREAD_POOL_H
#define MCC_COMMON_UTILS_THREAD_POOL_H

#include <functional>
#include <future>
#include <queue>
#include <thread>

namespace Mcc
{

    class ThreadPool
    {
        public:
            ThreadPool(size_t threadCount = std::thread::hardware_concurrency());
            ~ThreadPool();

            template<typename F, typename... Args>
            auto ExecuteTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;


        private:
            size_t mThreadCount;
            std::vector<std::thread> mThreads;
            std::queue<std::function<void()>> mTasks;
            std::mutex mMutex;
            std::condition_variable mCv;
            bool mStop;
    };

}

#include "Common/Utils/ThreadPool.inl"

#endif
