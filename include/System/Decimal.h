
#pragma once
/*
 * Decimal.h
 * C++ implementation of .NET's System.Decimal struct
 * 
 * Key .NET Decimal characteristics implemented:
 * - High-precision decimal arithmetic (128-bit equivalent)
 * - Value type semantics (pass by value)
 * - Core operations and conversions
 * - Static constants (MaxValue, MinValue, etc.)
 * - Parse and TryParse methods
 * 
 * Implementation uses boost::multiprecision::cpp_dec_float for high precision
 */
#include <string>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "String.h"
#include "Object.h"

namespace System {

class Decimal {
private:
    // Using 50 decimal digits for high precision (equivalent to .NET's 128-bit decimal)
    using decimal_type = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<50>>;
    decimal_type m_value;

public:
    // Constants (approximating .NET Decimal limits)
    static const Decimal MaxValue;
    static const Decimal MinValue;
    static const Decimal Zero;
    static const Decimal One;
    static const Decimal MinusOne;

    // Constructors
    constexpr Decimal() noexcept : m_value(0) {}
    
    explicit Decimal(int value) : m_value(value) {}
    explicit Decimal(unsigned int value) : m_value(value) {}
    explicit Decimal(long value) : m_value(value) {}
    explicit Decimal(unsigned long value) : m_value(value) {}
    explicit Decimal(long long value) : m_value(value) {}
    explicit Decimal(unsigned long long value) : m_value(value) {}
    explicit Decimal(float value) : m_value(value) {}
    explicit Decimal(double value) : m_value(value) {}
    explicit Decimal(const std::string& value) : m_value(value) {}
    explicit Decimal(const decimal_type& value) : m_value(value) {}
    
    // Copy constructor
    Decimal(const Decimal& other) = default;
    
    // Assignment operator
    Decimal& operator=(const Decimal& other) = default;
    
    // Conversion operators
    [[nodiscard]] explicit operator int() const { 
        return static_cast<int>(m_value); 
    }
    
    [[nodiscard]] explicit operator unsigned int() const { 
        return static_cast<unsigned int>(m_value); 
    }
    
    [[nodiscard]] explicit operator long() const { 
        return static_cast<long>(m_value); 
    }
    
    [[nodiscard]] explicit operator unsigned long() const { 
        return static_cast<unsigned long>(m_value); 
    }
    
    [[nodiscard]] explicit operator long long() const { 
        return static_cast<long long>(m_value); 
    }
    
    [[nodiscard]] explicit operator unsigned long long() const { 
        return static_cast<unsigned long long>(m_value); 
    }
    
    [[nodiscard]] explicit operator float() const { 
        return static_cast<float>(m_value); 
    }
    
    [[nodiscard]] explicit operator double() const { 
        return static_cast<double>(m_value); 
    }
    
    // Value property
    [[nodiscard]] const decimal_type& Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] int CompareTo(const Decimal& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] bool Equals(const Decimal& other) const noexcept {
        return m_value == other.m_value;
    }
    
    [[nodiscard]] int GetHashCode() const {
        return std::hash<std::string>{}(m_value.str());
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(28) << m_value; // .NET Decimal has 28-29 significant digits
        std::string str = oss.str();
        
        // Remove trailing zeros after decimal point
        if (str.find('.') != std::string::npos) {
            str = str.substr(0, str.find_last_not_of('0') + 1);
            if (str.back() == '.') {
                str.pop_back();
            }
        }
        
        return String(str.c_str());
    }
    
    // Static methods
    
    // Parse method (equivalent to .NET Decimal.Parse)
    [[nodiscard]] static Decimal Parse(const String& s) {
        try {
            return Decimal(decimal_type(s.ToStdString()));
        } catch (const std::exception&) {
            throw std::invalid_argument("Input string was not in a correct format.");
        }
    }
    
    // TryParse method (equivalent to .NET Decimal.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Decimal& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Decimal();
            return false;
        }
    }
    
    // Static utility methods
    [[nodiscard]] static Decimal Abs(const Decimal& value) {
        return Decimal(boost::multiprecision::abs(value.m_value));
    }
    
    [[nodiscard]] static Decimal Floor(const Decimal& value) {
        return Decimal(boost::multiprecision::floor(value.m_value));
    }
    
    [[nodiscard]] static Decimal Ceiling(const Decimal& value) {
        return Decimal(boost::multiprecision::ceil(value.m_value));
    }
    
    [[nodiscard]] static Decimal Round(const Decimal& value) {
        return Decimal(boost::multiprecision::round(value.m_value));
    }
    
    [[nodiscard]] static Decimal Truncate(const Decimal& value) {
        return Decimal(boost::multiprecision::trunc(value.m_value));
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend bool operator==(const Decimal& left, const Decimal& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend bool operator!=(const Decimal& left, const Decimal& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend bool operator<(const Decimal& left, const Decimal& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend bool operator<=(const Decimal& left, const Decimal& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend bool operator>(const Decimal& left, const Decimal& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend bool operator>=(const Decimal& left, const Decimal& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Arithmetic operators
    [[nodiscard]] friend Decimal operator+(const Decimal& left, const Decimal& right) {
        return Decimal(left.m_value + right.m_value);
    }
    
    [[nodiscard]] friend Decimal operator-(const Decimal& left, const Decimal& right) {
        return Decimal(left.m_value - right.m_value);
    }
    
    [[nodiscard]] friend Decimal operator*(const Decimal& left, const Decimal& right) {
        return Decimal(left.m_value * right.m_value);
    }
    
    [[nodiscard]] friend Decimal operator/(const Decimal& left, const Decimal& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Decimal(left.m_value / right.m_value);
    }
    
    [[nodiscard]] friend Decimal operator%(const Decimal& left, const Decimal& right) {
        if (right.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return Decimal(boost::multiprecision::fmod(left.m_value, right.m_value));
    }
    
    // Unary operators
    [[nodiscard]] Decimal operator+() const {
        return *this;
    }
    
    [[nodiscard]] Decimal operator-() const {
        return Decimal(-m_value);
    }
    
    // Increment/decrement operators
    Decimal& operator++() {
        m_value += 1;
        return *this;
    }
    
    Decimal operator++(int) {
        Decimal temp(*this);
        m_value += 1;
        return temp;
    }
    
    Decimal& operator--() {
        m_value -= 1;
        return *this;
    }
    
    Decimal operator--(int) {
        Decimal temp(*this);
        m_value -= 1;
        return temp;
    }
    
    // Compound assignment operators
    Decimal& operator+=(const Decimal& other) {
        m_value += other.m_value;
        return *this;
    }
    
    Decimal& operator-=(const Decimal& other) {
        m_value -= other.m_value;
        return *this;
    }
    
    Decimal& operator*=(const Decimal& other) {
        m_value *= other.m_value;
        return *this;
    }
    
    Decimal& operator/=(const Decimal& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value /= other.m_value;
        return *this;
    }
    
    Decimal& operator%=(const Decimal& other) {
        if (other.m_value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        m_value = boost::multiprecision::fmod(m_value, other.m_value);
        return *this;
    }
};

} // namespace System
