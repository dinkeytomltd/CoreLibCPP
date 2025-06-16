
#pragma once
/*
 * UIntPtr.h
 * C++ implementation of .NET's System.UIntPtr struct
 * 
 * Key .NET UIntPtr characteristics implemented:
 * - Platform-specific unsigned integer (pointer size)
 * - Value type semantics (pass by value)
 * - Core operations and conversions
 * - Static constants (Zero)
 * - Parse and TryParse methods
 */
#include <string>
#include <limits>
#include <stdexcept>
#include <charconv>
#include <cstdint>
#include <type_traits>
#include "String.h"
#include "Object.h"

namespace System {

class UIntPtr {
private:
    std::uintptr_t m_value;

public:
    // Constants
    static constexpr std::uintptr_t MaxValue = std::numeric_limits<std::uintptr_t>::max();
    static constexpr std::uintptr_t MinValue = std::numeric_limits<std::uintptr_t>::min();
    static const UIntPtr Zero;

    // Constructors
    constexpr UIntPtr() noexcept : m_value(0) {}
    constexpr explicit UIntPtr(std::uintptr_t value) noexcept : m_value(value) {}
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, std::uintptr_t>>>
    constexpr explicit UIntPtr(T value) noexcept : m_value(static_cast<std::uintptr_t>(value)) {}
    constexpr explicit UIntPtr(void* pointer) noexcept : m_value(reinterpret_cast<std::uintptr_t>(pointer)) {}
    
    // Copy constructor
    constexpr UIntPtr(const UIntPtr& other) noexcept = default;
    
    // Assignment operator
    constexpr UIntPtr& operator=(const UIntPtr& other) noexcept = default;
    
    // Conversion operators
    [[nodiscard]] constexpr operator std::uintptr_t() const noexcept { return m_value; }
    [[nodiscard]] constexpr operator void*() const noexcept { return reinterpret_cast<void*>(m_value); }
    
    // Value property (equivalent to .NET UIntPtr.Value)
    [[nodiscard]] constexpr std::uintptr_t Value() const noexcept { return m_value; }
    
    // ToPointer method (equivalent to .NET UIntPtr.ToPointer)
    [[nodiscard]] constexpr void* ToPointer() const noexcept { return reinterpret_cast<void*>(m_value); }
    
    // ToUInt32 method (equivalent to .NET UIntPtr.ToUInt32)
    [[nodiscard]] constexpr unsigned int ToUInt32() const {
        if (m_value > std::numeric_limits<unsigned int>::max()) {
            throw std::overflow_error("Value was either too large or too small for a UInt32.");
        }
        return static_cast<unsigned int>(m_value);
    }
    
    // ToUInt64 method (equivalent to .NET UIntPtr.ToUInt64)
    [[nodiscard]] constexpr unsigned long long ToUInt64() const noexcept {
        return static_cast<unsigned long long>(m_value);
    }
    
    // Size property (equivalent to .NET UIntPtr.Size)
    [[nodiscard]] static constexpr int Size() noexcept {
        return sizeof(std::uintptr_t);
    }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const UIntPtr& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const UIntPtr& other) const noexcept {
        return m_value == other.m_value;
    }
    
    [[nodiscard]] constexpr int GetHashCode() const noexcept {
        return static_cast<size_t>(m_value);
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        return String(std::to_string(m_value).c_str());
    }
    
    // Static methods
    
    // Parse method (equivalent to .NET UIntPtr.Parse)
    [[nodiscard]] static UIntPtr Parse(const String& s) {
        try {
            return UIntPtr(static_cast<std::uintptr_t>(std::stoull(s.ToStdString())));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a UIntPtr.");
        }
    }
    
    // TryParse method (equivalent to .NET UIntPtr.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, UIntPtr& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = UIntPtr();
            return false;
        }
    }
    
    // Add method (equivalent to .NET UIntPtr.Add)
    [[nodiscard]] static UIntPtr Add(const UIntPtr& pointer, int offset) {
        return UIntPtr(pointer.m_value + offset);
    }
    
    // Subtract method (equivalent to .NET UIntPtr.Subtract)
    [[nodiscard]] static UIntPtr Subtract(const UIntPtr& pointer, int offset) {
        return UIntPtr(pointer.m_value - offset);
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const UIntPtr& left, const UIntPtr& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const UIntPtr& left, const UIntPtr& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const UIntPtr& left, const UIntPtr& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const UIntPtr& left, const UIntPtr& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const UIntPtr& left, const UIntPtr& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const UIntPtr& left, const UIntPtr& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr UIntPtr operator+(const UIntPtr& pointer, int offset) noexcept {
        return UIntPtr(pointer.m_value + offset);
    }
    
    [[nodiscard]] friend constexpr UIntPtr operator+(int offset, const UIntPtr& pointer) noexcept {
        return UIntPtr(pointer.m_value + offset);
    }
    
    [[nodiscard]] friend constexpr UIntPtr operator-(const UIntPtr& pointer, int offset) noexcept {
        return UIntPtr(pointer.m_value - offset);
    }
    
    [[nodiscard]] friend constexpr std::uintptr_t operator-(const UIntPtr& left, const UIntPtr& right) noexcept {
        return left.m_value - right.m_value;
    }
    
    // Compound assignment operators
    constexpr UIntPtr& operator+=(int offset) noexcept {
        m_value += offset;
        return *this;
    }
    
    constexpr UIntPtr& operator-=(int offset) noexcept {
        m_value -= offset;
        return *this;
    }
};

} // namespace System
