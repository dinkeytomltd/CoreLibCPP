
#pragma once

#include <atomic>

namespace System::Threading
{
    class Interlocked
    {
    public:
        // Increment/Decrement
        static int Increment(std::atomic<int>& location);
        static long Increment(std::atomic<long>& location);
        static int Decrement(std::atomic<int>& location);
        static long Decrement(std::atomic<long>& location);

        // Add
        static int Add(std::atomic<int>& location1, int value);
        static long Add(std::atomic<long>& location1, long value);

        // Exchange
        static int Exchange(std::atomic<int>& location1, int value);
        static long Exchange(std::atomic<long>& location1, long value);
        static float Exchange(std::atomic<float>& location1, float value);
        static double Exchange(std::atomic<double>& location1, double value);
        
        template<typename T>
        static T Exchange(std::atomic<T>& location1, T value);

        // CompareExchange
        static int CompareExchange(std::atomic<int>& location1, int value, int comparand);
        static long CompareExchange(std::atomic<long>& location1, long value, long comparand);
        static float CompareExchange(std::atomic<float>& location1, float value, float comparand);
        static double CompareExchange(std::atomic<double>& location1, double value, double comparand);
        
        template<typename T>
        static T CompareExchange(std::atomic<T>& location1, T value, T comparand);

        // Read
        static long Read(std::atomic<long>& location);
        static double Read(std::atomic<double>& location);

        // Memory barriers
        static void MemoryBarrier();
        static void MemoryBarrierProcessWide();
    };
}
