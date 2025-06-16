
#pragma once

#include "System/Reflection/Type.h"
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <optional>
#include <atomic>

namespace System::Reflection
{
    /// <summary>
    /// Thread-safe cache for generic type resolution with efficient lookup
    /// and memory management. Provides compile-time type hashing and
    /// lazy initialization for optimal performance.
    /// </summary>
    class GenericTypeCache
    {
    private:
        // Type cache using type_index for efficient lookup
        mutable std::unordered_map<std::type_index, Type> _typeCache;
        mutable std::unordered_map<std::string, Type> _nameCache;
        mutable std::unordered_map<size_t, Type> _hashCache;
        
        // Thread safety
        mutable std::shared_mutex _cacheMutex;
        
        // Cache statistics
        mutable std::atomic<size_t> _cacheHits{0};
        mutable std::atomic<size_t> _cacheMisses{0};
        mutable std::atomic<size_t> _totalLookups{0};
        
        // Singleton instance
        static std::unique_ptr<GenericTypeCache> _instance;
        static std::once_flag _instanceFlag;
        
        // Private constructor for singleton
        GenericTypeCache() = default;

    public:
        /// <summary>
        /// Gets the singleton instance of the GenericTypeCache
        /// </summary>
        static GenericTypeCache& Instance()
        {
            std::call_once(_instanceFlag, []() {
                _instance = std::unique_ptr<GenericTypeCache>(new GenericTypeCache());
            });
            return *_instance;
        }

        /// <summary>
        /// Template-based type caching with compile-time optimization
        /// </summary>
        template<typename T>
        void CacheType(const Type& type)
        {
            std::unique_lock lock(_cacheMutex);
            
            // Cache by type_index
            _typeCache[std::type_index(typeid(T))] = type;
            
            // Cache by type name
            if (type.IsValid())
            {
                _nameCache[type.GetFullName().ToString()] = type;
            }
            
            // Cache by hash
            constexpr auto hash = std::hash<std::string>{}(typeid(T).name());
            _hashCache[hash] = type;
        }

        /// <summary>
        /// Template-based type lookup with compile-time optimization
        /// </summary>
        template<typename T>
        std::optional<Type> GetCachedType() const
        {
            _totalLookups.fetch_add(1, std::memory_order_relaxed);
            
            std::shared_lock lock(_cacheMutex);
            
            auto it = _typeCache.find(std::type_index(typeid(T)));
            if (it != _typeCache.end())
            {
                _cacheHits.fetch_add(1, std::memory_order_relaxed);
                return it->second;
            }
            
            _cacheMisses.fetch_add(1, std::memory_order_relaxed);
            return std::nullopt;
        }

        /// <summary>
        /// String-based type lookup for runtime type resolution
        /// </summary>
        std::optional<Type> GetCachedType(const System::String& typeName) const
        {
            _totalLookups.fetch_add(1, std::memory_order_relaxed);
            
            std::shared_lock lock(_cacheMutex);
            
            auto it = _nameCache.find(typeName.ToString());
            if (it != _nameCache.end())
            {
                _cacheHits.fetch_add(1, std::memory_order_relaxed);
                return it->second;
            }
            
            _cacheMisses.fetch_add(1, std::memory_order_relaxed);
            return std::nullopt;
        }

        /// <summary>
        /// Hash-based type lookup for efficient compile-time resolution
        /// </summary>
        std::optional<Type> GetCachedType(size_t typeHash) const
        {
            _totalLookups.fetch_add(1, std::memory_order_relaxed);
            
            std::shared_lock lock(_cacheMutex);
            
            auto it = _hashCache.find(typeHash);
            if (it != _hashCache.end())
            {
                _cacheHits.fetch_add(1, std::memory_order_relaxed);
                return it->second;
            }
            
            _cacheMisses.fetch_add(1, std::memory_order_relaxed);
            return std::nullopt;
        }

        /// <summary>
        /// Cache a type by string name
        /// </summary>
        void CacheType(const System::String& typeName, const Type& type)
        {
            std::unique_lock lock(_cacheMutex);
            _nameCache[typeName.ToString()] = type;
        }

        /// <summary>
        /// Cache a type by hash
        /// </summary>
        void CacheType(size_t typeHash, const Type& type)
        {
            std::unique_lock lock(_cacheMutex);
            _hashCache[typeHash] = type;
        }

        /// <summary>
        /// Clear all cached types
        /// </summary>
        void ClearCache()
        {
            std::unique_lock lock(_cacheMutex);
            _typeCache.clear();
            _nameCache.clear();
            _hashCache.clear();
            
            // Reset statistics
            _cacheHits.store(0, std::memory_order_relaxed);
            _cacheMisses.store(0, std::memory_order_relaxed);
            _totalLookups.store(0, std::memory_order_relaxed);
        }

        /// <summary>
        /// Get cache statistics
        /// </summary>
        struct CacheStatistics
        {
            size_t totalLookups;
            size_t cacheHits;
            size_t cacheMisses;
            double hitRatio;
            size_t typeCacheSize;
            size_t nameCacheSize;
            size_t hashCacheSize;
        };

