
#pragma once

#include <functional>

namespace System {
namespace Threading {

/**
 * @brief Represents the method that executes on a Thread.
 * 
 * This delegate type represents a method that can be executed by a thread.
 * It takes no parameters and returns void.
 */
using ThreadStart = std::function<void()>;

/**
 * @brief Represents the method that executes on a Thread with a parameter.
 * 
 * This delegate type represents a method that can be executed by a thread
 * and accepts a single object parameter.
 */
using ParameterizedThreadStart = std::function<void(void*)>;

/**
 * @brief Represents the method that handles calls from a Timer.
 * 
 * This delegate type represents a callback method that is called by a Timer
 * when the timer interval elapses.
 */
using TimerCallback = std::function<void(void*)>;

/**
 * @brief Represents a callback method to be executed by a thread pool thread.
 * 
 * This delegate type represents a method that can be queued for execution
 * by the thread pool.
 */
using WaitCallback = std::function<void(void*)>;

/**
 * @brief Represents a method to be called when a WaitHandle is signaled or times out.
 * 
 * This delegate type represents a callback method that is called when a wait
 * operation completes, either because the waited object was signaled or because
 * the wait timed out.
 * 
 * @param state User-defined state object
 * @param timedOut true if the wait timed out; false if the wait handle was signaled
 */
using WaitOrTimerCallback = std::function<void(void*, bool)>;

} // namespace Threading
} // namespace System
