
#pragma once

// Compiler detection macros
#if defined(_MSC_VER)
    #define CORELIB_COMPILER_MSVC 1
    #define CORELIB_COMPILER_GCC 0
    #define CORELIB_COMPILER_CLANG 0
#elif defined(__clang__)
    #define CORELIB_COMPILER_MSVC 0
    #define CORELIB_COMPILER_GCC 0
    #define CORELIB_COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define CORELIB_COMPILER_MSVC 0
    #define CORELIB_COMPILER_GCC 1
    #define CORELIB_COMPILER_CLANG 0
#else
    #define CORELIB_COMPILER_MSVC 0
    #define CORELIB_COMPILER_GCC 0
    #define CORELIB_COMPILER_CLANG 0
#endif

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define CORELIB_PLATFORM_WINDOWS 1
    #define CORELIB_PLATFORM_LINUX 0
    #define CORELIB_PLATFORM_MACOS 0
#elif defined(__linux__)
    #define CORELIB_PLATFORM_WINDOWS 0
    #define CORELIB_PLATFORM_LINUX 1
    #define CORELIB_PLATFORM_MACOS 0
#elif defined(__APPLE__)
    #define CORELIB_PLATFORM_WINDOWS 0
    #define CORELIB_PLATFORM_LINUX 0
    #define CORELIB_PLATFORM_MACOS 1
#else
    #define CORELIB_PLATFORM_WINDOWS 0
    #define CORELIB_PLATFORM_LINUX 0
    #define CORELIB_PLATFORM_MACOS 0
#endif

// Compiler-specific includes
#if CORELIB_COMPILER_MSVC
    #include <intrin.h>
    #include <windows.h>
    #pragma warning(push)
    #pragma warning(disable: 4996) // Disable deprecated function warnings
#elif CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #include <climits>
    #include <cstdint>
    #if defined(__x86_64__) || defined(__i386__)
        #include <x86intrin.h>
    #endif
#endif

// Force inline macros
#if CORELIB_COMPILER_MSVC
    #define CORELIB_FORCE_INLINE __forceinline
    #define CORELIB_NOINLINE __declspec(noinline)
#elif CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #define CORELIB_FORCE_INLINE inline __attribute__((always_inline))
    #define CORELIB_NOINLINE __attribute__((noinline))
#else
    #define CORELIB_FORCE_INLINE inline
    #define CORELIB_NOINLINE
#endif

// Thread local storage
#if CORELIB_COMPILER_MSVC
    #define CORELIB_THREAD_LOCAL __declspec(thread)
#elif CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #define CORELIB_THREAD_LOCAL __thread
#else
    #define CORELIB_THREAD_LOCAL thread_local
#endif

// Alignment macros
#if CORELIB_COMPILER_MSVC
    #define CORELIB_ALIGN(x) __declspec(align(x))
#elif CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #define CORELIB_ALIGN(x) __attribute__((aligned(x)))
#else
    #define CORELIB_ALIGN(x) alignas(x)
#endif

// Likely/unlikely hints
#if CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #define CORELIB_LIKELY(x) __builtin_expect(!!(x), 1)
    #define CORELIB_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define CORELIB_LIKELY(x) (x)
    #define CORELIB_UNLIKELY(x) (x)
#endif

// Unreachable macro
#if CORELIB_COMPILER_MSVC
    #define CORELIB_UNREACHABLE() __assume(0)
#elif CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #define CORELIB_UNREACHABLE() __builtin_unreachable()
#else
    #define CORELIB_UNREACHABLE() do { } while(0)
#endif

// Pragma macros
#if CORELIB_COMPILER_MSVC
    #define CORELIB_PRAGMA(x) __pragma(x)
    #define CORELIB_WARNING_PUSH() __pragma(warning(push))
    #define CORELIB_WARNING_POP() __pragma(warning(pop))
    #define CORELIB_WARNING_DISABLE_MSVC(x) __pragma(warning(disable: x))
    #define CORELIB_WARNING_DISABLE_GCC(x)
    #define CORELIB_WARNING_DISABLE_CLANG(x)
#elif CORELIB_COMPILER_GCC
    #define CORELIB_PRAGMA(x) _Pragma(#x)
    #define CORELIB_WARNING_PUSH() _Pragma("GCC diagnostic push")
    #define CORELIB_WARNING_POP() _Pragma("GCC diagnostic pop")
    #define CORELIB_WARNING_DISABLE_MSVC(x)
    #define CORELIB_WARNING_DISABLE_GCC(x) _Pragma("GCC diagnostic ignored " x)
    #define CORELIB_WARNING_DISABLE_CLANG(x)
#elif CORELIB_COMPILER_CLANG
    #define CORELIB_PRAGMA(x) _Pragma(#x)
    #define CORELIB_WARNING_PUSH() _Pragma("clang diagnostic push")
    #define CORELIB_WARNING_POP() _Pragma("clang diagnostic pop")
    #define CORELIB_WARNING_DISABLE_MSVC(x)
    #define CORELIB_WARNING_DISABLE_GCC(x)
    #define CORELIB_WARNING_DISABLE_CLANG(x) _Pragma("clang diagnostic ignored " x)
#else
    #define CORELIB_PRAGMA(x)
    #define CORELIB_WARNING_PUSH()
    #define CORELIB_WARNING_POP()
    #define CORELIB_WARNING_DISABLE_MSVC(x)
    #define CORELIB_WARNING_DISABLE_GCC(x)
    #define CORELIB_WARNING_DISABLE_CLANG(x)
#endif

// Exception handling
#if CORELIB_COMPILER_MSVC
    #define CORELIB_NOEXCEPT throw()
#else
    #define CORELIB_NOEXCEPT noexcept
#endif

// Standard library compatibility
#if CORELIB_COMPILER_MSVC && _MSC_VER < 1920 // Before VS 2019
    #define CORELIB_CONSTEXPR_IF constexpr
#else
    #define CORELIB_CONSTEXPR_IF constexpr
#endif

// Memory barriers and atomic operations
#if CORELIB_COMPILER_MSVC
    #define CORELIB_MEMORY_BARRIER() _ReadWriteBarrier()
    #define CORELIB_COMPILER_BARRIER() _ReadWriteBarrier()
#elif CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #define CORELIB_MEMORY_BARRIER() __sync_synchronize()
    #define CORELIB_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
#else
    #define CORELIB_MEMORY_BARRIER() std::atomic_thread_fence(std::memory_order_seq_cst)
    #define CORELIB_COMPILER_BARRIER() std::atomic_thread_fence(std::memory_order_seq_cst)
#endif

// CPU pause instruction
#if CORELIB_COMPILER_MSVC
    #define CORELIB_CPU_PAUSE() _mm_pause()
#elif CORELIB_COMPILER_GCC || CORELIB_COMPILER_CLANG
    #if defined(__x86_64__) || defined(__i386__)
        #define CORELIB_CPU_PAUSE() __builtin_ia32_pause()
    #elif defined(__aarch64__) || defined(__arm__)
        #define CORELIB_CPU_PAUSE() __asm__ __volatile__("yield" ::: "memory")
    #else
        #define CORELIB_CPU_PAUSE() do { } while(0)
    #endif
#else
    #define CORELIB_CPU_PAUSE() do { } while(0)
#endif

// Cleanup at end of file for MSVC
#if CORELIB_COMPILER_MSVC
    #pragma warning(pop)
#endif
