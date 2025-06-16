
#pragma once

#include <atomic>
#include <type_traits>

namespace System {
namespace Threading {

class Volatile {
public:
    // Read operations
    template<typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    read(const T& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<T>*>(&location));
    }
    
    template<typename T>
    static typename std::enable_if<std::is_pointer<T>::value, T>::type
    read(const T& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<T>*>(&location));
    }
    
    // Write operations
    template<typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    write(T& location, T value) {
        std::atomic_store(reinterpret_cast<std::atomic<T>*>(&location), value);
    }
    
    template<typename T>
    static typename std::enable_if<std::is_pointer<T>::value, void>::type
    write(T& location, T value) {
        std::atomic_store(reinterpret_cast<std::atomic<T>*>(&location), value);
    }
    
    // Specialized overloads for common types
    static bool read(const bool& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<bool>*>(&location));
    }
    
    static void write(bool& location, bool value) {
        std::atomic_store(reinterpret_cast<std::atomic<bool>*>(&location), value);
    }
    
    static int8_t read(const int8_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<int8_t>*>(&location));
    }
    
    static void write(int8_t& location, int8_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<int8_t>*>(&location), value);
    }
    
    static uint8_t read(const uint8_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<uint8_t>*>(&location));
    }
    
    static void write(uint8_t& location, uint8_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<uint8_t>*>(&location), value);
    }
    
    static int16_t read(const int16_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<int16_t>*>(&location));
    }
    
    static void write(int16_t& location, int16_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<int16_t>*>(&location), value);
    }
    
    static uint16_t read(const uint16_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<uint16_t>*>(&location));
    }
    
    static void write(uint16_t& location, uint16_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<uint16_t>*>(&location), value);
    }
    
    static int32_t read(const int32_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<int32_t>*>(&location));
    }
    
    static void write(int32_t& location, int32_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<int32_t>*>(&location), value);
    }
    
    static uint32_t read(const uint32_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<uint32_t>*>(&location));
    }
    
    static void write(uint32_t& location, uint32_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<uint32_t>*>(&location), value);
    }
    
    static int64_t read(const int64_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<int64_t>*>(&location));
    }
    
    static void write(int64_t& location, int64_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<int64_t>*>(&location), value);
    }
    
    static uint64_t read(const uint64_t& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<uint64_t>*>(&location));
    }
    
    static void write(uint64_t& location, uint64_t value) {
        std::atomic_store(reinterpret_cast<std::atomic<uint64_t>*>(&location), value);
    }
    
    static float read(const float& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<float>*>(&location));
    }
    
    static void write(float& location, float value) {
        std::atomic_store(reinterpret_cast<std::atomic<float>*>(&location), value);
    }
    
    static double read(const double& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<double>*>(&location));
    }
    
    static void write(double& location, double value) {
        std::atomic_store(reinterpret_cast<std::atomic<double>*>(&location), value);
    }
    
    // Pointer specializations
    template<typename T>
    static T* read(T* const& location) {
        return std::atomic_load(reinterpret_cast<const std::atomic<T*>*>(&location));
    }
    
    template<typename T>
    static void write(T*& location, T* value) {
        std::atomic_store(reinterpret_cast<std::atomic<T*>*>(&location), value);
    }
};

} // namespace Threading
} // namespace System
