#pragma once

#include <vector>
#include <stdexcept>

namespace System {

    /**
     * @brief Simple Array template that wraps std::vector with .NET-like interface
     */
    template<typename T>
    class Array {
    private:
        std::vector<T> m_data;

    public:
        /**
         * @brief Default constructor
         */
        Array() = default;

        /**
         * @brief Constructor with size
         */
        explicit Array(size_t size) : m_data(size) {}

        /**
         * @brief Constructor from data and size
         */
        Array(const T* data, size_t size) : m_data(data, data + size) {}

        /**
         * @brief Constructor from std::vector
         */
        explicit Array(const std::vector<T>& vec) : m_data(vec) {}

        /**
         * @brief Constructor from initializer list
         */
        Array(std::initializer_list<T> init) : m_data(init) {}

        /**
         * @brief Copy constructor
         */
        Array(const Array& other) = default;

        /**
         * @brief Move constructor
         */
        Array(Array&& other) noexcept = default;

        /**
         * @brief Copy assignment
         */
        Array& operator=(const Array& other) = default;

        /**
         * @brief Move assignment
         */
        Array& operator=(Array&& other) noexcept = default;

        /**
         * @brief Destructor
         */
        ~Array() = default;

        /**
         * @brief Gets the length of the array
         */
        size_t Length() const { return m_data.size(); }

        /**
         * @brief Gets the size of the array (alias for Length)
         */
        size_t Size() const { return m_data.size(); }

        /**
         * @brief Checks if the array is empty
         */
        bool IsEmpty() const { return m_data.empty(); }

        /**
         * @brief Element access operator
         */
        T& operator[](size_t index) {
            if (index >= m_data.size()) {
                throw std::out_of_range("Index out of range");
            }
            return m_data[index];
        }

        /**
         * @brief Const element access operator
         */
        const T& operator[](size_t index) const {
            if (index >= m_data.size()) {
                throw std::out_of_range("Index out of range");
            }
            return m_data[index];
        }

        /**
         * @brief Gets pointer to the underlying data
         */
        T* Data() { return m_data.data(); }

        /**
         * @brief Gets const pointer to the underlying data
         */
        const T* Data() const { return m_data.data(); }

        /**
         * @brief Gets reference to the underlying vector
         */
        std::vector<T>& GetVector() { return m_data; }

        /**
         * @brief Gets const reference to the underlying vector
         */
        const std::vector<T>& GetVector() const { return m_data; }

        /**
         * @brief Begin iterator
         */
        typename std::vector<T>::iterator begin() { return m_data.begin(); }

        /**
         * @brief End iterator
         */
        typename std::vector<T>::iterator end() { return m_data.end(); }

        /**
         * @brief Const begin iterator
         */
        typename std::vector<T>::const_iterator begin() const { return m_data.begin(); }

        /**
         * @brief Const end iterator
         */
        typename std::vector<T>::const_iterator end() const { return m_data.end(); }

        /**
         * @brief Const begin iterator
         */
        typename std::vector<T>::const_iterator cbegin() const { return m_data.cbegin(); }

        /**
         * @brief Const end iterator
         */
        typename std::vector<T>::const_iterator cend() const { return m_data.cend(); }

        /**
         * @brief Resize the array
         */
        void Resize(size_t newSize) { m_data.resize(newSize); }

        /**
         * @brief Clear the array
         */
        void Clear() { m_data.clear(); }

        /**
         * @brief Equality operator
         */
        bool operator==(const Array& other) const {
            return m_data == other.m_data;
        }

        /**
         * @brief Inequality operator
         */
        bool operator!=(const Array& other) const {
            return !(*this == other);
        }
    };

} // namespace System
