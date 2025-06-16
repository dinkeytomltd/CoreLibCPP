
#pragma once
/*
 * SortedDictionary.h
 * C++ implementation of .NET's System.Collections.Generic.SortedDictionary<K,V> class
 * 
 * Key .NET SortedDictionary<K,V> characteristics implemented:
 * - Sorted key-value collection that maintains keys in sorted order
 * - O(log n) operations for Add, Remove, ContainsKey, TryGetValue
 * - Same API as Dictionary but with sorted enumeration
 * - Type-safe generic implementation
 */
#include <map>
#include <stdexcept>
#include <vector>
#include <memory>
#include "../../../System/Object.h"
#include "../../../System/String.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename K, typename V>
class SortedDictionary {
private:
    std::map<K, V> m_items;

public:
    // Constructors
    SortedDictionary() = default;
    
    // Constructor with initializer list
    SortedDictionary(std::initializer_list<std::pair<const K, V>> items) : m_items(items) {}
    
    // Copy constructor
    SortedDictionary(const SortedDictionary<K, V>& other) = default;
    
    // Assignment operator
    SortedDictionary<K, V>& operator=(const SortedDictionary<K, V>& other) = default;
    
    // Move constructor
    SortedDictionary(SortedDictionary<K, V>&& other) noexcept = default;
    
    // Move assignment operator
    SortedDictionary<K, V>& operator=(SortedDictionary<K, V>&& other) noexcept = default;
    
    // Destructor
    ~SortedDictionary() = default;
    
    // Properties
    
    // Count property (equivalent to .NET SortedDictionary<K,V>.Count)
    [[nodiscard]] size_t Count() const {
        return m_items.size();
    }
    
    // Keys property (equivalent to .NET SortedDictionary<K,V>.Keys)
    [[nodiscard]] std::vector<K> Keys() const {
        std::vector<K> keys;
        keys.reserve(m_items.size());
        for (const auto& pair : m_items) {
            keys.push_back(pair.first);
        }
        return keys;
    }
    
    // Values property (equivalent to .NET SortedDictionary<K,V>.Values)
    [[nodiscard]] std::vector<V> Values() const {
        std::vector<V> values;
        values.reserve(m_items.size());
        for (const auto& pair : m_items) {
            values.push_back(pair.second);
        }
        return values;
    }
    
    // Methods
    
    // Add method (equivalent to .NET SortedDictionary<K,V>.Add)
    void Add(const K& key, const V& value) {
        if (m_items.find(key) != m_items.end()) {
            throw std::invalid_argument("An item with the same key has already been added");
        }
        m_items[key] = value;
    }
    
    void Add(K&& key, V&& value) {
        if (m_items.find(key) != m_items.end()) {
            throw std::invalid_argument("An item with the same key has already been added");
        }
        m_items[std::move(key)] = std::move(value);
    }
    
    // Clear method (equivalent to .NET SortedDictionary<K,V>.Clear)
    void Clear() {
        m_items.clear();
    }
    
    // ContainsKey method (equivalent to .NET SortedDictionary<K,V>.ContainsKey)
    [[nodiscard]] bool ContainsKey(const K& key) const {
        return m_items.find(key) != m_items.end();
    }
    
    // ContainsValue method (equivalent to .NET SortedDictionary<K,V>.ContainsValue)
    [[nodiscard]] bool ContainsValue(const V& value) const {
        for (const auto& pair : m_items) {
            if (pair.second == value) {
                return true;
            }
        }
        return false;
    }
    
    // Remove method (equivalent to .NET SortedDictionary<K,V>.Remove)
    bool Remove(const K& key) {
        auto it = m_items.find(key);
        if (it == m_items.end()) {
            return false;
        }
        m_items.erase(it);
        return true;
    }
    
    // TryGetValue method (equivalent to .NET SortedDictionary<K,V>.TryGetValue)
    bool TryGetValue(const K& key, V& value) const {
        auto it = m_items.find(key);
        if (it == m_items.end()) {
            return false;
        }
        value = it->second;
        return true;
    }
    
    // Indexer (equivalent to .NET SortedDictionary<K,V>[key])
    [[nodiscard]] V& operator[](const K& key) {
        return m_items[key];
    }
    
    [[nodiscard]] const V& operator[](const K& key) const {
        auto it = m_items.find(key);
        if (it == m_items.end()) {
            throw std::out_of_range("The given key was not present in the dictionary");
        }
        return it->second;
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
        return System::String("System.Collections.Generic.SortedDictionary`2[K,V]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
