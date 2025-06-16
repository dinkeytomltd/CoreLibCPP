#pragma once
/*
 * Byte.h
 * C++ implementation of .NET's System.Byte struct
 * 
 * Key .NET Byte characteristics implemented:
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

class Byte {
private:
    unsigned char m_value;

public:
    // Constants
    static constexpr unsigned char MaxValue = std::numeric_limits<unsigned char>::max();
    static constexpr unsigned char MinValue = std::numeric_limits<unsigned char>::min();

    // Constructors
    constexpr Byte() noexcept : m_value(0) {}
    constexpr explicit Byte(unsigned char value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Byte(const Byte& other) noexcept = default;
    
    // Assignment operator
    constexpr Byte& operator=(const Byte& other) noexcept = default;
    
    // Conversion operator to unsigned char (implicit in C#)
    [[nodiscard]] constexpr operator unsigned char() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Byte.Value)
    [[nodiscard]] constexpr unsigned char Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const Byte& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const Byte& other) const noexcept {
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
    
    // Parse method (equivalent to .NET Byte.Parse)
    [[nodiscard]] static Byte Parse(const String& s) {
        try {
            unsigned long value = std::stoul(s.ToStdString());
            if (value > MaxValue) {
                throw std::out_of_range("Value was either too large or too small for a Byte.");
            }
            return Byte(static_cast<unsigned char>(value));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a Byte.");
        }
    }
    
    // TryParse method (equivalent to .NET Byte.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Byte& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Byte(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const Byte& left, const Byte& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const Byte& left, const Byte& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const Byte& left, const Byte& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const Byte& left, const Byte& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const Byte& left, const Byte& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const Byte& left, const Byte& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr Byte operator+(const Byte& left, const Byte& right) noexcept {
        return Byte(static_cast<unsigned char>(left.m_value + right.m_value));
    }
    
    [[nodiscard]] friend constexpr Byte operator-(const Byte& left, const Byte& right) noexcept {
        return Byte(static_cast<unsigned char>(left.m_value - right.m_value));
    }
    
    [[nodiscard]] friend constexpr Byte operator*(const Byte& left, const Byte& right) noexcept {
        return Byte(static_cast<unsigned char>(left.m_value * right.m_value));
    }
    
    [[nodiscard]] friend constexpr Byte operator/(const Byte& left, const Byte& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Byte(static_cast<unsigned char>(left.m_value / right.m_value));
    }
    
    [[nodiscard]] friend constexpr Byte operator%(const Byte& left, const Byte& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Byte(static_cast<unsigned char>(left.m_value % right.m_value));
    }
    
    // Unary operators
    [[nodiscard]] constexpr Byte operator+() const noexcept {
        return *this;
    }
    
    // Increment/decrement operators
    constexpr Byte& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr Byte operator++(int) noexcept {
        Byte temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr Byte& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr Byte operator--(int) noexcept {
        Byte temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr Byte& operator+=(const Byte& other) noexcept {
        m_value = static_cast<unsigned char>(m_value + other.m_value);
        return *this;
    }
    
    constexpr Byte& operator-=(const Byte& other) noexcept {
        m_value = static_cast<unsigned char>(m_value - other.m_value);
        return *this;
    }
    
    constexpr Byte& operator*=(const Byte& other) noexcept {
        m_value = static_cast<unsigned char>(m_value * other.m_value);
        return *this;
    }
    
    constexpr Byte& operator/=(const Byte& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value = static_cast<unsigned char>(m_value / other.m_value);
        return *this;
    }
    
    constexpr Byte& operator%=(const Byte& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value = static_cast<unsigned char>(m_value % other.m_value);
        return *this;
    }
    
    // Bitwise operators
    [[nodiscard]] friend constexpr Byte operator&(const Byte& left, const Byte& right) noexcept {
        return Byte(static_cast<unsigned char>(left.m_value & right.m_value));
    }
    
    [[nodiscard]] friend constexpr Byte operator|(const Byte& left, const Byte& right) noexcept {
        return Byte(static_cast<unsigned char>(left.m_value | right.m_value));
    }
    
    [[nodiscard]] friend constexpr Byte operator^(const Byte& left, const Byte& right) noexcept {
        return Byte(static_cast<unsigned char>(left.m_value ^ right.m_value));
    }
    
    [[nodiscard]] constexpr Byte operator~() const noexcept {
        return Byte(static_cast<unsigned char>(~m_value));
    }
    
    // Bitwise compound assignment operators
    constexpr Byte& operator&=(const Byte& other) noexcept {
        m_value &= other.m_value;
        return *this;
    }
    
    constexpr Byte& operator|=(const Byte& other) noexcept {
        m_value |= other.m_value;
        return *this;
    }
    
    constexpr Byte& operator^=(const Byte& other) noexcept {
        m_value ^= other.m_value;
        return *this;
    }
};

} // namespace System
