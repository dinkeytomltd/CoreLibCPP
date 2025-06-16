
#pragma once
/*
 * SystemException.h
 * C++ implementation of .NET's System.SystemException class
 * 
 * Base class for system exceptions
 */
#include "Exception.h"

namespace System {

class SystemException : public Exception {
public:
    // Constructors
    SystemException();
    explicit SystemException(const String& message);
    SystemException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~SystemException() = default;
};

} // namespace System
