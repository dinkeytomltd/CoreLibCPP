
#pragma once

#include "IEnumerable.hpp"
#include "IEnumerator.hpp"
#include "EnumerableWrapper.hpp"
#include "ListEnumerableAdapter.hpp"
#include "../Collections/Generic/List.h"
#include "../InvalidOperationException.h"
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <type_traits>

// Forward declaration for Generator integration
namespace System::Coroutines {
    template<typename T> class Generator;
    template<typename T> class GeneratorEnumerable;
    class GeneratorExtensions;
}

namespace System::Linq {

    /**
     * @brief Static class providing LINQ extension methods for IEnumerable<T>
     * 
     * This class provides all standard LINQ operations with deferred execution semantics.
     * All operations return lazy-evaluated sequences except for materialization operations.
     */
    class Enumerable {
    public:
        // ==================== FILTERING OPERATIONS ====================

        /**
         * @brief Filters a sequence of values based on a predicate
         */
        template<typename T, typename TPredicate>
        static EnumerableWrapper<T> Where(const IEnumerable<T>& source, TPredicate predicate) {
            auto sourcePtr = std::shared_ptr<IEnumerable<T>>(const_cast<IEnumerable<T>*>(&source), [](IEnumerable<T>*){});
            return EnumerableWrapper<T>(sourcePtr).Where(predicate);
        }

        template<typename T, typename TPredicate>
        static EnumerableWrapper<T> Where(const std::vector<T>& source, TPredicate predicate) {
            return EnumerableWrapper<T>(source).Where(predicate);
        }

        // ==================== PROJECTION OPERATIONS ====================

        /**
         * @brief Projects each element of a sequence into a new form
         */
        template<typename T, typename TResult, typename TSelector>
        static EnumerableWrapper<TResult> Select(const IEnumerable<T>& source, TSelector selector) {
            auto sourcePtr = std::shared_ptr<IEnumerable<T>>(const_cast<IEnumerable<T>*>(&source), [](IEnumerable<T>*){});
            return EnumerableWrapper<T>(sourcePtr).template Select<TResult>(selector);
        }

        template<typename T, typename TResult, typename TSelector>
        static EnumerableWrapper<TResult> Select(const std::vector<T>& source, TSelector selector) {
            return EnumerableWrapper<T>(source).template Select<TResult>(selector);
        }

        /**
         * @brief Projects each element to an IEnumerable<T> and flattens the resulting sequences into one sequence
         */
        template<typename T, typename TResult, typename TSelector>
        static EnumerableWrapper<TResult> SelectMany(const IEnumerable<T>& source, TSelector selector) {
            auto result = std::make_shared<System::Collections::Generic::List<TResult>>();
            auto enumerator = source.GetEnumerator();
            
            while (enumerator->MoveNext()) {
                auto innerSequence = selector(enumerator->Current());
                auto innerEnumerator = innerSequence.GetEnumerator();
                while (innerEnumerator->MoveNext()) {
                    result->Add(innerEnumerator->Current());
                }
            }
            
            return EnumerableWrapper<TResult>(result);
        }

        // ==================== ELEMENT OPERATIONS ====================

        /**
         * @brief Returns the first element of a sequence
         */
        template<typename T>
        static T First(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            if (enumerator->MoveNext()) {
                return enumerator->Current();
            }
            throw InvalidOperationException("Sequence contains no elements");
        }

        template<typename T, typename TPredicate>
        static T First(const IEnumerable<T>& source, TPredicate predicate) {
            return Where(source, predicate).First();
        }

        /**
         * @brief Returns the first element of a sequence, or a default value if no element is found
         */
        template<typename T>
        static T FirstOrDefault(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            if (enumerator->MoveNext()) {
                return enumerator->Current();
            }
            return T{};
        }

        template<typename T, typename TPredicate>
        static T FirstOrDefault(const IEnumerable<T>& source, TPredicate predicate) {
            return Where(source, predicate).FirstOrDefault();
        }

        /**
         * @brief Returns the last element of a sequence
         */
        template<typename T>
        static T Last(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            bool hasElements = false;
            T lastElement{};
            
            while (enumerator->MoveNext()) {
                lastElement = enumerator->Current();
                hasElements = true;
            }
            
            if (!hasElements) {
                throw InvalidOperationException("Sequence contains no elements");
            }
            return lastElement;
        }

        template<typename T, typename TPredicate>
        static T Last(const IEnumerable<T>& source, TPredicate predicate) {
            return Where(source, predicate).Last();
        }

        /**
         * @brief Returns the last element of a sequence, or a default value if no element is found
         */
        template<typename T>
        static T LastOrDefault(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            T lastElement{};
            
            while (enumerator->MoveNext()) {
                lastElement = enumerator->Current();
            }
            
            return lastElement;
        }

