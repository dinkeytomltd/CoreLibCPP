
#pragma once

#include "IEnumerable.hpp"
#include "IEnumerator.hpp"
#include "LinqIterators.hpp"
#include "ListEnumerableAdapter.hpp"
#include "../Collections/Generic/List.h"
#include "../InvalidOperationException.h"
#include <vector>
#include <memory>
#include <functional>

namespace System::Linq {

    /**
     * @brief Wrapper class that makes any container enumerable and provides LINQ operations
     */
    template<typename T>
    class EnumerableWrapper : public IEnumerable<T> {
    private:
        std::shared_ptr<IEnumerable<T>> _source;

    public:
        // Constructor from IEnumerable
        explicit EnumerableWrapper(std::shared_ptr<IEnumerable<T>> source)
            : _source(source) {}

        // Constructor from std::vector
        explicit EnumerableWrapper(const std::vector<T>& container) {
            auto vector = std::make_shared<std::vector<T>>(container);
            auto adapter = std::make_shared<VectorEnumerableAdapter<T>>(vector);
            _source = adapter;
        }

        // Constructor from std::initializer_list
        explicit EnumerableWrapper(std::initializer_list<T> items) {
            auto vector = std::make_shared<std::vector<T>>(items);
            auto adapter = std::make_shared<VectorEnumerableAdapter<T>>(vector);
            _source = adapter;
        }

        // Constructor from System::Collections::Generic::List<T>
        explicit EnumerableWrapper(const System::Collections::Generic::List<T>& list) {
            auto listPtr = std::make_shared<System::Collections::Generic::List<T>>(list);
            auto adapter = std::make_shared<ListEnumerableAdapter<T>>(listPtr);
            _source = adapter;
        }

        std::unique_ptr<IEnumerator<T>> GetEnumerator() override {
            return _source->GetEnumerator();
        }

        std::unique_ptr<IEnumerator<T>> GetEnumerator() const override {
            return _source->GetEnumerator();
        }

        // LINQ Operations

        /**
         * @brief Filters elements based on a predicate
         */
        template<typename TPredicate>
        EnumerableWrapper<T> Where(TPredicate predicate) const {
            auto whereEnumerable = std::make_shared<WhereEnumerable<T, TPredicate>>(_source, predicate);
            return EnumerableWrapper<T>(whereEnumerable);
        }

        /**
         * @brief Projects each element to a new form
         */
        template<typename TResult, typename TSelector>
        EnumerableWrapper<TResult> Select(TSelector selector) const {
            auto selectEnumerable = std::make_shared<SelectEnumerable<T, TResult, TSelector>>(_source, selector);
            return EnumerableWrapper<TResult>(selectEnumerable);
        }

        /**
         * @brief Takes the first count elements
         */
        EnumerableWrapper<T> Take(int count) const {
            auto takeEnumerable = std::make_shared<TakeEnumerable<T>>(_source, count);
            return EnumerableWrapper<T>(takeEnumerable);
        }

        /**
         * @brief Skips the first count elements
         */
        EnumerableWrapper<T> Skip(int count) const {
            auto skipEnumerable = std::make_shared<SkipEnumerable<T>>(_source, count);
            return EnumerableWrapper<T>(skipEnumerable);
        }

        /**
         * @brief Takes elements while predicate is true
         */
        template<typename TPredicate>
        EnumerableWrapper<T> TakeWhile(TPredicate predicate) const {
            auto takeWhileEnumerable = std::make_shared<TakeWhileEnumerable<T, TPredicate>>(_source, predicate);
            return EnumerableWrapper<T>(takeWhileEnumerable);
        }

        /**
         * @brief Skips elements while predicate is true
         */
        template<typename TPredicate>
        EnumerableWrapper<T> SkipWhile(TPredicate predicate) const {
            auto skipWhileEnumerable = std::make_shared<SkipWhileEnumerable<T, TPredicate>>(_source, predicate);
            return EnumerableWrapper<T>(skipWhileEnumerable);
        }

        // Element operations
        T First() const {
            auto enumerator = GetEnumerator();
            if (enumerator->MoveNext()) {
                return enumerator->Current();
            }
            throw InvalidOperationException("Sequence contains no elements");
        }

        T FirstOrDefault() const {
            auto enumerator = GetEnumerator();
            if (enumerator->MoveNext()) {
                return enumerator->Current();
            }
            return T{};
        }

        template<typename TPredicate>
        T First(TPredicate predicate) const {
            return Where(predicate).First();
        }

        template<typename TPredicate>
        T FirstOrDefault(TPredicate predicate) const {
            return Where(predicate).FirstOrDefault();
        }

        // Quantifier operations
        bool Any() const {
            auto enumerator = GetEnumerator();
            return enumerator->MoveNext();
        }

        template<typename TPredicate>
        bool Any(TPredicate predicate) const {
            auto enumerator = GetEnumerator();
            while (enumerator->MoveNext()) {
                if (predicate(enumerator->Current())) {
                    return true;
                }
            }
            return false;
        }

