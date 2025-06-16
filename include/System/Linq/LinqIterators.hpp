
#pragma once

#include "IEnumerable.hpp"
#include "IEnumerator.hpp"
#include "../InvalidOperationException.h"
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace System::Linq {

    // Forward declarations
    template<typename T>
    class EnumerableWrapper;

    /**
     * @brief Base iterator class for LINQ operations with lazy evaluation
     */
    template<typename T>
    class LazyEnumerator : public IEnumerator<T> {
    protected:
        bool _started = false;
        bool _finished = false;
        T _current;
        bool _hasCurrent = false;

    public:
        const T& Current() const override {
            if (!_hasCurrent) {
                throw InvalidOperationException("Enumerator is not positioned on a valid element");
            }
            return _current;
        }

        T& Current() override {
            if (!_hasCurrent) {
                throw InvalidOperationException("Enumerator is not positioned on a valid element");
            }
            return _current;
        }

        void Reset() override {
            _started = false;
            _finished = false;
            _hasCurrent = false;
            ResetInternal();
        }

    protected:
        virtual void ResetInternal() = 0;
        
        void SetCurrent(const T& value) {
            _current = value;
            _hasCurrent = true;
        }

        void SetCurrent(T&& value) {
            _current = std::move(value);
            _hasCurrent = true;
        }

        void ClearCurrent() {
            _hasCurrent = false;
        }
    };

    /**
     * @brief Enumerator for Where operation (filtering)
     */
    template<typename T, typename TPredicate>
    class WhereEnumerator : public LazyEnumerator<T> {
    private:
        std::unique_ptr<IEnumerator<T>> _source;
        TPredicate _predicate;

    public:
        WhereEnumerator(std::unique_ptr<IEnumerator<T>> source, TPredicate predicate)
            : _source(std::move(source)), _predicate(predicate) {}

        bool MoveNext() override {
            while (_source->MoveNext()) {
                const T& current = _source->Current();
                if (_predicate(current)) {
                    this->SetCurrent(current);
                    return true;
                }
            }
            this->ClearCurrent();
            this->_finished = true;
            return false;
        }

    protected:
        void ResetInternal() override {
            _source->Reset();
        }
    };

    /**
     * @brief Enumerator for Select operation (projection/transformation)
     */
    template<typename TSource, typename TResult, typename TSelector>
    class SelectEnumerator : public LazyEnumerator<TResult> {
    private:
        std::unique_ptr<IEnumerator<TSource>> _source;
        TSelector _selector;

    public:
        SelectEnumerator(std::unique_ptr<IEnumerator<TSource>> source, TSelector selector)
            : _source(std::move(source)), _selector(selector) {}

        bool MoveNext() override {
            if (_source->MoveNext()) {
                auto result = _selector(_source->Current());
                this->SetCurrent(std::move(result));
                return true;
            }
            this->ClearCurrent();
            this->_finished = true;
            return false;
        }

    protected:
        void ResetInternal() override {
            _source->Reset();
        }
    };

    /**
     * @brief Enumerator for Take operation (taking first N elements)
     */
    template<typename T>
    class TakeEnumerator : public LazyEnumerator<T> {
    private:
        std::unique_ptr<IEnumerator<T>> _source;
        int _count;
        int _taken = 0;

    public:
        TakeEnumerator(std::unique_ptr<IEnumerator<T>> source, int count)
            : _source(std::move(source)), _count(count) {}

        bool MoveNext() override {
            if (_taken >= _count) {
                this->ClearCurrent();
                this->_finished = true;
                return false;
            }

            if (_source->MoveNext()) {
                this->SetCurrent(_source->Current());
                _taken++;
                return true;
            }

            this->ClearCurrent();
            this->_finished = true;
            return false;
        }

    protected:
        void ResetInternal() override {
            _source->Reset();
            _taken = 0;
        }
    };

    /**
     * @brief Enumerator for Skip operation (skipping first N elements)
     */
    template<typename T>
    class SkipEnumerator : public LazyEnumerator<T> {
    private:
        std::unique_ptr<IEnumerator<T>> _source;
        int _count;
        bool _skipped = false;

    public:
        SkipEnumerator(std::unique_ptr<IEnumerator<T>> source, int count)
            : _source(std::move(source)), _count(count) {}

        bool MoveNext() override {
            if (!_skipped) {
                for (int i = 0; i < _count && _source->MoveNext(); i++) {
                    // Skip elements
                }
                _skipped = true;
            }

            if (_source->MoveNext()) {
                this->SetCurrent(_source->Current());
                return true;
            }

            this->ClearCurrent();
            this->_finished = true;
            return false;
        }

    protected:
        void ResetInternal() override {
            _source->Reset();
            _skipped = false;
        }
    };

    /**
     * @brief Enumerator for TakeWhile operation
     */
    template<typename T, typename TPredicate>
    class TakeWhileEnumerator : public LazyEnumerator<T> {
    private:
        std::unique_ptr<IEnumerator<T>> _source;
        TPredicate _predicate;
        bool _yielding = true;

    public:
        TakeWhileEnumerator(std::unique_ptr<IEnumerator<T>> source, TPredicate predicate)
            : _source(std::move(source)), _predicate(predicate) {}

        bool MoveNext() override {
            if (!_yielding) {
                this->ClearCurrent();
                return false;
            }

            if (_source->MoveNext()) {
                const T& current = _source->Current();
                if (_predicate(current)) {
                    this->SetCurrent(current);
                    return true;
                } else {
                    _yielding = false;
                }
            }

            this->ClearCurrent();
            this->_finished = true;
            return false;
        }

    protected:
        void ResetInternal() override {
            _source->Reset();
            _yielding = true;
        }
    };

    /**
     * @brief Enumerator for SkipWhile operation
     */
    template<typename T, typename TPredicate>
    class SkipWhileEnumerator : public LazyEnumerator<T> {
    private:
        std::unique_ptr<IEnumerator<T>> _source;
        TPredicate _predicate;
        bool _skipped = false;

    public:
        SkipWhileEnumerator(std::unique_ptr<IEnumerator<T>> source, TPredicate predicate)
            : _source(std::move(source)), _predicate(predicate) {}

        bool MoveNext() override {
            if (!_skipped) {
                while (_source->MoveNext()) {
                    const T& current = _source->Current();
                    if (!_predicate(current)) {
                        _skipped = true;
                        this->SetCurrent(current);
                        return true;
                    }
                }
                _skipped = true;
                this->ClearCurrent();
                return false;
            }

            if (_source->MoveNext()) {
                this->SetCurrent(_source->Current());
                return true;
            }

            this->ClearCurrent();
            this->_finished = true;
            return false;
        }

    protected:
        void ResetInternal() override {
            _source->Reset();
            _skipped = false;
        }
    };

} // namespace System::Linq
