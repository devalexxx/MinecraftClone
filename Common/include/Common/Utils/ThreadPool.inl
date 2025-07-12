//
// Created by Alex Clorennec on 12/07/2025.
//

namespace Mcc
{

    template<typename F, typename... Args>
    auto ThreadPool::ExecuteTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        std::unique_lock lock(mMutex);
        mTasks.emplace([task]()-> void { (*task)(); });
        lock.unlock();
        mCv.notify_one();

        return res;
    }


}
