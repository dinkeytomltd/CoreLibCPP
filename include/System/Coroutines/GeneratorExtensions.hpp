
#pragma once

#include "Generator.hpp"
#include <vector>

// Forward declarations to avoid circular dependencies
namespace System::Linq {
    template<typename T> class EnumerableWrapper;
    template<typename T> class IEnumerable;
}

namespace System::Coroutines {

    /**
     * @brief Extension methods to integrate Generator<T> with LINQ operations
     */
    class GeneratorExtensions {
    public:
        /**
         * @brief Convert Generator<T> to vector for LINQ operations
         */
        template<typename T>
        static std::vector<T> ToVector(Generator<T>&& generator) {
            std::vector<T> result;
            for (auto&& item : generator) {
                result.push_back(item);
            }
            return result;
        }

        /**
         * @brief Create Generator<T> from range
         */
        template<typename T>
        static Generator<T> Range(T start, int count) {
            for (int i = 0; i < count; ++i) {
                co_yield static_cast<T>(start + i);
            }
        }

        /**
         * @brief Create Generator<T> that repeats a value
         */
        template<typename T>
        static Generator<T> Repeat(T value, int count) {
            for (int i = 0; i < count; ++i) {
                co_yield value;
            }
        }

        /**
         * @brief Create empty Generator<T>
         */
        template<typename T>
        static Generator<T> Empty() {
            co_return;
        }

        /**
         * @brief Flatten nested generators (yield from equivalent)
         */
        template<typename T>
        static Generator<T> SelectMany(Generator<Generator<T>>&& source) {
            for (auto&& inner : source) {
                for (auto&& item : inner) {
                    co_yield item;
                }
            }
        }
    };

} // namespace System::Coroutines
