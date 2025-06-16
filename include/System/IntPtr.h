
#pragma once
/*
 * IntPtr.h
 * C++ implementation of .NET's System.IntPtr struct
 * 
 * Key .NET IntPtr characteristics implemented:
 * - Platform-specific signed integer (pointer size)
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

class IntPtr {
private:
    std::intptr_t m_value;

public:
    // Constants
    static constexpr std::intptr_t MaxValue = std::numeric_limits<std::intptr_t>::max();
    static constexpr std::intptr_t MinValue = std::numeric_limits<std::intptr_t>::min();
    static const IntPtr Zero;

    // Constructors
    constexpr IntPtr() noexcept : m_value(0) {}
    constexpr explicit IntPtr(std::intptr_t value) noexcept : m_value(value) {}
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, std::intptr_t>>>
    constexpr explicit IntPtr(T value) noexcept : m_value(static_cast<std::intptr_t>(value)) {}
    constexpr explicit IntPtr(void* pointer) noexcept : m_value(reinterpret_cast<std::intptr_t>(pointer)) {}
    
    // Copy constructor
    constexpr IntPtr(const IntPtr& other) noexcept = default;
    
    // Assignment operator
    constexpr IntPtr& operator=(const IntPtr& other) noexcept = default;
    
    // Conversion operators
    [[nodiscard]] constexpr operator std::intptr_t() const noexcept { return m_value; }
    [[nodiscard]] constexpr operator void*() const noexcept { return reinterpret_cast<void*>(m_value); }
    
    // Value property (equivalent to .NET IntPtr.Value)
    [[nodiscard]] constexpr std::intptr_t Value() const noexcept { return m_value; }
    
    // ToPointer method (equivalent to .NET IntPtr.ToPointer)
    [[nodiscard]] constexpr void* ToPointer() const noexcept { return reinterpret_cast<void*>(m_value); }
    
    // ToInt32 method (equivalent to .NET IntPtr.ToInt32)
    [[nodiscard]] constexpr int ToInt32() const {
        if (m_value > std::numeric_limits<int>::max() || m_value < std::numeric_limits<int>::min()) {
            throw std::overflow_error("Value was either too large or too small for an Int32.");
        }
        return static_cast<int>(m_value);
    }
    
    // ToInt64 method (equivalent to .NET IntPtr.ToInt64)
    [[nodiscard]] constexpr long long ToInt64() const noexcept {
        return static_cast<long long>(m_value);
    }
    
    // Size property (equivalent to .NET IntPtr.Size)
    [[nodiscard]] static constexpr int Size() noexcept {
        return sizeof(std::intptr_t);
    }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const IntPtr& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const IntPtr& other) const noexcept {
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
    
    // Parse method (equivalent to .NET IntPtr.Parse)
    [[nodiscard]] static IntPtr Parse(const String& s) {
        try {
            return IntPtr(static_cast<std::intptr_t>(std::stoll(s.ToStdString())));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for an IntPtr.");
        }
    }
    
    // TryParse method (equivalent to .NET IntPtr.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, IntPtr& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = IntPtr();
            return false;
        }
    }
    
    // Add method (equivalent to .NET IntPtr.Add)
    [[nodiscard]] static IntPtr Add(const IntPtr& pointer, int offset) {
        return IntPtr(pointer.m_value + offset);
    }
    
    // Subtract method (equivalent to .NET IntPtr.Subtract)
    [[nodiscard]] static IntPtr Subtract(const IntPtr& pointer, int offset) {
        return IntPtr(pointer.m_value - offset);
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const IntPtr& left, const IntPtr& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const IntPtr& left, const IntPtr& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const IntPtr& left, const IntPtr& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const IntPtr& left, const IntPtr& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const IntPtr& left, const IntPtr& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const IntPtr& left, const IntPtr& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr IntPtr operator+(const IntPtr& pointer, int offset) noexcept {
        return IntPtr(pointer.m_value + offset);
    }
    
    [[nodiscard]] friend constexpr IntPtr operator+(int offset, const IntPtr& pointer) noexcept {
        return IntPtr(pointer.m_value + offset);
    }
    
    [[nodiscard]] friend constexpr IntPtr operator-(const IntPtr& pointer, int offset) noexcept {
        return IntPtr(pointer.m_value - offset);
    }
    
    [[nodiscard]] friend constexpr std::intptr_t operator-(const IntPtr& left, const IntPtr& right) noexcept {
        return left.m_value - right.m_value;
    }
    
    // Compound assignment operators
    constexpr IntPtr& operator+=(int offset) noexcept {
        m_value += offset;
        return *this;
    }
    
    constexpr IntPtr& operator-=(int offset) noexcept {
        m_value -= offset;
        return *this;
    }
};

} // namespace System
