
#pragma once
/*
 * ApplicationException.h
 * C++ implementation of .NET's System.ApplicationException class
 * 
 * Base class for application exceptions
 */
#include "Exception.h"

namespace System {

class ApplicationException : public Exception {
public:
    // Constructors
    ApplicationException();
    explicit ApplicationException(const String& message);
    ApplicationException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~ApplicationException() = default;
};

} // namespace System
