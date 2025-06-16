#pragma once
/*
 * List.h
 * C++ implementation of .NET's System.Collections.Generic.List<T> class
 * 
 * Key .NET List<T> characteristics implemented:
 * - Dynamic array-based collection
 * - Type-safe generic implementation
 * - Core collection operations (Add, Remove, Contains, etc.)
 * - Indexer access
 * - Iteration support
 */
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <functional>
#include "../../../System/Object.h"
#include "../../../System/String.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename T>
class List {
private:
    std::vector<T> m_items;

public:
    // Constructors
    List() = default;
    
    // Constructor with initial capacity
    explicit List(size_t capacity) {
        m_items.reserve(capacity);
    }
    
    // Constructor with initializer list
    List(std::initializer_list<T> items) : m_items(items) {}
    
    // Copy constructor
    List(const List<T>& other) = default;
    
    // Assignment operator
    List<T>& operator=(const List<T>& other) = default;
    
    // Move constructor
    List(List<T>&& other) noexcept = default;
    
    // Move assignment operator
    List<T>& operator=(List<T>&& other) noexcept = default;
    
    // Destructor
    ~List() = default;
    
    // Properties
    
    // Count property (equivalent to .NET List<T>.Count)
    [[nodiscard]] size_t Count() const {
        return m_items.size();
    }
    
    // Capacity property (equivalent to .NET List<T>.Capacity)
    [[nodiscard]] size_t Capacity() const {
        return m_items.capacity();
    }
    
    // Methods
    
    // Add method (equivalent to .NET List<T>.Add)
    void Add(const T& item) {
        m_items.push_back(item);
    }
    
    void Add(T&& item) {
        m_items.push_back(std::move(item));
    }
    
    // AddRange method (equivalent to .NET List<T>.AddRange)
    void AddRange(const List<T>& collection) {
        m_items.reserve(m_items.size() + collection.Count());
        for (size_t i = 0; i < collection.Count(); ++i) {
            m_items.push_back(collection[i]);
        }
    }
    
    // Clear method (equivalent to .NET List<T>.Clear)
    void Clear() {
        m_items.clear();
    }
    
    // Contains method (equivalent to .NET List<T>.Contains)
    [[nodiscard]] bool Contains(const T& item) const {
        return std::find(m_items.begin(), m_items.end(), item) != m_items.end();
    }
    
    // IndexOf method (equivalent to .NET List<T>.IndexOf)
    [[nodiscard]] size_t IndexOf(const T& item) const {
        auto it = std::find(m_items.begin(), m_items.end(), item);
        if (it == m_items.end()) {
            return static_cast<size_t>(-1); // Not found, return equivalent of -1
        }
        return static_cast<size_t>(std::distance(m_items.begin(), it));
    }
    
    // Insert method (equivalent to .NET List<T>.Insert)
    void Insert(size_t index, const T& item) {
        if (index > m_items.size()) {
            throw std::out_of_range("Index was out of range");
        }
        m_items.insert(m_items.begin() + index, item);
    }
    
    // Remove method (equivalent to .NET List<T>.Remove)
    bool Remove(const T& item) {
        auto it = std::find(m_items.begin(), m_items.end(), item);
        if (it == m_items.end()) {
            return false;
        }
        m_items.erase(it);
        return true;
    }
    
    // RemoveAt method (equivalent to .NET List<T>.RemoveAt)
    void RemoveAt(size_t index) {
        if (index >= m_items.size()) {
            throw std::out_of_range("Index was out of range");
        }
        m_items.erase(m_items.begin() + index);
    }
    
    // RemoveRange method (equivalent to .NET List<T>.RemoveRange)
    void RemoveRange(size_t index, size_t count) {
        if (index >= m_items.size()) {
            throw std::out_of_range("Index was out of range");
        }
        if (index + count > m_items.size()) {
            throw std::out_of_range("Count was out of range");
        }
        m_items.erase(m_items.begin() + index, m_items.begin() + index + count);
    }
    
    // TrimExcess method (equivalent to .NET List<T>.TrimExcess)
    void TrimExcess() {
        m_items.shrink_to_fit();
    }
    
    // ForEach method (equivalent to .NET List<T>.ForEach)
    void ForEach(const std::function<void(T&)>& action) {
        for (auto& item : m_items) {
            action(item);
        }
    }
    
    // Indexer (equivalent to .NET List<T>[index])
    [[nodiscard]] T& operator[](size_t index) {
        if (index >= m_items.size()) {
            throw std::out_of_range("Index was out of range");
        }
        return m_items[index];
    }
    
    [[nodiscard]] const T& operator[](size_t index) const {
        if (index >= m_items.size()) {
            throw std::out_of_range("Index was out of range");
        }
        return m_items[index];
    }
    
    // Iterator support (for range-based for loops)
    [[nodiscard]] auto begin() { return m_items.begin(); }
    [[nodiscard]] auto end() { return m_items.end(); }
    [[nodiscard]] auto begin() const { return m_items.begin(); }
    [[nodiscard]] auto end() const { return m_items.end(); }
    [[nodiscard]] auto cbegin() const { return m_items.cbegin(); }
    [[nodiscard]] auto cend() const { return m_items.cend(); }
    
    // ToArray method (equivalent to .NET List<T>.ToArray)
    [[nodiscard]] std::vector<T> ToArray() const {
        return m_items;
    }
    
    // ToString method (for debugging)
    [[nodiscard]] System::String ToString() const {
        return System::String("System.Collections.Generic.List`1[T]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
