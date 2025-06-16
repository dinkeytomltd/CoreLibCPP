#pragma once

#include "IEnumerable.hpp"
#include "IEnumerator.hpp"
#include "../Collections/Generic/List.h"
#include "../InvalidOperationException.h"
#include <memory>

namespace System::Linq {

    /**
     * @brief Enumerator adapter for System::Collections::Generic::List<T>
     */
    template<typename T>
    class ListEnumerator : public IEnumerator<T> {
    private:
        const System::Collections::Generic::List<T>* _list;
        size_t _index;
        bool _started;

    public:
        explicit ListEnumerator(const System::Collections::Generic::List<T>* list)
            : _list(list), _index(0), _started(false) {}

        const T& Current() const override {
            if (!_started || _index >= _list->Count()) {
                throw InvalidOperationException("Enumerator is not positioned on a valid element");
            }
            return (*_list)[_index];
        }

        T& Current() override {
            if (!_started || _index >= _list->Count()) {
                throw InvalidOperationException("Enumerator is not positioned on a valid element");
            }
            return const_cast<T&>((*_list)[_index]);
        }

        bool MoveNext() override {
            if (!_started) {
                _started = true;
                _index = 0;
            } else {
                _index++;
            }
            return _index < _list->Count();
        }

        void Reset() override {
            _started = false;
            _index = 0;
        }
    };

    /**
     * @brief Enumerable adapter for System::Collections::Generic::List<T>
     */
    template<typename T>
    class ListEnumerableAdapter : public IEnumerable<T> {
    private:
        std::shared_ptr<System::Collections::Generic::List<T>> _list;

    public:
        explicit ListEnumerableAdapter(std::shared_ptr<System::Collections::Generic::List<T>> list)
            : _list(list) {}

        std::unique_ptr<IEnumerator<T>> GetEnumerator() override {
            return std::make_unique<ListEnumerator<T>>(_list.get());
        }

        std::unique_ptr<IEnumerator<T>> GetEnumerator() const override {
            return std::make_unique<ListEnumerator<T>>(_list.get());
        }
    };

    /**
     * @brief Vector enumerator for std::vector<T>
     */
    template<typename T>
    class VectorEnumerator : public IEnumerator<T> {
    private:
        const std::vector<T>* _vector;
        size_t _index;
        bool _started;

    public:
        explicit VectorEnumerator(const std::vector<T>* vector)
            : _vector(vector), _index(0), _started(false) {}

        const T& Current() const override {
            if (!_started || _index >= _vector->size()) {
                throw InvalidOperationException("Enumerator is not positioned on a valid element");
            }
            return (*_vector)[_index];
        }

        T& Current() override {
            if (!_started || _index >= _vector->size()) {
                throw InvalidOperationException("Enumerator is not positioned on a valid element");
            }
            return const_cast<T&>((*_vector)[_index]);
        }

        bool MoveNext() override {
            if (!_started) {
                _started = true;
                _index = 0;
            } else {
                _index++;
            }
            return _index < _vector->size();
        }

        void Reset() override {
            _started = false;
            _index = 0;
        }
    };

    /**
     * @brief Enumerable adapter for std::vector<T>
     */
    template<typename T>
    class VectorEnumerableAdapter : public IEnumerable<T> {
    private:
        std::shared_ptr<std::vector<T>> _vector;

    public:
        explicit VectorEnumerableAdapter(std::shared_ptr<std::vector<T>> vector)
            : _vector(vector) {}

        std::unique_ptr<IEnumerator<T>> GetEnumerator() override {
            return std::make_unique<VectorEnumerator<T>>(_vector.get());
        }

        std::unique_ptr<IEnumerator<T>> GetEnumerator() const override {
            return std::make_unique<VectorEnumerator<T>>(_vector.get());
        }
    };

} // namespace System::Linq
