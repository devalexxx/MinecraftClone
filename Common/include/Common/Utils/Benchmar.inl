//
// Created by Alex Clorennec on 14/07/2025.
//

namespace Mcc::_
{

    template<typename Tag>
    TimeBenchmark<Tag>::~TimeBenchmark()
    {
        MCC_LOG_DEBUG("[TimeBench - {}] called {} times, total: {:.2f}, avg: {:.5f}", Tag::name(), callCount, timeSpent, timeSpent / callCount);
    }

    template<typename Tag>
    void TimeBenchmark<Tag>::AddCall(const float elapsed)
    {
        std::lock_guard l(mutex);
        timeSpent += elapsed;
        ++callCount;
    }


}