        template<typename T, typename TPredicate>
        static T LastOrDefault(const IEnumerable<T>& source, TPredicate predicate) {
            return Where(source, predicate).LastOrDefault();
        }

        /**
         * @brief Returns the only element of a sequence, and throws an exception if there is not exactly one element
         */
        template<typename T>
        static T Single(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            if (!enumerator->MoveNext()) {
                throw InvalidOperationException("Sequence contains no elements");
            }
            
            T result = enumerator->Current();
            if (enumerator->MoveNext()) {
                throw InvalidOperationException("Sequence contains more than one element");
            }
            
            return result;
        }

        template<typename T, typename TPredicate>
        static T Single(const IEnumerable<T>& source, TPredicate predicate) {
            return Where(source, predicate).Single();
        }

        /**
         * @brief Returns the only element of a sequence, or a default value if the sequence is empty
         */
        template<typename T>
        static T SingleOrDefault(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            if (!enumerator->MoveNext()) {
                return T{};
            }
            
            T result = enumerator->Current();
            if (enumerator->MoveNext()) {
                throw InvalidOperationException("Sequence contains more than one element");
            }
            
            return result;
        }

        template<typename T, typename TPredicate>
        static T SingleOrDefault(const IEnumerable<T>& source, TPredicate predicate) {
            return Where(source, predicate).SingleOrDefault();
        }

        // ==================== QUANTIFIER OPERATIONS ====================

        /**
         * @brief Determines whether a sequence contains any elements
         */
        template<typename T>
        static bool Any(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            return enumerator->MoveNext();
        }

        template<typename T, typename TPredicate>
        static bool Any(const IEnumerable<T>& source, TPredicate predicate) {
            auto enumerator = source.GetEnumerator();
            while (enumerator->MoveNext()) {
                if (predicate(enumerator->Current())) {
                    return true;
                }
            }
            return false;
        }

