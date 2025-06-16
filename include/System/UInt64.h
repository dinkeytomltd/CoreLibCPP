
#pragma once
/*
 * UInt64.h
 * C++ implementation of .NET's System.UInt64 struct (unsigned long long)
 * 
 * Key .NET UInt64 characteristics implemented:
 * - Value type semantics (pass by value)
 * - Core operations and conversions
 * - Static constants (MaxValue, MinValue)
 * - Parse and TryParse methods
 */
#include <string>
#include <limits>
#include <stdexcept>
#include <charconv>
#include "String.h"
#include "Object.h"

namespace System {

class UInt64 {
private:
    unsigned long long m_value;

public:
    // Constants
    static constexpr unsigned long long MaxValue = std::numeric_limits<unsigned long long>::max();
    static constexpr unsigned long long MinValue = std::numeric_limits<unsigned long long>::min();

    // Constructors
    constexpr UInt64() noexcept : m_value(0) {}
    constexpr explicit UInt64(unsigned long long value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr UInt64(const UInt64& other) noexcept = default;
    
    // Assignment operator
    constexpr UInt64& operator=(const UInt64& other) noexcept = default;
    
    // Conversion operator to unsigned long long (implicit in C#)
    [[nodiscard]] constexpr operator unsigned long long() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET UInt64.Value)
    [[nodiscard]] constexpr unsigned long long Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const UInt64& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const UInt64& other) const noexcept {
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
    
    // Parse method (equivalent to .NET UInt64.Parse)
    [[nodiscard]] static UInt64 Parse(const String& s) {
        try {
            return UInt64(std::stoull(s.ToStdString()));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a UInt64.");
        }
    }
    
    // TryParse method (equivalent to .NET UInt64.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, UInt64& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = UInt64(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const UInt64& left, const UInt64& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const UInt64& left, const UInt64& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const UInt64& left, const UInt64& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const UInt64& left, const UInt64& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const UInt64& left, const UInt64& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const UInt64& left, const UInt64& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr UInt64 operator+(const UInt64& left, const UInt64& right) noexcept {
        return UInt64(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt64 operator-(const UInt64& left, const UInt64& right) noexcept {
        return UInt64(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt64 operator*(const UInt64& left, const UInt64& right) noexcept {
        return UInt64(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt64 operator/(const UInt64& left, const UInt64& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return UInt64(left.m_value / right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt64 operator%(const UInt64& left, const UInt64& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return UInt64(left.m_value % right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] constexpr UInt64 operator+() const noexcept {
        return *this;
    }
    
    // Increment/decrement operators
    constexpr UInt64& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr UInt64 operator++(int) noexcept {
        UInt64 temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr UInt64& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr UInt64 operator--(int) noexcept {
        UInt64 temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr UInt64& operator+=(const UInt64& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    constexpr UInt64& operator-=(const UInt64& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    constexpr UInt64& operator*=(const UInt64& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    constexpr UInt64& operator/=(const UInt64& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value /= other.m_value;
        return *this;
    }
    
    constexpr UInt64& operator%=(const UInt64& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value %= other.m_value;
        return *this;
    }
};

} // namespace System
