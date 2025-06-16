
#pragma once

/**
 * @file Linq.hpp
 * @brief Main header file for LINQ (Language Integrated Query) functionality
 * 
 * This header provides comprehensive LINQ support for CoreLib.Cpp with 1:1 compatibility
 * with C# LINQ semantics. It includes all core interfaces, implementations, and extension
 * methods for data querying and transformation.
 * 
 * Key Features:
 * - Lazy evaluation with deferred execution
 * - Full template-based type safety
 * - Integration with existing CoreLib collections
 * - C# LINQ method compatibility
 * - Exception handling integration
 * - Performance optimized iterators
 * 
 * Usage:
 * @code
 * #include "System/Linq/Linq.hpp"
 * using namespace System::Linq;
 * 
 * std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 * 
 * auto result = Enumerable::From(numbers)
 *     .Where([](int x) { return x % 2 == 0; })
 *     .Select<int>([](int x) { return x * x; })
 *     .Take(3)
 *     .ToList();
 * @endcode
 */

// Core interfaces
#include "IEnumerable.hpp"
#include "IEnumerator.hpp"

// Iterator implementations
#include "LinqIterators.hpp"

// Wrapper and main functionality
#include "EnumerableWrapper.hpp"
#include "Enumerable.hpp"

namespace System::Linq {
    
    /**
     * @brief Convenience function to create an enumerable from various sources
     * 
     * This function provides a convenient way to start LINQ chains from different
     * data sources including std::vector, initializer lists, and existing enumerables.
     * 
     * @tparam T The element type
     * @param source The source container or enumerable
     * @return EnumerableWrapper<T> A LINQ-enabled wrapper
     */
    template<typename T>
    EnumerableWrapper<T> AsEnumerable(const std::vector<T>& source) {
        return Enumerable::From(source);
    }

    template<typename T>
    EnumerableWrapper<T> AsEnumerable(std::initializer_list<T> source) {
        return Enumerable::From(source);
    }

    template<typename T>
    EnumerableWrapper<T> AsEnumerable(const IEnumerable<T>& source) {
        return Enumerable::From(source);
    }

} // namespace System::Linq

/**
 * @brief Global convenience macros for LINQ operations (optional)
 * 
 * These macros provide a more C#-like syntax for common LINQ operations.
 * They are optional and can be disabled by defining CORELIB_NO_LINQ_MACROS.
 */
#ifndef CORELIB_NO_LINQ_MACROS

#define LINQ_FROM(source) System::Linq::Enumerable::From(source)
#define LINQ_RANGE(start, count) System::Linq::Enumerable::Range(start, count)
#define LINQ_REPEAT(element, count) System::Linq::Enumerable::Repeat(element, count)
#define LINQ_EMPTY(T) System::Linq::Enumerable::Empty<T>()

#endif // CORELIB_NO_LINQ_MACROS
