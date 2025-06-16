
#pragma once

namespace System {
namespace Threading {

/**
 * @brief Specifies the apartment state of a Thread.
 * 
 * The apartment state determines how COM objects are accessed and managed
 * within a thread. This is primarily used for COM interoperability.
 */
enum class ApartmentState : int {
    /**
     * @brief The Thread will create and enter a single-threaded apartment.
     * 
     * In STA mode, COM objects are accessed through a single thread, and
     * calls from other threads are marshaled through window messages.
     */
    STA = 0,

    /**
     * @brief The Thread will create and enter a multithreaded apartment.
     * 
     * In MTA mode, COM objects can be accessed directly from multiple threads,
     * and the COM runtime handles synchronization.
     */
    MTA = 1,

    /**
     * @brief The apartment state has not been set.
     * 
     * This is the default state before any apartment state is explicitly set.
     */
    Unknown = 2
};

} // namespace Threading
} // namespace System
