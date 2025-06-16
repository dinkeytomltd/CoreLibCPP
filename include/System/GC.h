
#pragma once

#include "System/Object.h"
#include "System/GC/GcPtr.hpp"
#include "System/GC/GcWeakPtr.hpp"
#include "System/GC/GcRef.hpp"
#include <atomic>
#include <chrono>
#include <vector>
#include <functional>

// Forward declare Boehm GC functions
extern "C" {
    void GC_init();
    void GC_gcollect();
    size_t GC_get_heap_size();
    size_t GC_get_free_bytes();
    void GC_set_finalize_on_demand(int);
    void GC_invoke_finalizers();
    void* GC_malloc(size_t);
    void GC_free(void*);
}

namespace System
{
    /// <summary>
    /// Provides garbage collection services compatible with .NET GC API.
    /// Implements hybrid approach using reference counting with Boehm GC for cycle collection.
    /// </summary>
    class GarbageCollector
    {
    public:
        /// <summary>
        /// Garbage collection generation enumeration.
        /// </summary>
        enum class Generation
        {
            Gen0 = 0,
            Gen1 = 1,
            Gen2 = 2,
            Large = 3
        };

        /// <summary>
        /// Garbage collection mode enumeration.
        /// </summary>
        enum class CollectionMode
        {
            Default = 0,
            Forced = 1,
            Optimized = 2
        };

        /// <summary>
        /// Garbage collection notification status.
        /// </summary>
        enum class NotificationStatus
        {
            Succeeded = 0,
            Failed = 1,
            Canceled = 2,
            Timeout = 3,
            NotApplicable = 4
        };

    private:
        static std::atomic<bool> s_initialized;
        static std::atomic<size_t> s_total_allocations;
        static std::atomic<size_t> s_total_collections;
        static std::atomic<size_t> s_gen0_collections;
        static std::atomic<size_t> s_gen1_collections;
        static std::atomic<size_t> s_gen2_collections;
        static std::atomic<bool> s_finalization_suppressed;
        static std::vector<std::function<void()>> s_finalizers;

        /// <summary>
        /// Initialize Boehm GC if not already initialized.
        /// </summary>
        static void EnsureInitialized();

    public:
        /// <summary>
        /// Forces an immediate garbage collection of all generations.
        /// </summary>
        static void Collect();

        /// <summary>
        /// Forces garbage collection of the specified generation and all lower generations.
        /// </summary>
        /// <param name="generation">The generation to collect.</param>
        static void Collect(int generation);

        /// <summary>
        /// Forces garbage collection with specified mode.
        /// </summary>
        /// <param name="generation">The generation to collect.</param>
        /// <param name="mode">The collection mode.</param>
        static void Collect(int generation, CollectionMode mode);

        /// <summary>
        /// Forces garbage collection with specified mode and blocking behavior.
        /// </summary>
        /// <param name="generation">The generation to collect.</param>
        /// <param name="mode">The collection mode.</param>
        /// <param name="blocking">Whether to block until collection completes.</param>
        static void Collect(int generation, CollectionMode mode, bool blocking);

        /// <summary>
        /// Forces garbage collection with specified mode, blocking behavior, and compaction.
        /// </summary>
        /// <param name="generation">The generation to collect.</param>
        /// <param name="mode">The collection mode.</param>
        /// <param name="blocking">Whether to block until collection completes.</param>
        /// <param name="compacting">Whether to compact the heap.</param>
        static void Collect(int generation, CollectionMode mode, bool blocking, bool compacting);

        /// <summary>
        /// Returns the current best available approximation of the number of bytes currently allocated in managed memory.
        /// </summary>
        /// <param name="forceFullCollection">Whether to force a full collection before returning.</param>
        /// <returns>The number of bytes currently allocated.</returns>
        static long GetTotalMemory(bool forceFullCollection);

        /// <summary>
        /// Suspends the current thread until the thread that is processing the queue of finalizers has emptied that queue.
        /// </summary>
        static void WaitForPendingFinalizers();

        /// <summary>
        /// Requests that the system not call the finalizer for the specified object.
        /// </summary>
        /// <param name="obj">The object whose finalizer should not be called.</param>
        static void SuppressFinalize(Object* obj);

        /// <summary>
        /// Requests that the system call the finalizer for the specified object for which SuppressFinalize has previously been called.
        /// </summary>
        /// <param name="obj">The object whose finalizer should be called.</param>
        static void ReRegisterForFinalize(Object* obj);

        /// <summary>
        /// Returns the number of times garbage collection has occurred for the specified generation of objects.
        /// </summary>
        /// <param name="generation">The generation of objects for which the garbage collection count is to be returned.</param>
        /// <returns>The number of times garbage collection has occurred for the specified generation.</returns>
        static int CollectionCount(int generation);

