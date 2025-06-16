
#pragma once
/*
 * Single.h
 * C++ implementation of .NET's System.Single struct (float)
 * 
 * Key .NET Single characteristics implemented:
 * - Value type semantics (pass by value)
 * - Core operations and conversions
 * - Static constants (MaxValue, MinValue, etc.)
 * - Parse and TryParse methods
 */
#include <string>
#include <limits>
#include <stdexcept>
#include <charconv>
#include <cmath>
#include "String.h"
#include "Object.h"

namespace System {

class Single {
private:
    float m_value;

public:
    // Constants
    static constexpr float MaxValue = std::numeric_limits<float>::max();
    static constexpr float MinValue = std::numeric_limits<float>::lowest();
    static constexpr float Epsilon = std::numeric_limits<float>::epsilon();
    static constexpr float PositiveInfinity = std::numeric_limits<float>::infinity();
    static constexpr float NegativeInfinity = -std::numeric_limits<float>::infinity();
    static constexpr float NaN = std::numeric_limits<float>::quiet_NaN();

    // Constructors
    constexpr Single() noexcept : m_value(0.0f) {}
    constexpr explicit Single(float value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Single(const Single& other) noexcept = default;
    
    // Assignment operator
    constexpr Single& operator=(const Single& other) noexcept = default;
    
    // Conversion operator to float (implicit in C#)
    [[nodiscard]] constexpr operator float() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Single.Value)
    [[nodiscard]] constexpr float Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const Single& other) const noexcept {
        if (std::isnan(m_value) || std::isnan(other.m_value)) {
            if (std::isnan(m_value) && std::isnan(other.m_value)) return 0;
            if (std::isnan(m_value)) return -1;
            return 1;
        }
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const Single& other) const noexcept {
        if (std::isnan(m_value) && std::isnan(other.m_value)) return true;
        return m_value == other.m_value;
    }
    
    [[nodiscard]] int GetHashCode() const noexcept {
        return std::hash<float>{}(m_value);
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        if (std::isnan(m_value)) return String("NaN");
        if (std::isinf(m_value)) return m_value > 0 ? String("Infinity") : String("-Infinity");
        return String(std::to_string(m_value).c_str());
    }
    
    // Static methods
    
    // Parse method (equivalent to .NET Single.Parse)
    [[nodiscard]] static Single Parse(const String& s) {
        try {
            return Single(std::stof(s.ToStdString()));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a Single.");
        }
    }
    
    // TryParse method (equivalent to .NET Single.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Single& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Single(0.0f);
            return false;
        }
    }
    
    // Static utility methods
    [[nodiscard]] static constexpr bool IsNaN(const Single& value) noexcept {
        return std::isnan(value.m_value);
    }
    
    [[nodiscard]] static constexpr bool IsInfinity(const Single& value) noexcept {
        return std::isinf(value.m_value);
    }
    
    [[nodiscard]] static constexpr bool IsPositiveInfinity(const Single& value) noexcept {
        return std::isinf(value.m_value) && value.m_value > 0;
    }
    
    [[nodiscard]] static constexpr bool IsNegativeInfinity(const Single& value) noexcept {
        return std::isinf(value.m_value) && value.m_value < 0;
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const Single& left, const Single& right) noexcept {
        return left.Equals(right);
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const Single& left, const Single& right) noexcept {
        return !left.Equals(right);
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const Single& left, const Single& right) noexcept {
        return left.CompareTo(right) < 0;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const Single& left, const Single& right) noexcept {
        return left.CompareTo(right) <= 0;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const Single& left, const Single& right) noexcept {
        return left.CompareTo(right) > 0;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const Single& left, const Single& right) noexcept {
        return left.CompareTo(right) >= 0;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend constexpr Single operator+(const Single& left, const Single& right) noexcept {
        return Single(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend constexpr Single operator-(const Single& left, const Single& right) noexcept {
        return Single(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend constexpr Single operator*(const Single& left, const Single& right) noexcept {
        return Single(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend constexpr Single operator/(const Single& left, const Single& right) noexcept {
        return Single(left.m_value / right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] constexpr Single operator+() const noexcept {
        return *this;
    }
    
    [[nodiscard]] constexpr Single operator-() const noexcept {
        return Single(-m_value);
    }
    
    // Compound assignment operators
    constexpr Single& operator+=(const Single& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    constexpr Single& operator-=(const Single& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    constexpr Single& operator*=(const Single& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    constexpr Single& operator/=(const Single& other) noexcept {
        m_value /= other.m_value;
        return *this;
    }
};

} // namespace System
