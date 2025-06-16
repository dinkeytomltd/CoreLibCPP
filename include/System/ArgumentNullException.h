
#pragma once
/*
 * ArgumentNullException.h
 * C++ implementation of .NET's System.ArgumentNullException class
 * 
 * Exception for null arguments
 */
#include "ArgumentException.h"

namespace System {

class ArgumentNullException : public ArgumentException {
public:
    // Constructors
    ArgumentNullException();
    explicit ArgumentNullException(const String& paramName);
    ArgumentNullException(const String& paramName, const String& message);
    ArgumentNullException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~ArgumentNullException() = default;
    
    // Static helper method (similar to .NET 6+ ThrowIfNull)
    template<typename T>
    static void ThrowIfNull(const T* argument, const String& paramName = "") {
        if (argument == nullptr) {
            if (paramName.ToStdString().empty()) {
                throw ArgumentNullException();
            } else {
                throw ArgumentNullException(paramName);
            }
        }
    }
    
    template<typename T>
    static void ThrowIfNull(const std::shared_ptr<T>& argument, const String& paramName = "") {
        if (argument == nullptr) {
            if (paramName.ToStdString().empty()) {
                throw ArgumentNullException();
            } else {
                throw ArgumentNullException(paramName);
            }
        }
    }
};

} // namespace System