        /// <summary>
        /// Returns the maximum number of generations that the system currently supports.
        /// </summary>
        /// <returns>The maximum number of generations.</returns>
        static int MaxGeneration();

        /// <summary>
        /// Returns the current generation number of the target of a specified weak reference.
        /// </summary>
        /// <param name="obj">The object whose generation is to be determined.</param>
        /// <returns>The current generation number of the target.</returns>
        static int GetGeneration(Object* obj);

        /// <summary>
        /// Returns the current generation number of the target of a specified weak reference.
        /// </summary>
        /// <param name="wo">The weak reference whose target's generation is to be determined.</param>
        /// <returns>The current generation number of the target.</returns>
        template<typename T>
        static int GetGeneration(const Memory::GcWeakPtr<T>& wo);

        /// <summary>
        /// Retrieves the number of bytes that are currently thought to be allocated.
        /// </summary>
        /// <returns>The number of bytes that are currently allocated.</returns>
        static long GetTotalAllocatedBytes();

        /// <summary>
        /// Retrieves the number of bytes that are currently thought to be allocated, with optional precise measurement.
        /// </summary>
        /// <param name="precise">Whether to force a precise measurement.</param>
        /// <returns>The number of bytes that are currently allocated.</returns>
        static long GetTotalAllocatedBytes(bool precise);

        /// <summary>
        /// Informs the runtime of a large allocation of unmanaged memory that should be taken into account when scheduling garbage collection.
        /// </summary>
        /// <param name="bytesAllocated">The incremental amount of unmanaged memory that has been allocated.</param>
        static void AddMemoryPressure(long bytesAllocated);

        /// <summary>
        /// Informs the runtime that unmanaged memory has been released and no longer needs to be taken into account when scheduling garbage collection.
        /// </summary>
        /// <param name="bytesAllocated">The amount of unmanaged memory that has been released.</param>
        static void RemoveMemoryPressure(long bytesAllocated);

        /// <summary>
        /// Returns the total number of milliseconds elapsed during garbage collections.
        /// </summary>
        /// <returns>The total time spent in garbage collections.</returns>
        static long GetTotalPauseDuration();

        /// <summary>
        /// Enables or disables concurrent garbage collection.
        /// </summary>
        /// <param name="enabled">Whether concurrent GC should be enabled.</param>
        static void SetConcurrent(bool enabled);

        /// <summary>
        /// Gets whether concurrent garbage collection is enabled.
        /// </summary>
        /// <returns>True if concurrent GC is enabled.</returns>
        static bool IsConcurrent();

        /// <summary>
        /// Enables or disables server garbage collection mode.
        /// </summary>
        /// <param name="enabled">Whether server GC should be enabled.</param>
        static void SetServerMode(bool enabled);

        /// <summary>
        /// Gets whether server garbage collection mode is enabled.
        /// </summary>
        /// <returns>True if server GC is enabled.</returns>
        static bool IsServerMode();

        /// <summary>
        /// Register a finalizer callback.
        /// </summary>
        /// <param name="finalizer">The finalizer function to register.</param>
        static void RegisterFinalizer(std::function<void()> finalizer);

        /// <summary>
        /// Get garbage collection statistics.
        /// </summary>
        struct Statistics
        {
            size_t TotalAllocations;
            size_t TotalCollections;
            size_t Gen0Collections;
            size_t Gen1Collections;
            size_t Gen2Collections;
            size_t HeapSize;
            size_t FreeBytes;
            std::chrono::milliseconds TotalPauseTime;
        };

        /// <summary>
        /// Get current GC statistics.
        /// </summary>
        /// <returns>Current garbage collection statistics.</returns>
        static Statistics GetStatistics();

        /// <summary>
        /// Reset GC statistics.
        /// </summary>
        static void ResetStatistics();

        /// <summary>
        /// Enable or disable GC logging.
        /// </summary>
        /// <param name="enabled">Whether to enable logging.</param>
        static void SetLogging(bool enabled);

        /// <summary>
        /// Set GC heap size limits.
        /// </summary>
        /// <param name="maxHeapSize">Maximum heap size in bytes.</param>
        static void SetHeapSizeLimit(size_t maxHeapSize);

        /// <summary>
        /// Get current heap size.
        /// </summary>
        /// <returns>Current heap size in bytes.</returns>
        static size_t GetHeapSize();

        /// <summary>
        /// Get amount of free memory in heap.
        /// </summary>
        /// <returns>Free memory in bytes.</returns>
        static size_t GetFreeMemory();

    private:
        // Static class - no instances allowed
        GarbageCollector() = delete;
        ~GarbageCollector() = delete;
        GarbageCollector(const GarbageCollector&) = delete;
        GarbageCollector& operator=(const GarbageCollector&) = delete;
    };

    /// <summary>
    /// Alias for GarbageCollector to provide .NET-compatible naming.
    /// </summary>
    using GC = GarbageCollector;
}
