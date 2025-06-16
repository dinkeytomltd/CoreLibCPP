
#pragma once

#include <coroutine>
#include <exception>
#include <iterator>
#include <memory>
#include <type_traits>
#include <chrono>
#include <iostream>
#include <vector>

// Forward declarations to avoid circular dependencies
namespace System::Linq {
    template<typename T> class IEnumerable;
    template<typename T> class IEnumerator;
    template<typename T> class EnumerableWrapper;
}

namespace System {
    class InvalidOperationException;
    class NotSupportedException;
}

namespace System::Coroutines {

    /**
     * @brief C++20 coroutine-based generator providing C# yield return/yield break equivalent functionality
     * 
     * This class implements a lazy-evaluated generator using C++20 coroutines with C# styled interface.
     * It integrates seamlessly with the existing LINQ system and provides exception safety.
     * 
     * Usage:
     * @code
     * Generator<int> GetNumbers()
     * {
     *     co_yield 1;
     *     co_yield 2;
     *     co_yield 3;
     *     co_return; // yield break equivalent
     * }
     * 
     * auto result = GetNumbers()
     *     .Where([](int x) { return x % 2 == 0; })
     *     .Select<int>([](int x) { return x * x; })
     *     .ToList();
     * @endcode
     */
    template<typename T>
    class Generator {
    public:
        // Forward declaration for promise type
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        /**
         * @brief Promise type for C++20 coroutine implementation
         * 
         * Manages coroutine state, value yielding, and exception handling
         */
        struct promise_type {
            T current_value{};
            std::exception_ptr exception{};

            Generator get_return_object() {
                return Generator{handle_type::from_promise(*this)};
            }

            std::suspend_always initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }

            void unhandled_exception() {
                exception = std::current_exception();
            }

            std::suspend_always yield_value(T value) {
                current_value = std::move(value);
                return {};
            }

            void return_void() {}
        };

        /**
         * @brief STL-compatible iterator for Generator<T>
         * 
         * Provides input iterator semantics with proper exception propagation
         */
        class iterator {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            explicit iterator(handle_type h) : handle_(h) {
                if (handle_ && !handle_.done()) {
                    advance();
                }
            }

            iterator() : handle_(nullptr) {}

            // Input iterator requirements
            bool operator==(const iterator& other) const {
                return handle_ == other.handle_ || (is_end() && other.is_end());
            }

            bool operator!=(const iterator& other) const {
                return !(*this == other);
            }

            const T& operator*() const {
                if (!handle_ || handle_.done()) {
                    throw std::runtime_error("Iterator is at end");
                }
                return handle_.promise().current_value;
            }

            const T* operator->() const {
                return &operator*();
            }

            iterator& operator++() {
                if (!handle_ || handle_.done()) {
                    throw std::runtime_error("Cannot advance iterator past end");
                }
                advance();
                return *this;
            }

            iterator operator++(int) {
                iterator temp = *this;
                ++(*this);
                return temp;
            }

        private:
            handle_type handle_;

            bool is_end() const {
                return !handle_ || handle_.done();
            }

            void advance() {
                if (handle_) {
                    handle_.resume();
                    if (handle_.promise().exception) {
                        std::rethrow_exception(handle_.promise().exception);
                    }
                }
            }
        };

        // Constructor and destructor
        explicit Generator(handle_type h) : handle_(h) {}

        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& other) noexcept : handle_(other.handle_) {
            other.handle_ = nullptr;
        }

        Generator& operator=(Generator&& other) noexcept {
            if (this != &other) {
                if (handle_) {
                    handle_.destroy();
                }
                handle_ = other.handle_;
                other.handle_ = nullptr;
            }
            return *this;
        }

        ~Generator() {
            if (handle_) {
                handle_.destroy();
            }
        }

        // Range-based for loop support
        iterator begin() {
            return iterator{handle_};
        }

        iterator end() {
            return iterator{};
        }

        // Const versions
        iterator begin() const {
            return iterator{handle_};
        }

        iterator end() const {
            return iterator{};
        }

        // Check if generator is valid
        bool valid() const {
            return handle_ != nullptr;
        }

        /**
         * @brief Fluent LINQ interface - Where
         */
        template<typename TPredicate>
        Generator<T> Where(TPredicate predicate) && {
            for (auto&& item : *this) {
                if (predicate(item)) {
                    co_yield item;
                }
            }
        }

        /**
         * @brief Fluent LINQ interface - Select
         */
        template<typename TResult, typename TSelector>
        Generator<TResult> Select(TSelector selector) && {
            for (auto&& item : *this) {
                co_yield selector(item);
            }
        }

        /**
         * @brief Fluent LINQ interface - Take
         */
        Generator<T> Take(int count) && {
            int taken = 0;
            for (auto&& item : *this) {
                if (taken >= count) break;
                co_yield item;
                ++taken;
            }
        }

        /**
         * @brief Fluent LINQ interface - Skip
         */
        Generator<T> Skip(int count) && {
            int skipped = 0;
            for (auto&& item : *this) {
                if (skipped < count) {
                    ++skipped;
                    continue;
                }
                co_yield item;
            }
        }

        /**
         * @brief Materialize to vector
         */
        std::vector<T> ToVector() && {
            std::vector<T> result;
            for (auto&& item : *this) {
                result.push_back(item);
            }
            return result;
        }

    private:
        handle_type handle_;
    };

    // Forward declaration for LINQ integration
    template<typename T> class GeneratorEnumerable;

} // namespace System::Coroutines
