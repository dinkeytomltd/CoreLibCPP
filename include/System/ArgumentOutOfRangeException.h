
#pragma once
/*
 * ArgumentOutOfRangeException.h
 * C++ implementation of .NET's System.ArgumentOutOfRangeException class
 * 
 * Exception for out of range arguments
 */
#include "ArgumentException.h"

namespace System {

class ArgumentOutOfRangeException : public ArgumentException {
private:
    String m_actualValue;

public:
    // Constructors
    ArgumentOutOfRangeException();
    explicit ArgumentOutOfRangeException(const String& paramName);
    ArgumentOutOfRangeException(const String& paramName, const String& message);
    ArgumentOutOfRangeException(const String& paramName, const String& actualValue, const String& message);
    ArgumentOutOfRangeException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~ArgumentOutOfRangeException() = default;
    
    // Properties
    [[nodiscard]] String ActualValue() const { return m_actualValue; }
    
    // Override ToString to include actual value
    [[nodiscard]] String ToString() const override;
    
    // Static helper methods (similar to .NET 7+ helpers)
    template<typename T>
    static void ThrowIfNegative(const T& value, const String& paramName = "") {
        if (value < T{}) {
            throw ArgumentOutOfRangeException(paramName, String("Value must be non-negative."));
        }
    }
    
    template<typename T>
    static void ThrowIfZero(const T& value, const String& paramName = "") {
        if (value == T{}) {
            throw ArgumentOutOfRangeException(paramName, String("Value cannot be zero."));
        }
    }
    
    template<typename T>
    static void ThrowIfGreaterThan(const T& value, const T& other, const String& paramName = "") {
        if (value > other) {
            throw ArgumentOutOfRangeException(paramName, String("Value cannot be greater than the specified limit."));
        }
    }
    
    template<typename T>
    static void ThrowIfLessThan(const T& value, const T& other, const String& paramName = "") {
        if (value < other) {
            throw ArgumentOutOfRangeException(paramName, String("Value cannot be less than the specified limit."));
        }
    }
};

} // namespace System
