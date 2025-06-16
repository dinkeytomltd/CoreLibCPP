
#pragma once
/*
 * DivideByZeroException.h
 * C++ implementation of .NET's System.DivideByZeroException class
 * 
 * Exception for division by zero
 */
#include "SystemException.h"

namespace System {

class DivideByZeroException : public SystemException {
public:
    // Constructors
    DivideByZeroException();
    explicit DivideByZeroException(const String& message);
    DivideByZeroException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~DivideByZeroException() = default;
};

} // namespace System
