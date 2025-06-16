#pragma once
/*
 * Char.h
 * C++ implementation of .NET's System.Char struct
 * 
 * Key .NET Char characteristics implemented:
 * - Value type semantics (pass by value)
 * - Core operations and conversions
 * - Static constants (MaxValue, MinValue)
 * - Parse and TryParse methods
 * - Character classification methods
 */
#include <string>
#include <limits>
#include <stdexcept>
#include <cctype>
#include <optional>
#include "String.h"
#include "Object.h"

namespace System {

class Char {
private:
    char m_value;

public:
    // Constants
    static constexpr char MaxValue = std::numeric_limits<char>::max();
    static constexpr char MinValue = std::numeric_limits<char>::min();

    // Constructors
    constexpr Char() noexcept : m_value('\0') {}
    constexpr explicit Char(char value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Char(const Char& other) noexcept = default;
    
    // Assignment operator
    constexpr Char& operator=(const Char& other) noexcept = default;
    
    // Conversion operator to char (implicit in C#)
    [[nodiscard]] constexpr operator char() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Char.Value)
    [[nodiscard]] constexpr char Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const Char& other) const noexcept {
        if (m_value < other.m_value) return -1;
        if (m_value > other.m_value) return 1;
        return 0;
    }
    
    [[nodiscard]] constexpr bool Equals(const Char& other) const noexcept {
        return m_value == other.m_value;
    }
    
    [[nodiscard]] constexpr int GetHashCode() const noexcept {
        return static_cast<size_t>(m_value);
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        return String(std::string(1, m_value).c_str());
    }
    
    // Static methods
    
    // Parse method (equivalent to .NET Char.Parse)
    [[nodiscard]] static Char Parse(const String& s) {
        if (s.Length() != 1) {
            throw std::invalid_argument("String must be exactly one character long.");
        }
        return Char(s[0]);
    }
    
    // TryParse method (equivalent to .NET Char.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Char& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Char('\0');
            return false;
        }
    }
    
    // Character classification methods (equivalent to .NET Char methods)
    [[nodiscard]] static bool IsDigit(char c) noexcept {
        return std::isdigit(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static bool IsLetter(char c) noexcept {
        return std::isalpha(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static bool IsWhiteSpace(char c) noexcept {
        return std::isspace(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static bool IsUpper(char c) noexcept {
        return std::isupper(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static bool IsLower(char c) noexcept {
        return std::islower(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static bool IsLetterOrDigit(char c) noexcept {
        return std::isalnum(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static bool IsPunctuation(char c) noexcept {
        return std::ispunct(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static bool IsControl(char c) noexcept {
        return std::iscntrl(static_cast<unsigned char>(c)) != 0;
    }
    
    [[nodiscard]] static char ToUpper(char c) noexcept {
        return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    
    [[nodiscard]] static char ToLower(char c) noexcept {
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    
    // Numeric conversion methods
    [[nodiscard]] static int GetNumericValue(char c) noexcept {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        return -1;  // Not a numeric character
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const Char& left, const Char& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const Char& left, const Char& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Comparison operators
    [[nodiscard]] friend constexpr bool operator<(const Char& left, const Char& right) noexcept {
        return left.m_value < right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator<=(const Char& left, const Char& right) noexcept {
        return left.m_value <= right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>(const Char& left, const Char& right) noexcept {
        return left.m_value > right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator>=(const Char& left, const Char& right) noexcept {
        return left.m_value >= right.m_value;
    }
    
    // Increment/decrement operators
    constexpr Char& operator++() noexcept {
        ++m_value;
        return *this;
    }
    
    constexpr Char operator++(int) noexcept {
        Char temp(*this);
        ++m_value;
        return temp;
    }
    
    constexpr Char& operator--() noexcept {
        --m_value;
        return *this;
    }
    
    constexpr Char operator--(int) noexcept {
        Char temp(*this);
        --m_value;
        return temp;
    }
};

} // namespace System
