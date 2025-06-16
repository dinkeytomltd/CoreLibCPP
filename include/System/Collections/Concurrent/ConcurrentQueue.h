
#pragma once

#include "System/Object.h"
#include <queue>
#include <mutex>
#include <vector>

namespace System::Collections::Concurrent
{
    template<typename T>
    class ConcurrentQueue : public System::Object
    {
    private:
        std::queue<T> queue;
        mutable std::mutex mutex;

    public:
        ConcurrentQueue() = default;
        ConcurrentQueue(const std::vector<T>& collection);

        void Enqueue(const T& item);
        bool TryDequeue(T& result);
        bool TryPeek(T& result);

        // Properties
        bool GetIsEmpty() const;
        int GetCount() const;

        // IEnumerable
        std::vector<T> ToArray();
        void CopyTo(std::vector<T>& array, int index);
    };
}
