
#pragma once
/*
 * NullReferenceException.h
 * C++ implementation of .NET's System.NullReferenceException class
 * 
 * Exception for null reference access
 */
#include "SystemException.h"

namespace System {

class NullReferenceException : public SystemException {
public:
    // Constructors
    NullReferenceException();
    explicit NullReferenceException(const String& message);
    NullReferenceException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~NullReferenceException() = default;
};

} // namespace System
