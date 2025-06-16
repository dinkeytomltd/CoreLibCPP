
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <rttr/registration>
#include "System/String.h"
#include "System/GC/GcPtr.hpp"

namespace System {
namespace Text {

/**
 * @brief High-performance mutable string builder optimized for .NET compatibility
 * 
 * This implementation focuses on addressing the 2-5x performance gap with C# StringBuilder
 * by using optimized memory management, SIMD operations where possible, and efficient
 * capacity growth strategies.
 */
class StringBuilder {
private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr size_t MAX_CAPACITY_DEFAULT = 0x7FFFFFFF;
    
    // Internal buffer management
    std::vector<char16_t> m_buffer;
    size_t m_length;
    size_t m_capacity;
    size_t m_maxCapacity;
    
    // Performance optimization: small string optimization
    static constexpr size_t SSO_SIZE = 32;
    char16_t m_ssoBuffer[SSO_SIZE];
    bool m_usingSSOBuffer;
    
    // Internal methods
    void EnsureCapacityInternal(size_t requiredCapacity);
    void ExpandCapacity(size_t minimumCapacity);
    char16_t* GetBuffer();
    const char16_t* GetBuffer() const;
    void CopyToBuffer(const char16_t* source, size_t sourceLength, size_t destIndex);
    
public:
    // Constructors
    StringBuilder();
    explicit StringBuilder(int capacity);
    StringBuilder(int capacity, int maxCapacity);
    explicit StringBuilder(const std::string& value);
    StringBuilder(const std::string& value, int capacity);
    StringBuilder(const std::string& value, int startIndex, int length, int capacity);
    StringBuilder(const String& value);
    StringBuilder(const String& value, int capacity);
    
    // Copy constructor and assignment
    StringBuilder(const StringBuilder& other);
    StringBuilder& operator=(const StringBuilder& other);
    
    // Move constructor and assignment
    StringBuilder(StringBuilder&& other) noexcept;
    StringBuilder& operator=(StringBuilder&& other) noexcept;
    
    // Destructor
    ~StringBuilder() = default;
    
    // Properties
    int GetCapacity() const { return static_cast<int>(m_capacity); }
    void SetCapacity(int value);
    
    int GetLength() const { return static_cast<int>(m_length); }
    void SetLength(int value);
    
    int GetMaxCapacity() const { return static_cast<int>(m_maxCapacity); }
    
    // Indexer
    char16_t operator[](int index) const;
    char16_t& operator[](int index);
    
    // Core append methods - optimized for performance
    StringBuilder& Append(char value);
    StringBuilder& Append(const char* value);
    StringBuilder& Append(const std::string& value);
    StringBuilder& Append(const String& value);
    StringBuilder& Append(const StringBuilder& value);
    StringBuilder& Append(char value, int repeatCount);
    StringBuilder& Append(const char* value, int startIndex, int count);
    StringBuilder& Append(const std::string& value, int startIndex, int count);
    
    // Append overloads for various types
    StringBuilder& Append(bool value);
    StringBuilder& Append(int8_t value);
    StringBuilder& Append(uint8_t value);
    StringBuilder& Append(int16_t value);
    StringBuilder& Append(uint16_t value);
    StringBuilder& Append(int32_t value);
    StringBuilder& Append(uint32_t value);
    StringBuilder& Append(int64_t value);
    StringBuilder& Append(uint64_t value);
    StringBuilder& Append(float value);
    StringBuilder& Append(double value);
    
    // AppendLine methods
    StringBuilder& AppendLine();
    StringBuilder& AppendLine(const std::string& value);
    StringBuilder& AppendLine(const String& value);
    
    // AppendFormat methods (simplified version)
    StringBuilder& AppendFormat(const std::string& format, const std::vector<std::string>& args);
    
    // Insert methods
    StringBuilder& Insert(int index, char value);
    StringBuilder& Insert(int index, const std::string& value);
    StringBuilder& Insert(int index, const String& value);
    StringBuilder& Insert(int index, char value, int count);
    
    // Remove and Replace methods
    StringBuilder& Remove(int startIndex, int length);
    StringBuilder& Replace(char oldChar, char newChar);
    StringBuilder& Replace(const std::string& oldValue, const std::string& newValue);
    StringBuilder& Replace(char oldChar, char newChar, int startIndex, int count);
    StringBuilder& Replace(const std::string& oldValue, const std::string& newValue, int startIndex, int count);
    
    // Clear method
    StringBuilder& Clear();
    
    // Capacity management
    int EnsureCapacity(int capacity);
    
    // String conversion
    std::string ToString() const;
    std::string ToString(int startIndex, int length) const;
    String ToSystemString() const;
    String ToSystemString(int startIndex, int length) const;
    
    // Utility methods
    bool Equals(const StringBuilder& other) const;
    void CopyTo(int sourceIndex, char* destination, int destinationIndex, int count) const;
    
    // RTTR registration
    RTTR_ENABLE()
};

} // namespace Text
} // namespace System
