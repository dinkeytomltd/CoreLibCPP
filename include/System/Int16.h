#pragma once
/*
 * Int16.h
 * C++ implementation of .NET's System.Int16 struct
 * 
 * Key .NET Int16 characteristics implemented:
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

class Int16 {
private:
    short m_value;

public:
    // Constants
    static constexpr short MaxValue = std::numeric_limits<short>::max();
    static constexpr short MinValue = std::numeric_limits<short>::min();

    // Constructors
    constexpr Int16() noexcept : m_value(0) {}
    constexpr explicit Int16(short value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Int16(const Int16& other) noexcept = default;
    
    // Assignment operator
    constexpr Int16& operator=(const Int16& other) noexcept = default;
    
    // Conversion operator to short (implicit in C#)
    [[nodiscard]] constexpr operator short() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Int16.Value)
    [[nodiscard]] constexpr short Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const Int16& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const Int16& other) const noexcept {
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
    
    // Parse method (equivalent to .NET Int16.Parse)
    [[nodiscard]] static Int16 Parse(const String& s) {
        try {
            int value = std::stoi(s.ToStdString());
            if (value > MaxValue || value < MinValue) {
                throw std::out_of_range("Value was either too large or too small for an Int16.");
            }
            return Int16(static_cast<short>(value));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for an Int16.");
        }
    }
    
    // TryParse method (equivalent to .NET Int16.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Int16& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Int16(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const Int16& left, const Int16& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const Int16& left, const Int16& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const Int16& left, const Int16& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const Int16& left, const Int16& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const Int16& left, const Int16& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const Int16& left, const Int16& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr Int16 operator+(const Int16& left, const Int16& right) noexcept {
        return Int16(static_cast<short>(left.m_value + right.m_value));
    }
    
    [[nodiscard]] friend constexpr Int16 operator-(const Int16& left, const Int16& right) noexcept {
        return Int16(static_cast<short>(left.m_value - right.m_value));
    }
    
    [[nodiscard]] friend constexpr Int16 operator*(const Int16& left, const Int16& right) noexcept {
        return Int16(static_cast<short>(left.m_value * right.m_value));
    }
    
    [[nodiscard]] friend constexpr Int16 operator/(const Int16& left, const Int16& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Int16(static_cast<short>(left.m_value / right.m_value));
    }
    
    [[nodiscard]] friend constexpr Int16 operator%(const Int16& left, const Int16& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Int16(static_cast<short>(left.m_value % right.m_value));
    }
    
    // Unary operators
    [[nodiscard]] constexpr Int16 operator+() const noexcept {
        return *this;
    }
    
    [[nodiscard]] constexpr Int16 operator-() const noexcept {
        return Int16(static_cast<short>(-m_value));
    }
    
    // Increment/decrement operators
    constexpr Int16& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr Int16 operator++(int) noexcept {
        Int16 temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr Int16& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr Int16 operator--(int) noexcept {
        Int16 temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr Int16& operator+=(const Int16& other) noexcept {
        m_value = static_cast<short>(m_value + other.m_value);
        return *this;
    }
    
    constexpr Int16& operator-=(const Int16& other) noexcept {
        m_value = static_cast<short>(m_value - other.m_value);
        return *this;
    }
    
    constexpr Int16& operator*=(const Int16& other) noexcept {
        m_value = static_cast<short>(m_value * other.m_value);
        return *this;
    }
    
    constexpr Int16& operator/=(const Int16& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value = static_cast<short>(m_value / other.m_value);
        return *this;
    }
    
    constexpr Int16& operator%=(const Int16& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value = static_cast<short>(m_value % other.m_value);
        return *this;
    }
    
    // Bitwise operators
    [[nodiscard]] friend constexpr Int16 operator&(const Int16& left, const Int16& right) noexcept {
        return Int16(static_cast<short>(left.m_value & right.m_value));
    }
    
    [[nodiscard]] friend constexpr Int16 operator|(const Int16& left, const Int16& right) noexcept {
        return Int16(static_cast<short>(left.m_value | right.m_value));
    }
    
    [[nodiscard]] friend constexpr Int16 operator^(const Int16& left, const Int16& right) noexcept {
        return Int16(static_cast<short>(left.m_value ^ right.m_value));
    }
    
    [[nodiscard]] constexpr Int16 operator~() const noexcept {
        return Int16(static_cast<short>(~m_value));
    }
    
    // Bitwise compound assignment operators
    constexpr Int16& operator&=(const Int16& other) noexcept {
        m_value &= other.m_value;
        return *this;
    }
    
    constexpr Int16& operator|=(const Int16& other) noexcept {
        m_value |= other.m_value;
        return *this;
    }
    
    constexpr Int16& operator^=(const Int16& other) noexcept {
        m_value ^= other.m_value;
        return *this;
    }
};

} // namespace System
