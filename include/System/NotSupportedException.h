
#pragma once
/*
 * NotSupportedException.h
 * C++ implementation of .NET's System.NotSupportedException class
 * 
 * Exception for unsupported operations
 */
#include "SystemException.h"

namespace System {

class NotSupportedException : public SystemException {
public:
    // Constructors
    NotSupportedException();
    explicit NotSupportedException(const String& message);
    NotSupportedException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~NotSupportedException() = default;
};

} // namespace System
