#pragma once
/*
 * String.h
 * C++ implementation of .NET's System.String class
 * 
 * Key .NET String characteristics implemented:
 * - Immutability: Once created, the string cannot be modified
 * - Reference semantics with value-type behavior
 * - Core string operations (comparison, concatenation, substring, etc.)
 */
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace System {

class String {
private:
    // Use shared_ptr for memory management, const for immutability
    std::shared_ptr<const std::string> m_data;

public:
    // Constructors
    String() : m_data(std::make_shared<const std::string>("")) {}
    String(const char* str) : m_data(std::make_shared<const std::string>(str ? str : "")) {}
    String(const std::string& str) : m_data(std::make_shared<const std::string>(str)) {}
    
    // Copy constructor (implicit in C#)
    String(const String& other) = default;
    
    // Assignment operator (implicit in C#)
    String& operator=(const String& other) = default;
    
    // Property: Length (equivalent to .NET String.Length)
    [[nodiscard]] size_t Length() const { return m_data->length(); }
    
    // Indexer (equivalent to .NET string[index])
    [[nodiscard]] char operator[](size_t index) const {
        if (index >= Length()) {
            throw std::out_of_range("Index was out of range");
        }
        return (*m_data)[index];
    }
    
    // Equality methods (equivalent to .NET String.Equals)
    [[nodiscard]] bool Equals(const String& other) const {
        return *m_data == *other.m_data;
    }
    
    [[nodiscard]] bool Equals(const String& other, bool ignoreCase) const {
        if (!ignoreCase) return Equals(other);
        
        if (Length() != other.Length()) return false;
        
        for (size_t i = 0; i < Length(); i++) {
            if (std::tolower((*m_data)[i]) != std::tolower((*other.m_data)[i])) {
                return false;
            }
        }
        return true;
    }
    
    // Comparison (equivalent to .NET String.CompareTo)
    [[nodiscard]] int CompareTo(const String& other) const {
        return m_data->compare(*other.m_data);
    }
    
    // Substring (equivalent to .NET String.Substring)
    [[nodiscard]] String Substring(size_t startIndex) const {
        if (startIndex > Length()) {
            throw std::out_of_range("startIndex cannot exceed Length");
        }
        return String(m_data->substr(startIndex));
    }
    
    [[nodiscard]] String Substring(size_t startIndex, size_t length) const {
        if (startIndex > Length()) {
            throw std::out_of_range("startIndex cannot exceed Length");
        }
        if (startIndex + length > Length()) {
            throw std::out_of_range("startIndex + length cannot exceed Length");
        }
        return String(m_data->substr(startIndex, length));
    }
    
    // Contains (equivalent to .NET String.Contains)
    [[nodiscard]] bool Contains(const String& value) const {
        return m_data->find(*value.m_data) != std::string::npos;
    }
    
    // StartsWith (equivalent to .NET String.StartsWith)
    [[nodiscard]] bool StartsWith(const String& value) const {
        if (value.Length() > Length()) return false;
        return m_data->substr(0, value.Length()) == *value.m_data;
    }
    
    // EndsWith (equivalent to .NET String.EndsWith)
    [[nodiscard]] bool EndsWith(const String& value) const {
        if (value.Length() > Length()) return false;
        return m_data->substr(Length() - value.Length()) == *value.m_data;
    }
    
    // IndexOf (equivalent to .NET String.IndexOf)
    [[nodiscard]] size_t IndexOf(char value) const {
        return m_data->find(value);
    }
    
    [[nodiscard]] size_t IndexOf(const String& value) const {
        return m_data->find(*value.m_data);
    }
    
    // Trim (equivalent to .NET String.Trim)
    [[nodiscard]] String Trim() const {
        auto copy = *m_data;
        copy.erase(0, copy.find_first_not_of(" \t\n\r\f\v"));
        copy.erase(copy.find_last_not_of(" \t\n\r\f\v") + 1);
        return String(copy);
    }
    
    // ToUpper (equivalent to .NET String.ToUpper)
    [[nodiscard]] String ToUpper() const {
        std::string result = *m_data;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        return String(result);
    }
    
    // ToLower (equivalent to .NET String.ToLower)
    [[nodiscard]] String ToLower() const {
        std::string result = *m_data;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return String(result);
    }
    
    // Split (simplified version of .NET String.Split)
    [[nodiscard]] std::vector<String> Split(char delimiter) const {
        std::vector<String> result;
        std::string current;
        
        for (char c : *m_data) {
            if (c == delimiter) {
                result.push_back(String(current));
                current.clear();
            } else {
                current += c;
            }
        }
        
        result.push_back(String(current));
        return result;
    }
    
    // Static methods
    
    // Concat (equivalent to .NET String.Concat)
    [[nodiscard]] static String Concat(const String& str1, const String& str2) {
        return String(*str1.m_data + *str2.m_data);
    }
    
    [[nodiscard]] static String Concat(const String& str1, const String& str2, const String& str3) {
        return String(*str1.m_data + *str2.m_data + *str3.m_data);
    }
    
    // Join (equivalent to .NET String.Join)
    [[nodiscard]] static String Join(const String& separator, const std::vector<String>& values) {
        if (values.empty()) return String();
        
        std::string result = *values[0].m_data;
        for (size_t i = 1; i < values.size(); i++) {
            result += *separator.m_data + *values[i].m_data;
        }
        return String(result);
    }
    
    // IsNullOrEmpty (equivalent to .NET String.IsNullOrEmpty)
    [[nodiscard]] static bool IsNullOrEmpty(const String& value) {
        return value.Length() == 0;
    }
    
    // Conversion to std::string (for interoperability)
    [[nodiscard]] std::string ToStdString() const {
        return *m_data;
    }
    
    // Operators
    
    // Equality operators
    friend bool operator==(const String& left, const String& right) {
        return left.Equals(right);
    }
    
    friend bool operator!=(const String& left, const String& right) {
        return !left.Equals(right);
    }
    
    // Concatenation operator
    friend String operator+(const String& left, const String& right) {
        return String::Concat(left, right);
    }
};

} // namespace System
