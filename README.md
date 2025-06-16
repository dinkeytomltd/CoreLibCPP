
# CoreLibCPP

A comprehensive C++ implementation of .NET Core's System.Threading components, providing cross-platform threading primitives and utilities.

## Overview

CoreLibCPP is a modern C++ library that brings the power and familiarity of .NET's System.Threading namespace to C++ developers. This library provides thread-safe, high-performance implementations of essential threading components.

## Features

### Threading Components
- **AsyncLocal<T>** - Thread-local storage that flows with async operations
- **ThreadLocal<T>** - Thread-specific storage with lazy initialization
- **Barrier** - Synchronization primitive for coordinating multiple threads
- **CountdownEvent** - Synchronization primitive that blocks until a count reaches zero
- **ReaderWriterLockSlim** - High-performance reader-writer lock
- **Volatile** - Provides volatile read/write operations
- **Task** - Asynchronous operation representation with continuations
- **TaskCompletionSource<T>** - Manual control over Task completion
- **CancellationToken** - Cooperative cancellation mechanism
- **Threading Delegates** - Function pointer abstractions for callbacks

### Enumerations
- **TaskStatus** - Task execution states
- **TaskCreationOptions** - Task creation configuration
- **TaskContinuationOptions** - Task continuation behavior
- **CancellationTokenState** - Cancellation token states

### Structures
- **SpinWait** - Efficient spinning wait primitive
- **CancellationTokenRegistration** - Cancellation callback registration

## Build Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- pthread library (Linux/macOS)

## Building

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Usage Example

```cpp
#include "System/Threading/Task.hpp"
#include "System/Threading/AsyncLocal.hpp"
#include "System/Threading/ThreadLocal.hpp"

using namespace System::Threading;

int main() {
    // AsyncLocal example
    AsyncLocal<int> asyncValue;
    asyncValue.set(42);
    
    // Task example
    auto task = Task::run([]() {
        return 100;
    });
    
    auto result = task.get_result();
    std::cout << "Task result: " << result << std::endl;
    
    return 0;
}
```

## Project Structure

```
CoreLibCPP/
├── include/
│   └── System/
│       └── Threading/
│           ├── AsyncLocal.hpp
│           ├── ThreadLocal.hpp
│           ├── Barrier.hpp
│           ├── CountdownEvent.hpp
│           ├── ReaderWriterLockSlim.hpp
│           ├── Volatile.hpp
│           ├── Task.hpp
│           ├── TaskCompletionSource.hpp
│           ├── CancellationToken.hpp
│           ├── Delegates.hpp
│           ├── Enums.hpp
│           └── Structures.hpp
├── src/
│   └── System/
│       └── Threading/
│           ├── AsyncLocal.cpp
│           ├── ThreadLocal.cpp
│           ├── Barrier.cpp
│           ├── CountdownEvent.cpp
│           ├── ReaderWriterLockSlim.cpp
│           ├── Task.cpp
│           ├── TaskCompletionSource.cpp
│           └── CancellationToken.cpp
├── tests/
│   └── (unit tests for each component)
├── examples/
│   └── (usage examples)
├── CMakeLists.txt
└── README.md
```

## Threading Components Details

### AsyncLocal<T>
Provides thread-local storage that automatically flows with asynchronous operations, ensuring data isolation across different execution contexts.

### ThreadLocal<T>
Thread-specific storage with lazy initialization support, allowing each thread to maintain its own instance of data.

### Barrier
Synchronization primitive that enables multiple threads to work cooperatively on an algorithm in parallel phases.

### CountdownEvent
A synchronization primitive that blocks threads until its internal count reaches zero, useful for coordinating completion of multiple operations.

### ReaderWriterLockSlim
High-performance lock that allows multiple readers or a single writer, optimized for scenarios with frequent reads and infrequent writes.

### Task System
Comprehensive task-based asynchronous programming model with support for:
- Task creation and execution
- Continuations and chaining
- Cancellation support
- Result handling
- Exception propagation

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## Compatibility

This library aims to provide API compatibility with .NET's System.Threading namespace while leveraging modern C++ features for optimal performance.
