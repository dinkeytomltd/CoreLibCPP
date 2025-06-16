
#include "System/GC.h"
#include <mutex>
#include <chrono>
#include <iostream>

// Include Boehm GC headers
#ifdef CORELIB_GC_BOEHM
extern "C" {
    #include <gc/gc.h>
}
#endif

namespace System
{
    // Static member definitions
    std::atomic<bool> GarbageCollector::s_initialized{false};
    std::atomic<size_t> GarbageCollector::s_total_allocations{0};
    std::atomic<size_t> GarbageCollector::s_total_collections{0};
    std::atomic<size_t> GarbageCollector::s_gen0_collections{0};
    std::atomic<size_t> GarbageCollector::s_gen1_collections{0};
    std::atomic<size_t> GarbageCollector::s_gen2_collections{0};
    std::atomic<bool> GarbageCollector::s_finalization_suppressed{false};
    std::vector<std::function<void()>> GarbageCollector::s_finalizers;

    void GarbageCollector::EnsureInitialized()
    {
        static std::once_flag init_flag;
        std::call_once(init_flag, []() {
#ifdef CORELIB_GC_BOEHM
            GC_init();
            GC_set_finalize_on_demand(1);
#endif
            s_initialized.store(true, std::memory_order_release);
        });
    }

    void GarbageCollector::Collect()
    {
        EnsureInitialized();
        
#ifdef CORELIB_GC_BOEHM
        GC_gcollect();
#endif
        
        s_total_collections.fetch_add(1, std::memory_order_relaxed);
        s_gen0_collections.fetch_add(1, std::memory_order_relaxed);
        s_gen1_collections.fetch_add(1, std::memory_order_relaxed);
        s_gen2_collections.fetch_add(1, std::memory_order_relaxed);
    }

    void GarbageCollector::Collect(int generation)
    {
        EnsureInitialized();
        
        // For Boehm GC, we don't have generational collection
        // So we treat any generation request as a full collection
#ifdef CORELIB_GC_BOEHM
        GC_gcollect();
#endif
        
        s_total_collections.fetch_add(1, std::memory_order_relaxed);
        
        if (generation >= 0)
            s_gen0_collections.fetch_add(1, std::memory_order_relaxed);
        if (generation >= 1)
            s_gen1_collections.fetch_add(1, std::memory_order_relaxed);
        if (generation >= 2)
            s_gen2_collections.fetch_add(1, std::memory_order_relaxed);
    }

    void GarbageCollector::Collect(int generation, CollectionMode mode)
    {
        // Mode is ignored for now - Boehm GC doesn't support different modes
        Collect(generation);
    }

    void GarbageCollector::Collect(int generation, CollectionMode mode, bool blocking)
    {
        // Blocking is ignored for now - Boehm GC is always blocking
        Collect(generation, mode);
    }

    void GarbageCollector::Collect(int generation, CollectionMode mode, bool blocking, bool compacting)
    {
        // Compacting is ignored for now - Boehm GC handles compaction internally
        Collect(generation, mode, blocking);
    }

    long GarbageCollector::GetTotalMemory(bool forceFullCollection)
    {
        EnsureInitialized();
        
        if (forceFullCollection)
        {
            Collect();
        }
        
#ifdef CORELIB_GC_BOEHM
        return static_cast<long>(GC_get_heap_size());
#else
        return static_cast<long>(s_total_allocations.load(std::memory_order_acquire));
#endif
    }

    void GarbageCollector::WaitForPendingFinalizers()
    {
        EnsureInitialized();
        
#ifdef CORELIB_GC_BOEHM
        GC_invoke_finalizers();
#endif
        
        // Execute registered finalizers
        for (auto& finalizer : s_finalizers)
        {
            try
            {
                finalizer();
            }
            catch (...)
            {
                // Ignore finalizer exceptions
            }
        }
        s_finalizers.clear();
    }

    void GarbageCollector::SuppressFinalize(Object* obj)
    {
        if (obj)
        {
            obj->SetFinalizationSuppressed(true);
        }
    }

    void GarbageCollector::ReRegisterForFinalize(Object* obj)
    {
        if (obj)
        {
            obj->SetFinalizationSuppressed(false);
        }
    }

    int GarbageCollector::CollectionCount(int generation)
    {
        switch (generation)
        {
            case 0: return static_cast<int>(s_gen0_collections.load(std::memory_order_acquire));
            case 1: return static_cast<int>(s_gen1_collections.load(std::memory_order_acquire));
            case 2: return static_cast<int>(s_gen2_collections.load(std::memory_order_acquire));
            default: return 0;
        }
    }

