
#pragma once
/*
 * UInt32.h
 * C++ implementation of .NET's System.UInt32 struct (unsigned int)
 * 
 * Key .NET UInt32 characteristics implemented:
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

class UInt32 {
private:
    unsigned int m_value;

public:
    // Constants
    static constexpr unsigned int MaxValue = std::numeric_limits<unsigned int>::max();
    static constexpr unsigned int MinValue = std::numeric_limits<unsigned int>::min();

    // Constructors
    constexpr UInt32() noexcept : m_value(0) {}
    constexpr explicit UInt32(unsigned int value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr UInt32(const UInt32& other) noexcept = default;
    
    // Assignment operator
    constexpr UInt32& operator=(const UInt32& other) noexcept = default;
    
    // Conversion operator to unsigned int (implicit in C#)
    [[nodiscard]] constexpr operator unsigned int() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET UInt32.Value)
    [[nodiscard]] constexpr unsigned int Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const UInt32& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const UInt32& other) const noexcept {
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
    
    // Parse method (equivalent to .NET UInt32.Parse)
    [[nodiscard]] static UInt32 Parse(const String& s) {
        try {
            unsigned long long result = std::stoull(s.ToStdString());
            if (result > MaxValue) {
                throw std::out_of_range("Value was either too large or too small for a UInt32.");
            }
            return UInt32(static_cast<unsigned int>(result));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a UInt32.");
        }
    }
    
    // TryParse method (equivalent to .NET UInt32.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, UInt32& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = UInt32(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const UInt32& left, const UInt32& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const UInt32& left, const UInt32& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const UInt32& left, const UInt32& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const UInt32& left, const UInt32& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const UInt32& left, const UInt32& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const UInt32& left, const UInt32& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr UInt32 operator+(const UInt32& left, const UInt32& right) noexcept {
        return UInt32(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt32 operator-(const UInt32& left, const UInt32& right) noexcept {
        return UInt32(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt32 operator*(const UInt32& left, const UInt32& right) noexcept {
        return UInt32(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt32 operator/(const UInt32& left, const UInt32& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return UInt32(left.m_value / right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt32 operator%(const UInt32& left, const UInt32& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return UInt32(left.m_value % right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] constexpr UInt32 operator+() const noexcept {
        return *this;
    }
    
    // Increment/decrement operators
    constexpr UInt32& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr UInt32 operator++(int) noexcept {
        UInt32 temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr UInt32& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr UInt32 operator--(int) noexcept {
        UInt32 temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr UInt32& operator+=(const UInt32& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    constexpr UInt32& operator-=(const UInt32& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    constexpr UInt32& operator*=(const UInt32& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    constexpr UInt32& operator/=(const UInt32& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value /= other.m_value;
        return *this;
    }
    
    constexpr UInt32& operator%=(const UInt32& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value %= other.m_value;
        return *this;
    }
};

} // namespace System
