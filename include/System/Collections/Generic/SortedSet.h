
#pragma once
/*
 * SortedSet.h
 * C++ implementation of .NET's System.Collections.Generic.SortedSet<T> class
 * 
 * Key .NET SortedSet<T> characteristics implemented:
 * - Sorted set collection that maintains elements in sorted order with no duplicates
 * - O(log n) operations for Add, Remove, Contains
 * - Set operations like UnionWith, IntersectWith, ExceptWith
 * - Type-safe generic implementation
 */
#include <set>
#include <stdexcept>
#include <vector>
#include <memory>
#include <algorithm>
#include "../../../System/Object.h"
#include "../../../System/String.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename T>
class SortedSet {
private:
    std::set<T> m_items;

public:
    // Constructors
    SortedSet() = default;
    
    // Constructor with initializer list
    SortedSet(std::initializer_list<T> items) : m_items(items) {}
    
    // Constructor from range
    template<typename Iterator>
    SortedSet(Iterator first, Iterator last) : m_items(first, last) {}
    
    // Copy constructor
    SortedSet(const SortedSet<T>& other) = default;
    
    // Assignment operator
    SortedSet<T>& operator=(const SortedSet<T>& other) = default;
    
    // Move constructor
    SortedSet(SortedSet<T>&& other) noexcept = default;
    
    // Move assignment operator
    SortedSet<T>& operator=(SortedSet<T>&& other) noexcept = default;
    
    // Destructor
    ~SortedSet() = default;
    
    // Properties
    
    // Count property (equivalent to .NET SortedSet<T>.Count)
    [[nodiscard]] size_t Count() const {
        return m_items.size();
    }
    
    // Min property (equivalent to .NET SortedSet<T>.Min)
    [[nodiscard]] const T& Min() const {
        if (m_items.empty()) {
            throw std::runtime_error("Set is empty");
        }
        return *m_items.begin();
    }
    
    // Max property (equivalent to .NET SortedSet<T>.Max)
    [[nodiscard]] const T& Max() const {
        if (m_items.empty()) {
            throw std::runtime_error("Set is empty");
        }
        return *m_items.rbegin();
    }
    
    // Methods
    
    // Add method (equivalent to .NET SortedSet<T>.Add)
    bool Add(const T& item) {
        auto result = m_items.insert(item);
        return result.second; // Returns true if insertion took place
    }
    
    bool Add(T&& item) {
        auto result = m_items.insert(std::move(item));
        return result.second; // Returns true if insertion took place
    }
    
    // Clear method (equivalent to .NET SortedSet<T>.Clear)
    void Clear() {
        m_items.clear();
    }
    
    // Contains method (equivalent to .NET SortedSet<T>.Contains)
    [[nodiscard]] bool Contains(const T& item) const {
        return m_items.find(item) != m_items.end();
    }
    
    // Remove method (equivalent to .NET SortedSet<T>.Remove)
    bool Remove(const T& item) {
        auto it = m_items.find(item);
        if (it == m_items.end()) {
            return false;
        }
        m_items.erase(it);
        return true;
    }
    
    // Set operations
    
    // UnionWith method (equivalent to .NET SortedSet<T>.UnionWith)
    void UnionWith(const SortedSet<T>& other) {
        for (const auto& item : other.m_items) {
            m_items.insert(item);
        }
    }
    
    template<typename Container>
    void UnionWith(const Container& other) {
        for (const auto& item : other) {
            m_items.insert(item);
        }
    }
    
    // IntersectWith method (equivalent to .NET SortedSet<T>.IntersectWith)
    void IntersectWith(const SortedSet<T>& other) {
        std::set<T> result;
        std::set_intersection(m_items.begin(), m_items.end(),
                            other.m_items.begin(), other.m_items.end(),
                            std::inserter(result, result.begin()));
        m_items = std::move(result);
    }
    
    template<typename Container>
    void IntersectWith(const Container& other) {
        std::set<T> otherSet(other.begin(), other.end());
        std::set<T> result;
        std::set_intersection(m_items.begin(), m_items.end(),
                            otherSet.begin(), otherSet.end(),
                            std::inserter(result, result.begin()));
        m_items = std::move(result);
    }
    
    // ExceptWith method (equivalent to .NET SortedSet<T>.ExceptWith)
    void ExceptWith(const SortedSet<T>& other) {
        for (const auto& item : other.m_items) {
            m_items.erase(item);
        }
    }
    
    template<typename Container>
    void ExceptWith(const Container& other) {
        for (const auto& item : other) {
            m_items.erase(item);
        }
    }
    
    // SymmetricExceptWith method (equivalent to .NET SortedSet<T>.SymmetricExceptWith)
    void SymmetricExceptWith(const SortedSet<T>& other) {
        std::set<T> result;
        std::set_symmetric_difference(m_items.begin(), m_items.end(),
                                    other.m_items.begin(), other.m_items.end(),
                                    std::inserter(result, result.begin()));
        m_items = std::move(result);
    }
    
    // IsSubsetOf method (equivalent to .NET SortedSet<T>.IsSubsetOf)
    [[nodiscard]] bool IsSubsetOf(const SortedSet<T>& other) const {
        return std::includes(other.m_items.begin(), other.m_items.end(),
                           m_items.begin(), m_items.end());
    }
    
    // IsSupersetOf method (equivalent to .NET SortedSet<T>.IsSupersetOf)
    [[nodiscard]] bool IsSupersetOf(const SortedSet<T>& other) const {
        return std::includes(m_items.begin(), m_items.end(),
                           other.m_items.begin(), other.m_items.end());
    }
    
    // Overlaps method (equivalent to .NET SortedSet<T>.Overlaps)
    [[nodiscard]] bool Overlaps(const SortedSet<T>& other) const {
        for (const auto& item : other.m_items) {
            if (m_items.find(item) != m_items.end()) {
                return true;
            }
        }
        return false;
    }
    
    // SetEquals method (equivalent to .NET SortedSet<T>.SetEquals)
    [[nodiscard]] bool SetEquals(const SortedSet<T>& other) const {
        return m_items == other.m_items;
    }
    
    // Iterator support (for range-based for loops) - returns sorted order
    [[nodiscard]] auto begin() { return m_items.begin(); }
    [[nodiscard]] auto end() { return m_items.end(); }
    [[nodiscard]] auto begin() const { return m_items.begin(); }
    [[nodiscard]] auto end() const { return m_items.end(); }
    [[nodiscard]] auto cbegin() const { return m_items.cbegin(); }
    [[nodiscard]] auto cend() const { return m_items.cend(); }
    
    // ToString method (for debugging)
    [[nodiscard]] System::String ToString() const {
        return System::String("System.Collections.Generic.SortedSet`1[T]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
