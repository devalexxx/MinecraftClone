//
// Created by Alex Clorennec on 12/07/2025.
//

#include "Common/Utils/ThreadPool.h"

namespace Mcc
{

    ThreadPool::ThreadPool(const size_t threadCount) : mThreadCount(threadCount), mStop(false)
    {
        for(size_t i = 0; i < mThreadCount; i++)
        {
            mThreads.emplace_back([this]
            {
                while (true)
                {
                    std::unique_lock lock(mMutex);
                    mCv.wait(lock, [this] { return !mTasks.empty() || mStop; });
                    if(mStop)
                        return;
                    std::function<void()> task = std::move(mTasks.front());
                    mTasks.pop();
                    lock.unlock();
                    task();
                  }
              });
        }
    }

    ThreadPool::~ThreadPool()
    {
        std::unique_lock lock(mMutex);
        mStop = true;
        lock.unlock();
        mCv.notify_all();

        for(auto& th: mThreads)
        {
            th.join();
        }
    }



}