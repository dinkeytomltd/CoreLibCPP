
#pragma once

// CoreLib.Threading - System.Threading namespace
// This header provides a convenient way to include all Threading module functionality

#include "CoreLib/Base.h"

// Core threading primitives
#include "System/Threading/Thread.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/WaitHandle.h"

// Synchronization primitives
#include "System/Threading/Mutex.h"
#include "System/Threading/Semaphore.h"
#include "System/Threading/SemaphoreSlim.h"
#include "System/Threading/Monitor.h"
#include "System/Threading/EventWaitHandle.h"
#include "System/Threading/AutoResetEvent.h"
#include "System/Threading/ManualResetEvent.h"
#include "System/Threading/ManualResetEventSlim.h"
#include "System/Threading/ReaderWriterLockSlim.h"

// Lock-free primitives
#include "System/Threading/SpinLock.h"
#include "System/Threading/SpinWait.h"
#include "System/Threading/Interlocked.h"

// Cancellation
#include "System/Threading/CancellationToken.h"
#include "System/Threading/CancellationTokenSource.h"
#include "System/Threading/CancellationTokenRegistration.h"

// Timer
#include "System/Threading/Timer.h"

// Task Parallel Library
#include "System/Threading/Tasks/Task.h"
#include "System/Threading/Tasks/TaskStatus.h"
#include "System/Threading/Tasks/TaskCreationOptions.h"
#include "System/Threading/Tasks/TaskContinuationOptions.h"
#include "System/Threading/Tasks/TaskAwaiter.h"
#include "System/Threading/Tasks/TaskScheduler.h"
#include "System/Threading/Tasks/Parallel.h"

// Concurrent collections
#include "System/Collections/Concurrent/ConcurrentQueue.h"
#include "System/Collections/Concurrent/ConcurrentStack.h"
#include "System/Collections/Concurrent/ConcurrentDictionary.h"
