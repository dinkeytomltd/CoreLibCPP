
#pragma once
/*
 * UInt16.h
 * C++ implementation of .NET's System.UInt16 struct (unsigned short)
 * 
 * Key .NET UInt16 characteristics implemented:
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

class UInt16 {
private:
    unsigned short m_value;

public:
    // Constants
    static constexpr unsigned short MaxValue = std::numeric_limits<unsigned short>::max();
    static constexpr unsigned short MinValue = std::numeric_limits<unsigned short>::min();

    // Constructors
    constexpr UInt16() noexcept : m_value(0) {}
    constexpr explicit UInt16(unsigned short value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr UInt16(const UInt16& other) noexcept = default;
    
    // Assignment operator
    constexpr UInt16& operator=(const UInt16& other) noexcept = default;
    
    // Conversion operator to unsigned short (implicit in C#)
    [[nodiscard]] constexpr operator unsigned short() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET UInt16.Value)
    [[nodiscard]] constexpr unsigned short Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const UInt16& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const UInt16& other) const noexcept {
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
    
    // Parse method (equivalent to .NET UInt16.Parse)
    [[nodiscard]] static UInt16 Parse(const String& s) {
        try {
            unsigned long result = std::stoul(s.ToStdString());
            if (result > MaxValue) {
                throw std::out_of_range("Value was either too large or too small for a UInt16.");
            }
            return UInt16(static_cast<unsigned short>(result));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a UInt16.");
        }
    }
    
    // TryParse method (equivalent to .NET UInt16.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, UInt16& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = UInt16(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const UInt16& left, const UInt16& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const UInt16& left, const UInt16& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const UInt16& left, const UInt16& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const UInt16& left, const UInt16& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const UInt16& left, const UInt16& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const UInt16& left, const UInt16& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr UInt16 operator+(const UInt16& left, const UInt16& right) noexcept {
        return UInt16(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt16 operator-(const UInt16& left, const UInt16& right) noexcept {
        return UInt16(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt16 operator*(const UInt16& left, const UInt16& right) noexcept {
        return UInt16(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt16 operator/(const UInt16& left, const UInt16& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return UInt16(left.m_value / right.m_value);
    }
    
    [[nodiscard]] friend constexpr UInt16 operator%(const UInt16& left, const UInt16& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return UInt16(left.m_value % right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] constexpr UInt16 operator+() const noexcept {
        return *this;
    }
    
    // Increment/decrement operators
    constexpr UInt16& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr UInt16 operator++(int) noexcept {
        UInt16 temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr UInt16& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr UInt16 operator--(int) noexcept {
        UInt16 temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr UInt16& operator+=(const UInt16& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    constexpr UInt16& operator-=(const UInt16& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    constexpr UInt16& operator*=(const UInt16& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    constexpr UInt16& operator/=(const UInt16& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value /= other.m_value;
        return *this;
    }
    
    constexpr UInt16& operator%=(const UInt16& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value %= other.m_value;
        return *this;
    }
};

} // namespace System
