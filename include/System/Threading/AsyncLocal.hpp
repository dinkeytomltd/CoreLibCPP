
#pragma once

#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <functional>

namespace System {
namespace Threading {

template<typename T>
class AsyncLocal {
private:
    static thread_local std::unordered_map<void*, std::shared_ptr<T>> thread_storage;
    static std::mutex global_mutex;
    
    void* key;
    std::function<T()> value_factory;
    
public:
    AsyncLocal() : key(this) {}
    
    explicit AsyncLocal(std::function<T()> factory) 
        : key(this), value_factory(std::move(factory)) {}
    
    ~AsyncLocal() {
        std::lock_guard<std::mutex> lock(global_mutex);
        thread_storage.erase(key);
    }
    
    T get() const {
        auto it = thread_storage.find(key);
        if (it != thread_storage.end() && it->second) {
            return *(it->second);
        }
        
        if (value_factory) {
            T value = value_factory();
            thread_storage[key] = std::make_shared<T>(std::move(value));
            return *(thread_storage[key]);
        }
        
        return T{};
    }
    
    void set(const T& value) {
        thread_storage[key] = std::make_shared<T>(value);
    }
    
    void set(T&& value) {
        thread_storage[key] = std::make_shared<T>(std::move(value));
    }
    
    bool has_value() const {
        auto it = thread_storage.find(key);
        return it != thread_storage.end() && it->second != nullptr;
    }
    
    void clear() {
        thread_storage.erase(key);
    }
};

template<typename T>
thread_local std::unordered_map<void*, std::shared_ptr<T>> AsyncLocal<T>::thread_storage;

template<typename T>
std::mutex AsyncLocal<T>::global_mutex;

} // namespace Threading
} // namespace System
