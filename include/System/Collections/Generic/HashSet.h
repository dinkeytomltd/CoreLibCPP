
#pragma once
/*
 * HashSet.h
 * C++ implementation of .NET's System.Collections.Generic.HashSet<T> class
 * 
 * Key .NET HashSet<T> characteristics implemented:
 * - Hash table-based set collection for unique elements
 * - Type-safe generic implementation
 * - Core collection operations (Add, Remove, Contains, etc.)
 * - Set operations (UnionWith, IntersectWith, ExceptWith)
 * - No duplicate elements
 */
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include "../../../System/Object.h"
#include "../../../System/String.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename T>
class HashSet {
private:
    std::unordered_set<T> m_items;

public:
    // Constructors
    HashSet() = default;
    
    // Constructor with initial capacity
    explicit HashSet(size_t capacity) {
        m_items.reserve(capacity);
    }
    
    // Constructor with initializer list
    HashSet(std::initializer_list<T> items) : m_items(items) {}
    
    // Copy constructor
    HashSet(const HashSet<T>& other) = default;
    
    // Assignment operator
    HashSet<T>& operator=(const HashSet<T>& other) = default;
    
    // Move constructor
    HashSet(HashSet<T>&& other) noexcept = default;
    
    // Move assignment operator
    HashSet<T>& operator=(HashSet<T>&& other) noexcept = default;
    
    // Destructor
    ~HashSet() = default;
    
    // Properties
    
    // Count property (equivalent to .NET HashSet<T>.Count)
    [[nodiscard]] size_t Count() const {
        return m_items.size();
    }
    
    // Methods
    
    // Add method (equivalent to .NET HashSet<T>.Add)
    bool Add(const T& item) {
        auto result = m_items.insert(item);
        return result.second; // Returns true if insertion took place
    }
    
    bool Add(T&& item) {
        auto result = m_items.insert(std::move(item));
        return result.second; // Returns true if insertion took place
    }
    
    // Clear method (equivalent to .NET HashSet<T>.Clear)
    void Clear() {
        m_items.clear();
    }
    
    // Contains method (equivalent to .NET HashSet<T>.Contains)
    [[nodiscard]] bool Contains(const T& item) const {
        return m_items.find(item) != m_items.end();
    }
    
    // Remove method (equivalent to .NET HashSet<T>.Remove)
    bool Remove(const T& item) {
        auto it = m_items.find(item);
        if (it == m_items.end()) {
            return false;
        }
        m_items.erase(it);
        return true;
    }
    
    // Set operations
    
    // UnionWith method (equivalent to .NET HashSet<T>.UnionWith)
    void UnionWith(const HashSet<T>& other) {
        for (const auto& item : other.m_items) {
            m_items.insert(item);
        }
    }
    
    // IntersectWith method (equivalent to .NET HashSet<T>.IntersectWith)
    void IntersectWith(const HashSet<T>& other) {
        std::unordered_set<T> result;
        for (const auto& item : m_items) {
            if (other.Contains(item)) {
                result.insert(item);
            }
        }
        m_items = std::move(result);
    }
    
    // ExceptWith method (equivalent to .NET HashSet<T>.ExceptWith)
    void ExceptWith(const HashSet<T>& other) {
        for (const auto& item : other.m_items) {
            m_items.erase(item);
        }
    }
    
    // SymmetricExceptWith method (equivalent to .NET HashSet<T>.SymmetricExceptWith)
    void SymmetricExceptWith(const HashSet<T>& other) {
        std::unordered_set<T> result;
        
        // Add items from this set that are not in other
        for (const auto& item : m_items) {
            if (!other.Contains(item)) {
                result.insert(item);
            }
        }
        
        // Add items from other set that are not in this
        for (const auto& item : other.m_items) {
            if (!Contains(item)) {
                result.insert(item);
            }
        }
        
        m_items = std::move(result);
    }
    
    // IsSubsetOf method (equivalent to .NET HashSet<T>.IsSubsetOf)
    [[nodiscard]] bool IsSubsetOf(const HashSet<T>& other) const {
        if (m_items.size() > other.m_items.size()) {
            return false;
        }
        
        for (const auto& item : m_items) {
            if (!other.Contains(item)) {
                return false;
            }
        }
        return true;
    }
    
    // IsSupersetOf method (equivalent to .NET HashSet<T>.IsSupersetOf)
    [[nodiscard]] bool IsSupersetOf(const HashSet<T>& other) const {
        return other.IsSubsetOf(*this);
    }
    
    // Overlaps method (equivalent to .NET HashSet<T>.Overlaps)
    [[nodiscard]] bool Overlaps(const HashSet<T>& other) const {
        for (const auto& item : m_items) {
            if (other.Contains(item)) {
                return true;
            }
        }
        return false;
    }
    
    // SetEquals method (equivalent to .NET HashSet<T>.SetEquals)
    [[nodiscard]] bool SetEquals(const HashSet<T>& other) const {
        if (m_items.size() != other.m_items.size()) {
            return false;
        }
        
        for (const auto& item : m_items) {
            if (!other.Contains(item)) {
                return false;
            }
        }
        return true;
    }
    
    // Iterator support (for range-based for loops)
    [[nodiscard]] auto begin() { return m_items.begin(); }
    [[nodiscard]] auto end() { return m_items.end(); }
    [[nodiscard]] auto begin() const { return m_items.begin(); }
    [[nodiscard]] auto end() const { return m_items.end(); }
    [[nodiscard]] auto cbegin() const { return m_items.cbegin(); }
    [[nodiscard]] auto cend() const { return m_items.cend(); }
    
    // ToArray method (equivalent to .NET HashSet<T>.ToArray)
    [[nodiscard]] std::vector<T> ToArray() const {
        std::vector<T> result;
        result.reserve(m_items.size());
        for (const auto& item : m_items) {
            result.push_back(item);
        }
        return result;
    }
    
    // ToString method (for debugging)
    [[nodiscard]] System::String ToString() const {
        return System::String("System.Collections.Generic.HashSet`1[T]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