        template<typename TPredicate>
        bool All(TPredicate predicate) const {
            auto enumerator = GetEnumerator();
            while (enumerator->MoveNext()) {
                if (!predicate(enumerator->Current())) {
                    return false;
                }
            }
            return true;
        }

        int Count() const {
            int count = 0;
            auto enumerator = GetEnumerator();
            while (enumerator->MoveNext()) {
                count++;
            }
            return count;
        }

        template<typename TPredicate>
        int Count(TPredicate predicate) const {
            return Where(predicate).Count();
        }

        // Materialization operations
        System::Collections::Generic::List<T> ToList() const {
            System::Collections::Generic::List<T> result;
            auto enumerator = GetEnumerator();
            while (enumerator->MoveNext()) {
                result.Add(enumerator->Current());
            }
            return result;
        }

        std::vector<T> ToVector() const {
            std::vector<T> result;
            auto enumerator = GetEnumerator();
            while (enumerator->MoveNext()) {
                result.push_back(enumerator->Current());
            }
            return result;
        }

    private:
        // Helper enumerable classes for lazy evaluation

        template<typename TSource, typename TPredicate>
        class WhereEnumerable : public IEnumerable<TSource> {
        private:
            std::shared_ptr<IEnumerable<TSource>> _source;
            TPredicate _predicate;

        public:
            WhereEnumerable(std::shared_ptr<IEnumerable<TSource>> source, TPredicate predicate)
                : _source(source), _predicate(predicate) {}

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() override {
                return std::make_unique<WhereEnumerator<TSource, TPredicate>>(_source->GetEnumerator(), _predicate);
            }

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() const override {
                return std::make_unique<WhereEnumerator<TSource, TPredicate>>(_source->GetEnumerator(), _predicate);
            }
        };

        template<typename TSource, typename TResult, typename TSelector>
        class SelectEnumerable : public IEnumerable<TResult> {
        private:
            std::shared_ptr<IEnumerable<TSource>> _source;
            TSelector _selector;

        public:
            SelectEnumerable(std::shared_ptr<IEnumerable<TSource>> source, TSelector selector)
                : _source(source), _selector(selector) {}

            std::unique_ptr<IEnumerator<TResult>> GetEnumerator() override {
                return std::make_unique<SelectEnumerator<TSource, TResult, TSelector>>(_source->GetEnumerator(), _selector);
            }

            std::unique_ptr<IEnumerator<TResult>> GetEnumerator() const override {
                return std::make_unique<SelectEnumerator<TSource, TResult, TSelector>>(_source->GetEnumerator(), _selector);
            }
        };

        template<typename TSource>
        class TakeEnumerable : public IEnumerable<TSource> {
        private:
            std::shared_ptr<IEnumerable<TSource>> _source;
            int _count;

        public:
            TakeEnumerable(std::shared_ptr<IEnumerable<TSource>> source, int count)
                : _source(source), _count(count) {}

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() override {
                return std::make_unique<TakeEnumerator<TSource>>(_source->GetEnumerator(), _count);
            }

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() const override {
                return std::make_unique<TakeEnumerator<TSource>>(_source->GetEnumerator(), _count);
            }
        };

        template<typename TSource>
        class SkipEnumerable : public IEnumerable<TSource> {
        private:
            std::shared_ptr<IEnumerable<TSource>> _source;
            int _count;

        public:
            SkipEnumerable(std::shared_ptr<IEnumerable<TSource>> source, int count)
                : _source(source), _count(count) {}

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() override {
                return std::make_unique<SkipEnumerator<TSource>>(_source->GetEnumerator(), _count);
            }

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() const override {
                return std::make_unique<SkipEnumerator<TSource>>(_source->GetEnumerator(), _count);
            }
        };

        template<typename TSource, typename TPredicate>
        class TakeWhileEnumerable : public IEnumerable<TSource> {
        private:
            std::shared_ptr<IEnumerable<TSource>> _source;
            TPredicate _predicate;

        public:
            TakeWhileEnumerable(std::shared_ptr<IEnumerable<TSource>> source, TPredicate predicate)
                : _source(source), _predicate(predicate) {}

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() override {
                return std::make_unique<TakeWhileEnumerator<TSource, TPredicate>>(_source->GetEnumerator(), _predicate);
            }

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() const override {
                return std::make_unique<TakeWhileEnumerator<TSource, TPredicate>>(_source->GetEnumerator(), _predicate);
            }
        };

        template<typename TSource, typename TPredicate>
        class SkipWhileEnumerable : public IEnumerable<TSource> {
        private:
            std::shared_ptr<IEnumerable<TSource>> _source;
            TPredicate _predicate;

        public:
            SkipWhileEnumerable(std::shared_ptr<IEnumerable<TSource>> source, TPredicate predicate)
                : _source(source), _predicate(predicate) {}

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() override {
                return std::make_unique<SkipWhileEnumerator<TSource, TPredicate>>(_source->GetEnumerator(), _predicate);
            }

            std::unique_ptr<IEnumerator<TSource>> GetEnumerator() const override {
                return std::make_unique<SkipWhileEnumerator<TSource, TPredicate>>(_source->GetEnumerator(), _predicate);
            }
        };
    };

} // namespace System::Linq
