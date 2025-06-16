
#pragma once
/*
 * Guid.h
 * C++ implementation of .NET's System.Guid struct
 * 
 * Key .NET Guid characteristics implemented:
 * - Represents a globally unique identifier (GUID), a 128-bit value
 * - Provides static method NewGuid() to generate new random GUIDs
 * - Supports standard GUID string formats (D, N, B, P)
 * - Includes Parse() and TryParse() methods for string conversion
 * - Supports comparison operations and Empty static property
 */
#include <array>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace System {

class Guid {
private:
    // 128-bit GUID stored as 16 bytes
    std::array<uint8_t, 16> m_data;

    // Helper method to convert hex character to value
    static uint8_t HexCharToValue(char c);
    
    // Helper method to format bytes as hex string
    static std::string BytesToHexString(const std::array<uint8_t, 16>& bytes, bool uppercase = false);

public:
    // Constructors
    Guid() : m_data{} {} // Default constructor creates empty GUID (all zeros)
    
    explicit Guid(const std::array<uint8_t, 16>& bytes) : m_data(bytes) {}
    
    Guid(uint32_t a, uint16_t b, uint16_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h, uint8_t i, uint8_t j, uint8_t k);
    
    // Copy constructor and assignment operator (default implementations are fine)
    Guid(const Guid& other) = default;
    Guid& operator=(const Guid& other) = default;

    // Static properties
    [[nodiscard]] static Guid Empty() {
        return Guid(); // All zeros
    }

    // Static methods
    [[nodiscard]] static Guid NewGuid();
    [[nodiscard]] static Guid Parse(const std::string& input);
    [[nodiscard]] static bool TryParse(const std::string& input, Guid& result);

    // Instance methods
    [[nodiscard]] bool Equals(const Guid& other) const {
        return m_data == other.m_data;
    }
    
    [[nodiscard]] int CompareTo(const Guid& other) const;
    
    [[nodiscard]] std::string ToString() const {
        return ToString("D");
    }
    
    [[nodiscard]] std::string ToString(const std::string& format) const;
    
    // Get raw byte array
    [[nodiscard]] const std::array<uint8_t, 16>& ToByteArray() const {
        return m_data;
    }

    // Operators
    friend bool operator==(const Guid& left, const Guid& right) {
        return left.Equals(right);
    }
    
    friend bool operator!=(const Guid& left, const Guid& right) {
        return !left.Equals(right);
    }
    
    friend bool operator<(const Guid& left, const Guid& right) {
        return left.CompareTo(right) < 0;
    }
    
    friend bool operator>(const Guid& left, const Guid& right) {
        return left.CompareTo(right) > 0;
    }
    
    friend bool operator<=(const Guid& left, const Guid& right) {
        return left.CompareTo(right) <= 0;
    }
    
    friend bool operator>=(const Guid& left, const Guid& right) {
        return left.CompareTo(right) >= 0;
    }
};

} // namespace System
