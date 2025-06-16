
#pragma once
/*
 * InvalidOperationException.h
 * C++ implementation of .NET's System.InvalidOperationException class
 * 
 * Exception for invalid operation state
 */
#include "SystemException.h"

namespace System {

class InvalidOperationException : public SystemException {
public:
    // Constructors
    InvalidOperationException();
    explicit InvalidOperationException(const String& message);
    InvalidOperationException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~InvalidOperationException() = default;
};

} // namespace System
