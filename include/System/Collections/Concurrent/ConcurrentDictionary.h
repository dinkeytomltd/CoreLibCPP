
#pragma once

#include "System/Object.h"
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <functional>

namespace System::Collections::Concurrent
{
    template<typename K, typename V>
    class ConcurrentDictionary : public System::Object
    {
    private:
        std::unordered_map<K, V> dictionary;
        mutable std::shared_mutex rwMutex;

    public:
        ConcurrentDictionary() = default;
        ConcurrentDictionary(const std::unordered_map<K, V>& dictionary);

        // Access
        V& operator[](const K& key);
        const V& operator[](const K& key) const;

        // Modification
        bool TryAdd(const K& key, const V& value);
        bool TryGetValue(const K& key, V& value) const;
        bool TryUpdate(const K& key, const V& newValue, const V& comparisonValue);
        bool TryRemove(const K& key, V& value);
        V AddOrUpdate(const K& key, const V& addValue, std::function<V(const K&, const V&)> updateValueFactory);
        V AddOrUpdate(const K& key, std::function<V(const K&)> addValueFactory, std::function<V(const K&, const V&)> updateValueFactory);
        V GetOrAdd(const K& key, const V& value);
        V GetOrAdd(const K& key, std::function<V(const K&)> valueFactory);

        // Properties
        int GetCount() const;
        bool GetIsEmpty() const;
        std::vector<K> GetKeys() const;
        std::vector<V> GetValues() const;

        // IEnumerable
        std::vector<std::pair<K, V>> ToArray();
        bool ContainsKey(const K& key) const;
        void Clear();
    };
}
