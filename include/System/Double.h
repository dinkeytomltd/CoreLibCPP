#pragma once
/*
 * Double.h
 * C++ implementation of .NET's System.Double struct
 * 
 * Key .NET Double characteristics implemented:
 * - Value type semantics (pass by value)
 * - Core operations and conversions
 * - Static constants (MaxValue, MinValue, Epsilon, NaN, etc.)
 * - Parse and TryParse methods
 */
#include <string>
#include <limits>
#include <cmath>
#include <stdexcept>
#include "String.h"
#include "Object.h"

namespace System {

class Double {
private:
    double m_value;

public:
    // Constants
    static constexpr double MaxValue = std::numeric_limits<double>::max();
    static constexpr double MinValue = std::numeric_limits<double>::lowest();
    static constexpr double Epsilon = std::numeric_limits<double>::epsilon();
    static constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
    static constexpr double PositiveInfinity = std::numeric_limits<double>::infinity();
    static constexpr double NegativeInfinity = -std::numeric_limits<double>::infinity();

    // Constructors
    constexpr Double() noexcept : m_value(0.0) {}
    constexpr explicit Double(double value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Double(const Double& other) noexcept = default;
    
    // Assignment operator
    constexpr Double& operator=(const Double& other) noexcept = default;
    
    // Conversion operator to double (implicit in C#)
    [[nodiscard]] constexpr operator double() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Double.Value)
    [[nodiscard]] constexpr double Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] int CompareTo(const Double& other) const noexcept {
        // Handle special cases like NaN according to .NET behavior
        if (std::isnan(m_value)) {
            return std::isnan(other.m_value) ? 0 : -1;
        }
        if (std::isnan(other.m_value)) {
            return 1;
        }
        
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] bool Equals(const Double& other) const noexcept {
        // Handle special cases like NaN according to .NET behavior
        if (std::isnan(m_value)) {
            return std::isnan(other.m_value);
        }
        return m_value == other.m_value;
    }
    
    [[nodiscard]] int GetHashCode() const noexcept {
        // Simple hash code implementation
        return std::hash<double>{}(m_value);
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        if (std::isnan(m_value)) {
            return String("NaN");
        }
        if (m_value == PositiveInfinity) {
            return String("Infinity");
        }
        if (m_value == NegativeInfinity) {
            return String("-Infinity");
        }
        return String(std::to_string(m_value).c_str());
    }
    
    // Static methods
    
    // IsNaN method (equivalent to .NET Double.IsNaN)
    [[nodiscard]] static constexpr bool IsNaN(double d) noexcept {
        return std::isnan(d);
    }
    
    // IsInfinity method (equivalent to .NET Double.IsInfinity)
    [[nodiscard]] static constexpr bool IsInfinity(double d) noexcept {
        return std::isinf(d);
    }
    
    // IsPositiveInfinity method (equivalent to .NET Double.IsPositiveInfinity)
    [[nodiscard]] static constexpr bool IsPositiveInfinity(double d) noexcept {
        return d == PositiveInfinity;
    }
    
    // IsNegativeInfinity method (equivalent to .NET Double.IsNegativeInfinity)
    [[nodiscard]] static constexpr bool IsNegativeInfinity(double d) noexcept {
        return d == NegativeInfinity;
    }
    
    // Parse method (equivalent to .NET Double.Parse)
    [[nodiscard]] static Double Parse(const String& s) {
        try {
            std::string str = s.ToStdString();
            
            // Handle special strings
            if (str == "NaN") return Double(NaN);
            if (str == "Infinity") return Double(PositiveInfinity);
            if (str == "-Infinity") return Double(NegativeInfinity);
            
            return Double(std::stod(str));
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Value was either too large or too small for a Double.");
        }
    }
    
    // TryParse method (equivalent to .NET Double.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Double& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Double(0.0);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend bool operator==(const Double& left, const Double& right) noexcept {
        // Handle NaN according to IEEE 754
        if (std::isnan(left.m_value) || std::isnan(right.m_value)) {
            return false;
        }
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend bool operator!=(const Double& left, const Double& right) noexcept {
        return !(left == right);
    }
    
    // Comparison operators
    [[nodiscard]] friend bool operator<(const Double& left, const Double& right) noexcept {
        // Handle NaN according to IEEE 754
        if (std::isnan(left.m_value) || std::isnan(right.m_value)) {
            return false;
        }
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend bool operator<=(const Double& left, const Double& right) noexcept {
        // Handle NaN according to IEEE 754
        if (std::isnan(left.m_value) || std::isnan(right.m_value)) {
            return false;
        }
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend bool operator>(const Double& left, const Double& right) noexcept {
        // Handle NaN according to IEEE 754
        if (std::isnan(left.m_value) || std::isnan(right.m_value)) {
            return false;
        }
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend bool operator>=(const Double& left, const Double& right) noexcept {
        // Handle NaN according to IEEE 754
        if (std::isnan(left.m_value) || std::isnan(right.m_value)) {
            return false;
        }
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend Double operator+(const Double& left, const Double& right) noexcept {
        return Double(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend Double operator-(const Double& left, const Double& right) noexcept {
        return Double(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend Double operator*(const Double& left, const Double& right) noexcept {
        return Double(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend Double operator/(const Double& left, const Double& right) noexcept {
        return Double(left.m_value / right.m_value);
    }
    
    // Unary operators
    [[nodiscard]] Double operator+() const noexcept {
        return *this;
    }
    
    [[nodiscard]] Double operator-() const noexcept {
        return Double(-m_value);
    }
    
    // Compound assignment operators
    Double& operator+=(const Double& other) noexcept {
        m_value += other.m_value;
        return *this;
    }
    
    Double& operator-=(const Double& other) noexcept {
        m_value -= other.m_value;
        return *this;
    }
    
    Double& operator*=(const Double& other) noexcept {
        m_value *= other.m_value;
        return *this;
    }
    
    Double& operator/=(const Double& other) noexcept {
        m_value /= other.m_value;
        return *this;
    }
};

} // namespace System
