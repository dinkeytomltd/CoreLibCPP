
#pragma once
/*
 * SByte.h
 * C++ implementation of .NET's System.SByte struct (signed char)
 * 
 * Key .NET SByte characteristics implemented:
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

class SByte {
private:
    signed char m_value;

public:
    // Constants
    static constexpr signed char MaxValue = std::numeric_limits<signed char>::max();
    static constexpr signed char MinValue = std::numeric_limits<signed char>::min();

    // Constructors
    constexpr SByte() noexcept : m_value(0) {}
    constexpr explicit SByte(signed char value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr SByte(const SByte& other) noexcept = default;
    
    // Assignment operator
    constexpr SByte& operator=(const SByte& other) noexcept = default;
    
    // Conversion operator to signed char (implicit in C#)
    [[nodiscard]] constexpr operator signed char() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET SByte.Value)
    [[nodiscard]] constexpr signed char Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const SByte& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const SByte& other) const noexcept {
        return m_value == other.m_value;
    }
    
    [[nodiscard]] constexpr int GetHashCode() const noexcept {
        return static_cast<size_t>(m_value);
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        return String(std::to_string(static_cast<int>(m_value)).c_str());
    }
    
    // Static methods
    
    // Parse method (equivalent to .NET SByte.Parse)
    [[nodiscard]] static SByte Parse(const String& s) {
        try {
            int result = std::stoi(s.ToStdString());
            if (result < MinValue || result > MaxValue) {
                throw std::out_of_range("Value was either too large or too small for a SByte.");
            }
            return SByte(static_cast<signed char>(result));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a SByte.");
        }
    }
    
    // TryParse method (equivalent to .NET SByte.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, SByte& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = SByte(0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const SByte& left, const SByte& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const SByte& left, const SByte& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const SByte& left, const SByte& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const SByte& left, const SByte& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const SByte& left, const SByte& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const SByte& left, const SByte& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr SByte operator+(const SByte& left, const SByte& right) noexcept {
        return SByte(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend constexpr SByte operator-(const SByte& left, const SByte& right) noexcept {
        return SByte(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend constexpr SByte operator*(const SByte& left, const SByte& right) noexcept {
        return SByte(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend constexpr SByte operator/(const SByte& left, const SByte& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return SByte(left.m_value / right.m_value);
    }
    
    [[nodiscard]] friend constexpr SByte operator%(const SByte& left, const SByte& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return SByte(left.m_value % right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] constexpr SByte operator+() const noexcept {
        return *this;
    }
    
    [[nodiscard]] constexpr SByte operator-() const noexcept {
        return SByte(-m_value);
    }
    
    // Increment/decrement operators
    constexpr SByte& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr SByte operator++(int) noexcept {
        SByte temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr SByte& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr SByte operator--(int) noexcept {
        SByte temp(*this);
        --m_value;
        return temp;
    }
    
    // Compound assignment operators
    constexpr SByte& operator+=(const SByte& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    constexpr SByte& operator-=(const SByte& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    constexpr SByte& operator*=(const SByte& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    constexpr SByte& operator/=(const SByte& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value /= other.m_value;
        return *this;
    }
    
    constexpr SByte& operator%=(const SByte& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value %= other.m_value;
        return *this;
    }
};

} // namespace System
