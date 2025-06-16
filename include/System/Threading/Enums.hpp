
#pragma once

namespace System {
namespace Threading {

enum class TaskStatus {
    Created,
    WaitingForActivation,
    WaitingToRun,
    Running,
    WaitingForChildrenToComplete,
    RanToCompletion,
    Canceled,
    Faulted
};

enum class TaskCreationOptions {
    None = 0,
    PreferFairness = 1,
    LongRunning = 2,
    AttachedToParent = 4,
    DenyChildAttach = 8,
    HideScheduler = 16,
    RunContinuationsAsynchronously = 64
};

enum class TaskContinuationOptions {
    None = 0,
    PreferFairness = 1,
    LongRunning = 2,
    AttachedToParent = 4,
    DenyChildAttach = 8,
    HideScheduler = 16,
    LazyCancellation = 32,
    RunContinuationsAsynchronously = 64,
    NotOnRanToCompletion = 65536,
    NotOnFaulted = 131072,
    NotOnCanceled = 262144,
    OnlyOnRanToCompletion = 393216,
    OnlyOnFaulted = 524288,
    OnlyOnCanceled = 1048576,
    ExecuteSynchronously = 524288
};

enum class CancellationTokenState {
    NotCanceled,
    Canceled
};

// Bitwise operators for TaskCreationOptions
inline TaskCreationOptions operator|(TaskCreationOptions lhs, TaskCreationOptions rhs) {
    return static_cast<TaskCreationOptions>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline TaskCreationOptions operator&(TaskCreationOptions lhs, TaskCreationOptions rhs) {
    return static_cast<TaskCreationOptions>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline TaskCreationOptions operator^(TaskCreationOptions lhs, TaskCreationOptions rhs) {
    return static_cast<TaskCreationOptions>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
}

inline TaskCreationOptions operator~(TaskCreationOptions options) {
    return static_cast<TaskCreationOptions>(~static_cast<int>(options));
}

// Bitwise operators for TaskContinuationOptions
inline TaskContinuationOptions operator|(TaskContinuationOptions lhs, TaskContinuationOptions rhs) {
    return static_cast<TaskContinuationOptions>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline TaskContinuationOptions operator&(TaskContinuationOptions lhs, TaskContinuationOptions rhs) {
    return static_cast<TaskContinuationOptions>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline TaskContinuationOptions operator^(TaskContinuationOptions lhs, TaskContinuationOptions rhs) {
    return static_cast<TaskContinuationOptions>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
}

inline TaskContinuationOptions operator~(TaskContinuationOptions options) {
    return static_cast<TaskContinuationOptions>(~static_cast<int>(options));
}

} // namespace Threading
} // namespace System