        CacheStatistics GetStatistics() const
        {
            std::shared_lock lock(_cacheMutex);
            
            auto totalLookups = _totalLookups.load(std::memory_order_relaxed);
            auto cacheHits = _cacheHits.load(std::memory_order_relaxed);
            auto cacheMisses = _cacheMisses.load(std::memory_order_relaxed);
            
            return CacheStatistics{
                totalLookups,
                cacheHits,
                cacheMisses,
                totalLookups > 0 ? static_cast<double>(cacheHits) / totalLookups : 0.0,
                _typeCache.size(),
                _nameCache.size(),
                _hashCache.size()
            };
        }

        /// <summary>
        /// Check if a type is cached
        /// </summary>
        template<typename T>
        bool IsTypeCached() const
        {
            std::shared_lock lock(_cacheMutex);
            return _typeCache.find(std::type_index(typeid(T))) != _typeCache.end();
        }

        /// <summary>
        /// Check if a type name is cached
        /// </summary>
        bool IsTypeCached(const System::String& typeName) const
        {
            std::shared_lock lock(_cacheMutex);
            return _nameCache.find(typeName.ToString()) != _nameCache.end();
        }

        /// <summary>
        /// Preload commonly used types for better performance
        /// </summary>
        void PreloadCommonTypes()
        {
            // Preload primitive types
            CacheType<int>(Type::GetType<int>());
            CacheType<bool>(Type::GetType<bool>());
            CacheType<double>(Type::GetType<double>());
            CacheType<float>(Type::GetType<float>());
            CacheType<char>(Type::GetType<char>());
            CacheType<long>(Type::GetType<long>());
            CacheType<short>(Type::GetType<short>());
            CacheType<unsigned int>(Type::GetType<unsigned int>());
            CacheType<unsigned long>(Type::GetType<unsigned long>());
            CacheType<unsigned short>(Type::GetType<unsigned short>());
            
            // Preload common system types
            CacheType<System::String>(Type::GetType<System::String>());
            CacheType<System::Object>(Type::GetType<System::Object>());
            
            // Preload void type
            CacheType<void>(Type::GetType<void>());
        }

        /// <summary>
        /// Get memory usage information
        /// </summary>
        struct MemoryInfo
        {
            size_t totalMemoryBytes;
            size_t typeCacheMemory;
            size_t nameCacheMemory;
            size_t hashCacheMemory;
        };

        MemoryInfo GetMemoryInfo() const
        {
            std::shared_lock lock(_cacheMutex);
            
            // Estimate memory usage (simplified calculation)
            size_t typeCacheMemory = _typeCache.size() * (sizeof(std::type_index) + sizeof(Type));
            size_t nameCacheMemory = 0;
            for (const auto& pair : _nameCache)
            {
                nameCacheMemory += pair.first.size() + sizeof(Type);
            }
            size_t hashCacheMemory = _hashCache.size() * (sizeof(size_t) + sizeof(Type));
            
            return MemoryInfo{
                typeCacheMemory + nameCacheMemory + hashCacheMemory,
                typeCacheMemory,
                nameCacheMemory,
                hashCacheMemory
            };
        }

        /// <summary>
        /// Remove a specific type from cache
        /// </summary>
        template<typename T>
        bool RemoveType()
        {
            std::unique_lock lock(_cacheMutex);
            
            auto typeIndex = std::type_index(typeid(T));
            auto it = _typeCache.find(typeIndex);
            if (it != _typeCache.end())
            {
                // Also remove from name cache if present
                auto typeName = it->second.GetFullName().ToString();
                _nameCache.erase(typeName);
                
                // Remove from hash cache
                constexpr auto hash = std::hash<std::string>{}(typeid(T).name());
                _hashCache.erase(hash);
                
                _typeCache.erase(it);
                return true;
            }
            return false;
        }

        /// <summary>
        /// Remove a type by name from cache
        /// </summary>
        bool RemoveType(const System::String& typeName)
        {
            std::unique_lock lock(_cacheMutex);
            
            auto it = _nameCache.find(typeName.ToString());
            if (it != _nameCache.end())
            {
                _nameCache.erase(it);
                return true;
            }
            return false;
        }

        /// <summary>
        /// Set cache size limits to prevent excessive memory usage
        /// </summary>
        void SetCacheLimits(size_t maxTypeCache, size_t maxNameCache, size_t maxHashCache)
        {
            std::unique_lock lock(_cacheMutex);
            
            // Simple LRU-style cleanup if limits exceeded
            if (_typeCache.size() > maxTypeCache)
            {
                // Remove oldest entries (simplified - would need proper LRU implementation)
                auto toRemove = _typeCache.size() - maxTypeCache;
                auto it = _typeCache.begin();
                for (size_t i = 0; i < toRemove && it != _typeCache.end(); ++i)
                {
                    it = _typeCache.erase(it);
                }
            }
            
            if (_nameCache.size() > maxNameCache)
            {
                auto toRemove = _nameCache.size() - maxNameCache;
                auto it = _nameCache.begin();
                for (size_t i = 0; i < toRemove && it != _nameCache.end(); ++i)
                {
                    it = _nameCache.erase(it);
                }
            }
            
            if (_hashCache.size() > maxHashCache)
            {
                auto toRemove = _hashCache.size() - maxHashCache;
                auto it = _hashCache.begin();
                for (size_t i = 0; i < toRemove && it != _hashCache.end(); ++i)
                {
                    it = _hashCache.erase(it);
                }
            }
        }
    };
}
