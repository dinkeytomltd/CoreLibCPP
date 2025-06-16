
#pragma once

#include <atomic>
#include <type_traits>

namespace System {
namespace Threading {

/**
 * @brief Provides static methods for volatile operations with memory ordering guarantees.
 * 
 * The Volatile class provides methods for performing volatile read and write operations
 * that ensure proper memory ordering semantics across threads. These operations prevent
 * compiler and CPU optimizations that could reorder memory accesses.
 */
class Volatile {
public:
    /**
     * @brief Performs a volatile read of the specified location.
     * 
     * @tparam T The type of the value to read
     * @param location Reference to the location to read from
     * @return The value read from the location
     */
    template<typename T>
    static T Read(const T& location) {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable for volatile operations");
        
        // Use atomic operations to ensure proper memory ordering
        const std::atomic<T>* atomic_location = reinterpret_cast<const std::atomic<T>*>(&location);
        return atomic_location->load(std::memory_order_acquire);
    }

    /**
     * @brief Performs a volatile write to the specified location.
     * 
     * @tparam T The type of the value to write
     * @param location Reference to the location to write to
     * @param value The value to write
     */
    template<typename T>
    static void Write(T& location, T value) {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable for volatile operations");
        
        // Use atomic operations to ensure proper memory ordering
        std::atomic<T>* atomic_location = reinterpret_cast<std::atomic<T>*>(&location);
        atomic_location->store(value, std::memory_order_release);
    }

private:
    // Static class - no instances allowed
    Volatile() = delete;
    ~Volatile() = delete;
    Volatile(const Volatile&) = delete;
    Volatile& operator=(const Volatile&) = delete;
};

} // namespace Threading
} // namespace System
