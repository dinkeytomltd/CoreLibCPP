
#pragma once
/*
 * Dictionary.h
 * C++ implementation of .NET's System.Collections.Generic.Dictionary<K,V> class
 * 
 * Key .NET Dictionary<K,V> characteristics implemented:
 * - Hash table-based key-value collection
 * - Type-safe generic implementation
 * - Core collection operations (Add, Remove, ContainsKey, TryGetValue, etc.)
 * - Indexer access
 * - Keys and Values properties
 */
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <memory>
#include "../../../System/Object.h"
#include "../../../System/String.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename K, typename V>
class Dictionary {
private:
    std::unordered_map<K, V> m_items;

public:
    // Constructors
    Dictionary() = default;
    
    // Constructor with initial capacity
    explicit Dictionary(size_t capacity) {
        m_items.reserve(capacity);
    }
    
    // Constructor with initializer list
    Dictionary(std::initializer_list<std::pair<const K, V>> items) : m_items(items) {}
    
    // Copy constructor
    Dictionary(const Dictionary<K, V>& other) = default;
    
    // Assignment operator
    Dictionary<K, V>& operator=(const Dictionary<K, V>& other) = default;
    
    // Move constructor
    Dictionary(Dictionary<K, V>&& other) noexcept = default;
    
    // Move assignment operator
    Dictionary<K, V>& operator=(Dictionary<K, V>&& other) noexcept = default;
    
    // Destructor
    ~Dictionary() = default;
    
    // Properties
    
    // Count property (equivalent to .NET Dictionary<K,V>.Count)
    [[nodiscard]] size_t Count() const {
        return m_items.size();
    }
    
    // Keys property (equivalent to .NET Dictionary<K,V>.Keys)
    [[nodiscard]] std::vector<K> Keys() const {
        std::vector<K> keys;
        keys.reserve(m_items.size());
        for (const auto& pair : m_items) {
            keys.push_back(pair.first);
        }
        return keys;
    }
    
    // Values property (equivalent to .NET Dictionary<K,V>.Values)
    [[nodiscard]] std::vector<V> Values() const {
        std::vector<V> values;
        values.reserve(m_items.size());
        for (const auto& pair : m_items) {
            values.push_back(pair.second);
        }
        return values;
    }
    
    // Methods
    
    // Add method (equivalent to .NET Dictionary<K,V>.Add)
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
    
    // Clear method (equivalent to .NET Dictionary<K,V>.Clear)
    void Clear() {
        m_items.clear();
    }
    
    // ContainsKey method (equivalent to .NET Dictionary<K,V>.ContainsKey)
    [[nodiscard]] bool ContainsKey(const K& key) const {
        return m_items.find(key) != m_items.end();
    }
    
    // ContainsValue method (equivalent to .NET Dictionary<K,V>.ContainsValue)
    [[nodiscard]] bool ContainsValue(const V& value) const {
        for (const auto& pair : m_items) {
            if (pair.second == value) {
                return true;
            }
        }
        return false;
    }
    
    // Remove method (equivalent to .NET Dictionary<K,V>.Remove)
    bool Remove(const K& key) {
        auto it = m_items.find(key);
        if (it == m_items.end()) {
            return false;
        }
        m_items.erase(it);
        return true;
    }
    
    // TryGetValue method (equivalent to .NET Dictionary<K,V>.TryGetValue)
    bool TryGetValue(const K& key, V& value) const {
        auto it = m_items.find(key);
        if (it == m_items.end()) {
            return false;
        }
        value = it->second;
        return true;
    }
    
    // Indexer (equivalent to .NET Dictionary<K,V>[key])
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
    
    // Iterator support (for range-based for loops)
    [[nodiscard]] auto begin() { return m_items.begin(); }
    [[nodiscard]] auto end() { return m_items.end(); }
    [[nodiscard]] auto begin() const { return m_items.begin(); }
    [[nodiscard]] auto end() const { return m_items.end(); }
    [[nodiscard]] auto cbegin() const { return m_items.cbegin(); }
    [[nodiscard]] auto cend() const { return m_items.cend(); }
    
    // ToString method (for debugging)
    [[nodiscard]] System::String ToString() const {
        return System::String("System.Collections.Generic.Dictionary`2[K,V]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
