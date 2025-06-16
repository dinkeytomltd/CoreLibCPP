#pragma once
/*
 * Int32.h
 * C++ implementation of .NET's System.Int32 struct
 * 
 * Key .NET Int32 characteristics implemented:
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

class Int32 {
private:
    int m_value;

public:
    // Constants
    static constexpr int MaxValue = std::numeric_limits<int>::max();
    static constexpr int MinValue = std::numeric_limits<int>::min();

    // Constructors
    constexpr Int32() noexcept : m_value(0) {}
    constexpr explicit Int32(int value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Int32(const Int32& other) noexcept = default;
    
    // Assignment operator
    constexpr Int32& operator=(const Int32& other) noexcept = default;
    
    // Conversion operator to int (implicit in C#)
    [[nodiscard]] constexpr operator int() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Int32.Value)
    [[nodiscard]] constexpr int Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const Int32& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const Int32& other) const noexcept {
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
    
    // Parse method (equivalent to .NET Int32.Parse)
    [[nodiscard]] static Int32 Parse(const String& s) {
        try {
            return Int32(std::stoi(s.ToStdString()));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for an Int32.");
        }
    }
    
    // TryParse method (equivalent to .NET Int32.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Int32& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Int32(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const Int32& left, const Int32& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const Int32& left, const Int32& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const Int32& left, const Int32& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const Int32& left, const Int32& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const Int32& left, const Int32& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const Int32& left, const Int32& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr Int32 operator+(const Int32& left, const Int32& right) noexcept {
        return Int32(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int32 operator-(const Int32& left, const Int32& right) noexcept {
        return Int32(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int32 operator*(const Int32& left, const Int32& right) noexcept {
        return Int32(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int32 operator/(const Int32& left, const Int32& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Int32(left.m_value / right.m_value);
    }
    
    [[nodiscard]] friend constexpr Int32 operator%(const Int32& left, const Int32& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Int32(left.m_value % right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] constexpr Int32 operator+() const noexcept {
        return *this;
    }
    
    [[nodiscard]] constexpr Int32 operator-() const noexcept {
        return Int32(-m_value);
    }
    
    // Increment/decrement operators
    constexpr Int32& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr Int32 operator++(int) noexcept {
        Int32 temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr Int32& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr Int32 operator--(int) noexcept {
        Int32 temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr Int32& operator+=(const Int32& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    constexpr Int32& operator-=(const Int32& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    constexpr Int32& operator*=(const Int32& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    constexpr Int32& operator/=(const Int32& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value /= other.m_value;
        return *this;
    }
    
    constexpr Int32& operator%=(const Int32& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value %= other.m_value;
        return *this;
    }
};

} // namespace System
