
#pragma once
/*
 * ArgumentException.h
 * C++ implementation of .NET's System.ArgumentException class
 * 
 * Exception for invalid arguments
 */
#include "SystemException.h"

namespace System {

class ArgumentException : public SystemException {
private:
    String m_paramName;

public:
    // Constructors
    ArgumentException();
    explicit ArgumentException(const String& message);
    ArgumentException(const String& message, const String& paramName);
    ArgumentException(const String& message, std::shared_ptr<Exception> innerException);
    ArgumentException(const String& message, const String& paramName, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~ArgumentException() = default;
    
    // Properties
    [[nodiscard]] String ParamName() const { return m_paramName; }
    
    // Override ToString to include parameter name
    [[nodiscard]] String ToString() const override;
};

} // namespace System
