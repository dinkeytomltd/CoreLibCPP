
#pragma once
/*
 * FormatException.h
 * C++ implementation of .NET's System.FormatException class
 * 
 * Exception for string format errors
 */
#include "SystemException.h"

namespace System {

class FormatException : public SystemException {
public:
    // Constructors
    FormatException();
    explicit FormatException(const String& message);
    FormatException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~FormatException() = default;
};

} // namespace System
