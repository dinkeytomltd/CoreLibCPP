#pragma once
/*
 * Int64.h
 * C++ implementation of .NET's System.Int64 struct
 * 
 * Key .NET Int64 characteristics implemented:
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

class Int64 {
private:
    long long m_value;

public:
    // Constants
    static constexpr long long MaxValue = std::numeric_limits<long long>::max();
    static constexpr long long MinValue = std::numeric_limits<long long>::min();

    // Constructors
    constexpr Int64() noexcept : m_value(0) {}
    constexpr explicit Int64(long long value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Int64(const Int64& other) noexcept = default;
    
    // Assignment operator
    constexpr Int64& operator=(const Int64& other) noexcept = default;
    
    // Conversion operator to long long (implicit in C#)
    [[nodiscard]] constexpr operator long long() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Int64.Value)
    [[nodiscard]] constexpr long long Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const Int64& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const Int64& other) const noexcept {
        return m_value == other.m_value;
    }
    
    [[nodiscard]] constexpr int GetHashCode() const noexcept {
        // Simple hash code implementation for 64-bit value
        return static_cast<size_t>(m_value ^ (m_value >> 32));
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        return String(std::to_string(m_value).c_str());
    }
    
    // Static methods
    
    // Parse method (equivalent to .NET Int64.Parse)
    [[nodiscard]] static Int64 Parse(const String& s) {
        try {
            return Int64(std::stoll(s.ToStdString()));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for an Int64.");
        }
    }
    
    // TryParse method (equivalent to .NET Int64.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Int64& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Int64(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const Int64& left, const Int64& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const Int64& left, const Int64& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const Int64& left, const Int64& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const Int64& left, const Int64& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const Int64& left, const Int64& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const Int64& left, const Int64& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr Int64 operator+(const Int64& left, const Int64& right) noexcept {
        return Int64(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int64 operator-(const Int64& left, const Int64& right) noexcept {
        return Int64(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int64 operator*(const Int64& left, const Int64& right) noexcept {
        return Int64(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int64 operator/(const Int64& left, const Int64& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Int64(left.m_value / right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int64 operator%(const Int64& left, const Int64& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Int64(left.m_value % right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] constexpr Int64 operator+() const noexcept {
        return *this;
    }
    
    [[nodiscard]] constexpr Int64 operator-() const noexcept {
        return Int64(-m_value);
    }
    
    // Increment/decrement operators
    constexpr Int64& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr Int64 operator++(int) noexcept {
        Int64 temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr Int64& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr Int64 operator--(int) noexcept {
        Int64 temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr Int64& operator+=(const Int64& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    constexpr Int64& operator-=(const Int64& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    constexpr Int64& operator*=(const Int64& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    constexpr Int64& operator/=(const Int64& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value /= other.m_value;
        return *this;
    }
    
    constexpr Int64& operator%=(const Int64& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value %= other.m_value;
        return *this;
    }
    
    // Bitwise operators
    [[nodiscard]] friend constexpr Int64 operator&(const Int64& left, const Int64& right) noexcept {
        return Int64(left.m_value & right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int64 operator|(const Int64& left, const Int64& right) noexcept {
        return Int64(left.m_value | right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int64 operator^(const Int64& left, const Int64& right) noexcept {
        return Int64(left.m_value ^ right.m_value);
    }
    
    [[nodiscard]] constexpr Int64 operator~() const noexcept {
        return Int64(~m_value);
    }
    
    // Bitwise compound assignment operators
    constexpr Int64& operator&=(const Int64& other) noexcept {
        m_value &= other.m_value;
        return *this;
    }
    
    constexpr Int64& operator|=(const Int64& other) noexcept {
        m_value |= other.m_value;
        return *this;
    }
    
    constexpr Int64& operator^=(const Int64& other) noexcept {
        m_value ^= other.m_value;
        return *this;
    }
};

} // namespace System
