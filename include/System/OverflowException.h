
#pragma once
/*
 * OverflowException.h
 * C++ implementation of .NET's System.OverflowException class
 * 
 * Exception for arithmetic overflow
 */
#include "SystemException.h"

namespace System {

class OverflowException : public SystemException {
public:
    // Constructors
    OverflowException();
    explicit OverflowException(const String& message);
    OverflowException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~OverflowException() = default;
};

} // namespace System