        /**
         * @brief Determines whether all elements of a sequence satisfy a condition
         */
        template<typename T, typename TPredicate>
        static bool All(const IEnumerable<T>& source, TPredicate predicate) {
            auto enumerator = source.GetEnumerator();
            while (enumerator->MoveNext()) {
                if (!predicate(enumerator->Current())) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Returns the number of elements in a sequence
         */
        template<typename T>
        static int Count(const IEnumerable<T>& source) {
            int count = 0;
            auto enumerator = source.GetEnumerator();
            while (enumerator->MoveNext()) {
                count++;
            }
            return count;
        }

        template<typename T, typename TPredicate>
        static int Count(const IEnumerable<T>& source, TPredicate predicate) {
            return Count(Where(source, predicate));
        }

        // ==================== PARTITIONING OPERATIONS ====================

        /**
         * @brief Returns a specified number of contiguous elements from the start of a sequence
         */
        template<typename T>
        static EnumerableWrapper<T> Take(const IEnumerable<T>& source, int count) {
            auto sourcePtr = std::shared_ptr<IEnumerable<T>>(const_cast<IEnumerable<T>*>(&source), [](IEnumerable<T>*){});
            return EnumerableWrapper<T>(sourcePtr).Take(count);
        }

        /**
         * @brief Bypasses a specified number of elements in a sequence and returns the remaining elements
         */
        template<typename T>
        static EnumerableWrapper<T> Skip(const IEnumerable<T>& source, int count) {
            auto sourcePtr = std::shared_ptr<IEnumerable<T>>(const_cast<IEnumerable<T>*>(&source), [](IEnumerable<T>*){});
            return EnumerableWrapper<T>(sourcePtr).Skip(count);
        }

        /**
         * @brief Returns elements from a sequence as long as a specified condition is true
         */
        template<typename T, typename TPredicate>
        static EnumerableWrapper<T> TakeWhile(const IEnumerable<T>& source, TPredicate predicate) {
            auto sourcePtr = std::shared_ptr<IEnumerable<T>>(const_cast<IEnumerable<T>*>(&source), [](IEnumerable<T>*){});
            return EnumerableWrapper<T>(sourcePtr).TakeWhile(predicate);
        }

        /**
         * @brief Bypasses elements in a sequence as long as a specified condition is true and returns the remaining elements
         */
        template<typename T, typename TPredicate>
        static EnumerableWrapper<T> SkipWhile(const IEnumerable<T>& source, TPredicate predicate) {
            auto sourcePtr = std::shared_ptr<IEnumerable<T>>(const_cast<IEnumerable<T>*>(&source), [](IEnumerable<T>*){});
            return EnumerableWrapper<T>(sourcePtr).SkipWhile(predicate);
        }

        // ==================== SET OPERATIONS ====================

        /**
         * @brief Returns distinct elements from a sequence
         */
        template<typename T>
        static EnumerableWrapper<T> Distinct(const IEnumerable<T>& source) {
            auto result = std::make_shared<System::Collections::Generic::List<T>>();
            std::unordered_set<T> seen;
            auto enumerator = source.GetEnumerator();
            
            while (enumerator->MoveNext()) {
                const T& current = enumerator->Current();
                if (seen.find(current) == seen.end()) {
                    seen.insert(current);
                    result->Add(current);
                }
            }
            
            return EnumerableWrapper<T>(result);
        }

        /**
         * @brief Produces the set union of two sequences
         */
        template<typename T>
        static EnumerableWrapper<T> Union(const IEnumerable<T>& first, const IEnumerable<T>& second) {
            auto result = std::make_shared<System::Collections::Generic::List<T>>();
            std::unordered_set<T> seen;
            
            auto enumerator1 = first.GetEnumerator();
            while (enumerator1->MoveNext()) {
                const T& current = enumerator1->Current();
                if (seen.find(current) == seen.end()) {
                    seen.insert(current);
                    result->Add(current);
                }
            }
            
            auto enumerator2 = second.GetEnumerator();
            while (enumerator2->MoveNext()) {
                const T& current = enumerator2->Current();
                if (seen.find(current) == seen.end()) {
                    seen.insert(current);
                    result->Add(current);
                }
            }
            
            return EnumerableWrapper<T>(result);
        }

        /**
         * @brief Produces the set intersection of two sequences
         */
        template<typename T>
        static EnumerableWrapper<T> Intersect(const IEnumerable<T>& first, const IEnumerable<T>& second) {
            auto result = std::make_shared<System::Collections::Generic::List<T>>();
            std::unordered_set<T> secondSet;
            std::unordered_set<T> resultSet;
            
            // Build set from second sequence
            auto enumerator2 = second.GetEnumerator();
            while (enumerator2->MoveNext()) {
                secondSet.insert(enumerator2->Current());
            }
            
            // Find intersection
            auto enumerator1 = first.GetEnumerator();
            while (enumerator1->MoveNext()) {
                const T& current = enumerator1->Current();
                if (secondSet.find(current) != secondSet.end() && 
                    resultSet.find(current) == resultSet.end()) {
                    resultSet.insert(current);
                    result->Add(current);
                }
            }
            
            return EnumerableWrapper<T>(result);
        }

        /**
         * @brief Produces the set difference of two sequences
         */
        template<typename T>
        static EnumerableWrapper<T> Except(const IEnumerable<T>& first, const IEnumerable<T>& second) {
            auto result = std::make_shared<System::Collections::Generic::List<T>>();
            std::unordered_set<T> secondSet;
            std::unordered_set<T> resultSet;
            
            // Build set from second sequence
            auto enumerator2 = second.GetEnumerator();
            while (enumerator2->MoveNext()) {
                secondSet.insert(enumerator2->Current());
            }
            
            // Find elements in first but not in second
            auto enumerator1 = first.GetEnumerator();
            while (enumerator1->MoveNext()) {
                const T& current = enumerator1->Current();
                if (secondSet.find(current) == secondSet.end() && 
                    resultSet.find(current) == resultSet.end()) {
                    resultSet.insert(current);
                    result->Add(current);
                }
            }
            
            return EnumerableWrapper<T>(result);
        }

        // ==================== AGGREGATION OPERATIONS ====================

        /**
         * @brief Applies an accumulator function over a sequence
         */
        template<typename T, typename TAccumulate, typename TFunc>
        static TAccumulate Aggregate(const IEnumerable<T>& source, TAccumulate seed, TFunc func) {
            TAccumulate result = seed;
            auto enumerator = source.GetEnumerator();
            
            while (enumerator->MoveNext()) {
                result = func(result, enumerator->Current());
            }
            
            return result;
        }

        /**
         * @brief Computes the sum of a sequence of numeric values
         */
        template<typename T>
        static T Sum(const IEnumerable<T>& source) {
            static_assert(std::is_arithmetic_v<T>, "Sum requires arithmetic type");
            return Aggregate(source, T{}, [](T acc, const T& val) { return acc + val; });
        }

        template<typename T, typename TSelector>
        static auto Sum(const IEnumerable<T>& source, TSelector selector) {
            using TResult = decltype(selector(std::declval<T>()));
            static_assert(std::is_arithmetic_v<TResult>, "Sum requires arithmetic type");
            return Aggregate(source, TResult{}, [selector](TResult acc, const T& val) { 
                return acc + selector(val); 
            });
        }

        /**
         * @brief Computes the average of a sequence of numeric values
         */
        template<typename T>
        static double Average(const IEnumerable<T>& source) {
            static_assert(std::is_arithmetic_v<T>, "Average requires arithmetic type");
            T sum = T{};
            int count = 0;
            auto enumerator = source.GetEnumerator();
            
            while (enumerator->MoveNext()) {
                sum += enumerator->Current();
                count++;
            }
            
            if (count == 0) {
                throw InvalidOperationException("Sequence contains no elements");
            }
            
            return static_cast<double>(sum) / count;
        }

        /**
         * @brief Returns the minimum value in a sequence
         */
        template<typename T>
        static T Min(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            if (!enumerator->MoveNext()) {
                throw InvalidOperationException("Sequence contains no elements");
            }
            
            T min = enumerator->Current();
            while (enumerator->MoveNext()) {
                const T& current = enumerator->Current();
                if (current < min) {
                    min = current;
                }
            }
            
            return min;
        }

        /**
         * @brief Returns the maximum value in a sequence
         */
        template<typename T>
        static T Max(const IEnumerable<T>& source) {
            auto enumerator = source.GetEnumerator();
            if (!enumerator->MoveNext()) {
                throw InvalidOperationException("Sequence contains no elements");
            }
            
            T max = enumerator->Current();
            while (enumerator->MoveNext()) {
                const T& current = enumerator->Current();
                if (current > max) {
                    max = current;
                }
            }
            
            return max;
        }

        // ==================== MATERIALIZATION OPERATIONS ====================

        /**
         * @brief Creates a List<T> from an IEnumerable<T>
         */
        template<typename T>
        static System::Collections::Generic::List<T> ToList(const IEnumerable<T>& source) {
            System::Collections::Generic::List<T> result;
            auto enumerator = source.GetEnumerator();
            while (enumerator->MoveNext()) {
                result.Add(enumerator->Current());
            }
            return result;
        }

        /**
         * @brief Creates an array from an IEnumerable<T>
         */
        template<typename T>
        static std::vector<T> ToArray(const IEnumerable<T>& source) {
            std::vector<T> result;
            auto enumerator = source.GetEnumerator();
            while (enumerator->MoveNext()) {
                result.push_back(enumerator->Current());
            }
            return result;
        }

        // ==================== UTILITY FUNCTIONS ====================

        /**
         * @brief Creates an EnumerableWrapper from various sources
         */
        template<typename T>
        static EnumerableWrapper<T> From(const std::vector<T>& source) {
            return EnumerableWrapper<T>(source);
        }

        template<typename T>
        static EnumerableWrapper<T> From(std::initializer_list<T> source) {
            return EnumerableWrapper<T>(source);
        }

        template<typename T>
        static EnumerableWrapper<T> From(const IEnumerable<T>& source) {
            auto sourcePtr = std::shared_ptr<IEnumerable<T>>(const_cast<IEnumerable<T>*>(&source), [](IEnumerable<T>*){});
            return EnumerableWrapper<T>(sourcePtr);
        }

        /**
         * @brief Generates a sequence of integral numbers within a specified range
         */
        static EnumerableWrapper<int> Range(int start, int count) {
            auto result = System::Collections::Generic::List<int>();
            for (int i = 0; i < count; i++) {
                result.Add(start + i);
            }
            return EnumerableWrapper<int>(result);
        }

        /**
         * @brief Generates a sequence that contains one repeated value
         */
        template<typename T>
        static EnumerableWrapper<T> Repeat(const T& element, int count) {
            auto result = System::Collections::Generic::List<T>();
            for (int i = 0; i < count; i++) {
                result.Add(element);
            }
            return EnumerableWrapper<T>(result);
        }

        /**
         * @brief Returns an empty IEnumerable<T>
         */
        template<typename T>
        static EnumerableWrapper<T> Empty() {
            auto result = System::Collections::Generic::List<T>();
            return EnumerableWrapper<T>(result);
        }

        // ==================== GENERATOR INTEGRATION ====================

        /**
         * @brief Create EnumerableWrapper from Generator<T> for LINQ operations
         */
        template<typename T>
        static EnumerableWrapper<T> From(System::Coroutines::Generator<T>&& generator);

        /**
         * @brief Range generator using coroutines (lazy evaluation)
         */
        template<typename T = int>
        static System::Coroutines::Generator<T> RangeGenerator(T start, int count);

        /**
         * @brief Repeat generator using coroutines (lazy evaluation)
         */
        template<typename T>
        static System::Coroutines::Generator<T> RepeatGenerator(T value, int count);

        /**
         * @brief Empty generator
         */
        template<typename T>
        static System::Coroutines::Generator<T> EmptyGenerator();
    };

} // namespace System::Linq