    int GarbageCollector::MaxGeneration()
    {
        return 2; // Standard .NET has 3 generations (0, 1, 2)
    }

    int GarbageCollector::GetGeneration(Object* obj)
    {
        // For simplicity, we'll return generation 2 for all objects
        // In a real implementation, this would track object generations
        return obj ? 2 : -1;
    }

    template<typename T>
    int GarbageCollector::GetGeneration(const Memory::GcWeakPtr<T>& wo)
    {
        auto locked = wo.Lock();
        return locked ? GetGeneration(locked.Get()) : -1;
    }

    long GarbageCollector::GetTotalAllocatedBytes()
    {
        return GetTotalAllocatedBytes(false);
    }

    long GarbageCollector::GetTotalAllocatedBytes(bool precise)
    {
        EnsureInitialized();
        
        if (precise)
        {
            Collect();
        }
        
#ifdef CORELIB_GC_BOEHM
        return static_cast<long>(GC_get_heap_size());
#else
        return static_cast<long>(s_total_allocations.load(std::memory_order_acquire));
#endif
    }

    void GarbageCollector::AddMemoryPressure(long bytesAllocated)
    {
        if (bytesAllocated > 0)
        {
            s_total_allocations.fetch_add(static_cast<size_t>(bytesAllocated), std::memory_order_relaxed);
        }
    }

    void GarbageCollector::RemoveMemoryPressure(long bytesAllocated)
    {
        if (bytesAllocated > 0)
        {
            s_total_allocations.fetch_sub(static_cast<size_t>(bytesAllocated), std::memory_order_relaxed);
        }
    }

    long GarbageCollector::GetTotalPauseDuration()
    {
        // TODO: Implement pause time tracking
        return 0;
    }

    void GarbageCollector::SetConcurrent(bool enabled)
    {
        // TODO: Implement concurrent GC control
    }

    bool GarbageCollector::IsConcurrent()
    {
        // TODO: Implement concurrent GC status
        return false;
    }

    void GarbageCollector::SetServerMode(bool enabled)
    {
        // TODO: Implement server mode control
    }

    bool GarbageCollector::IsServerMode()
    {
        // TODO: Implement server mode status
        return false;
    }

    void GarbageCollector::RegisterFinalizer(std::function<void()> finalizer)
    {
        s_finalizers.push_back(std::move(finalizer));
    }

    GarbageCollector::Statistics GarbageCollector::GetStatistics()
    {
        EnsureInitialized();
        
        Statistics stats;
        stats.TotalAllocations = s_total_allocations.load(std::memory_order_acquire);
        stats.TotalCollections = s_total_collections.load(std::memory_order_acquire);
        stats.Gen0Collections = s_gen0_collections.load(std::memory_order_acquire);
        stats.Gen1Collections = s_gen1_collections.load(std::memory_order_acquire);
        stats.Gen2Collections = s_gen2_collections.load(std::memory_order_acquire);
        
#ifdef CORELIB_GC_BOEHM
        stats.HeapSize = GC_get_heap_size();
        stats.FreeBytes = GC_get_free_bytes();
#else
        stats.HeapSize = stats.TotalAllocations;
        stats.FreeBytes = 0;
#endif
        
        stats.TotalPauseTime = std::chrono::milliseconds(GetTotalPauseDuration());
        
        return stats;
    }

    void GarbageCollector::ResetStatistics()
    {
        s_total_allocations.store(0, std::memory_order_release);
        s_total_collections.store(0, std::memory_order_release);
        s_gen0_collections.store(0, std::memory_order_release);
        s_gen1_collections.store(0, std::memory_order_release);
        s_gen2_collections.store(0, std::memory_order_release);
    }

    void GarbageCollector::SetLogging(bool enabled)
    {
        // TODO: Implement GC logging
    }

    void GarbageCollector::SetHeapSizeLimit(size_t maxHeapSize)
    {
        // TODO: Implement heap size limits
    }

    size_t GarbageCollector::GetHeapSize()
    {
        EnsureInitialized();
        
#ifdef CORELIB_GC_BOEHM
        return GC_get_heap_size();
#else
        return s_total_allocations.load(std::memory_order_acquire);
#endif
    }

    size_t GarbageCollector::GetFreeMemory()
    {
        EnsureInitialized();
        
#ifdef CORELIB_GC_BOEHM
        return GC_get_free_bytes();
#else
        return 0;
#endif
    }
}
