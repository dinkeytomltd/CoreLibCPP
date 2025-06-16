#pragma once
/*
 * Boolean.h
 * C++ implementation of .NET's System.Boolean struct
 * 
 * Key .NET Boolean characteristics implemented:
 * - Value type semantics (pass by value)
 * - Core operations and conversions
 * - Parse and TryParse methods
 * - Static constants (TrueString, FalseString)
 */
#include <string>
#include <algorithm>
#include <cctype>
#include "String.h"
#include "Object.h"

namespace System {

class Boolean {
private:
    bool m_value;

public:
    // Constants
    static const String TrueString;
    static const String FalseString;

    // Constructors
    constexpr Boolean() noexcept : m_value(false) {}
    constexpr explicit Boolean(bool value) noexcept : m_value(value) {}
    
    // Copy constructor
    constexpr Boolean(const Boolean& other) noexcept = default;
    
    // Assignment operator
    constexpr Boolean& operator=(const Boolean& other) noexcept = default;
    
    // Conversion operator to bool (implicit in C#)
    [[nodiscard]] constexpr operator bool() const noexcept { return m_value; }
    
    // Value property (equivalent to .NET Boolean.Value)
    [[nodiscard]] constexpr bool Value() const noexcept { return m_value; }
    
    // Comparison methods
    [[nodiscard]] constexpr int CompareTo(const Boolean& other) const noexcept {
        if (m_value == other.m_value) return 0;
        return m_value ? 1 : -1;
    }
    
    [[nodiscard]] constexpr bool Equals(const Boolean& other) const noexcept {
        return m_value == other.m_value;
    }
    
    [[nodiscard]] constexpr int GetHashCode() const noexcept {
        return m_value ? 1 : 0;
    }
    
    // ToString method
    [[nodiscard]] String ToString() const {
        return m_value ? TrueString : FalseString;
    }
    
    // Static methods
    
    // Parse method (equivalent to .NET Boolean.Parse)
    [[nodiscard]] static Boolean Parse(const String& s) {
        String value = s.Trim().ToLower();
        
        if (value.Equals(TrueString.ToLower())) {
            return Boolean(true);
        }
        else if (value.Equals(FalseString.ToLower())) {
            return Boolean(false);
        }
        
        throw std::invalid_argument("String was not recognized as a valid Boolean.");
    }
    
    // TryParse method (equivalent to .NET Boolean.TryParse)
    [[nodiscard]] static bool TryParse(const String& s, Boolean& result) {
        try {
            result = Parse(s);
            return true;
        } catch (...) {
            result = Boolean(false);
            return false;
        }
    }
    
    // Operators
    
    // Equality operators
    [[nodiscard]] friend constexpr bool operator==(const Boolean& left, const Boolean& right) noexcept {
        return left.m_value == right.m_value;
    }
    
    [[nodiscard]] friend constexpr bool operator!=(const Boolean& left, const Boolean& right) noexcept {
        return left.m_value != right.m_value;
    }
    
    // Logical operators
    [[nodiscard]] friend constexpr Boolean operator!(const Boolean& value) noexcept {
        return Boolean(!value.m_value);
    }
    
    [[nodiscard]] friend constexpr Boolean operator&&(const Boolean& left, const Boolean& right) noexcept {
        return Boolean(left.m_value && right.m_value);
    }
    
    [[nodiscard]] friend constexpr Boolean operator||(const Boolean& left, const Boolean& right) noexcept {
        return Boolean(left.m_value || right.m_value);
    }
};

// Initialize static constants
inline const String Boolean::TrueString = String("True");
inline const String Boolean::FalseString = String("False");

} // namespace System
