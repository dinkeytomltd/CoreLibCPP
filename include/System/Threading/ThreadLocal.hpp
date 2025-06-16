
#pragma once

#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <functional>

namespace System {
namespace Threading {

template<typename T>
class ThreadLocal {
private:
    static thread_local std::unordered_map<void*, std::shared_ptr<T>> thread_storage;
    static std::mutex global_mutex;
    
    void* key;
    std::function<T()> value_factory;
    bool track_all_values;
    
public:
    ThreadLocal() : key(this), track_all_values(false) {}
    
    explicit ThreadLocal(std::function<T()> factory) 
        : key(this), value_factory(std::move(factory)), track_all_values(false) {}
    
    ThreadLocal(std::function<T()> factory, bool trackAllValues)
        : key(this), value_factory(std::move(factory)), track_all_values(trackAllValues) {}
    
    ~ThreadLocal() {
        std::lock_guard<std::mutex> lock(global_mutex);
        thread_storage.erase(key);
    }
    
    T& get() {
        auto it = thread_storage.find(key);
        if (it != thread_storage.end() && it->second) {
            return *(it->second);
        }
        
        if (value_factory) {
            T value = value_factory();
            thread_storage[key] = std::make_shared<T>(std::move(value));
        } else {
            thread_storage[key] = std::make_shared<T>();
        }
        
        return *(thread_storage[key]);
    }
    
    const T& get() const {
        auto it = thread_storage.find(key);
        if (it != thread_storage.end() && it->second) {
            return *(it->second);
        }
        
        static const T default_value{};
        return default_value;
    }
    
    void set(const T& value) {
        thread_storage[key] = std::make_shared<T>(value);
    }
    
    void set(T&& value) {
        thread_storage[key] = std::make_shared<T>(std::move(value));
    }
    
    bool is_value_created() const {
        auto it = thread_storage.find(key);
        return it != thread_storage.end() && it->second != nullptr;
    }
    
    T& operator*() { return get(); }
    const T& operator*() const { return get(); }
    
    T* operator->() { return &get(); }
    const T* operator->() const { return &get(); }
};

template<typename T>
thread_local std::unordered_map<void*, std::shared_ptr<T>> ThreadLocal<T>::thread_storage;

template<typename T>
std::mutex ThreadLocal<T>::global_mutex;

} // namespace Threading
} // namespace System
