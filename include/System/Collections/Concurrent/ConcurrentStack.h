
#pragma once

#include "System/Object.h"
#include <stack>
#include <mutex>
#include <vector>

namespace System::Collections::Concurrent
{
    template<typename T>
    class ConcurrentStack : public System::Object
    {
    private:
        std::stack<T> stack;
        mutable std::mutex mutex;

    public:
        ConcurrentStack() = default;
        ConcurrentStack(const std::vector<T>& collection);

        void Push(const T& item);
        void PushRange(const std::vector<T>& items);
        bool TryPop(T& result);
        int TryPopRange(std::vector<T>& items);
        int TryPopRange(std::vector<T>& items, int startIndex, int count);
        bool TryPeek(T& result);

        // Properties
        bool GetIsEmpty() const;
        int GetCount() const;

        // IEnumerable
        std::vector<T> ToArray();
        void CopyTo(std::vector<T>& array, int index);
    };
}
