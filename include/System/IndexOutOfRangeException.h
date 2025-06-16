
#pragma once
/*
 * IndexOutOfRangeException.h
 * C++ implementation of .NET's System.IndexOutOfRangeException class
 * 
 * Exception for array/collection index errors
 */
#include "SystemException.h"

namespace System {

class IndexOutOfRangeException : public SystemException {
public:
    // Constructors
    IndexOutOfRangeException();
    explicit IndexOutOfRangeException(const String& message);
    IndexOutOfRangeException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~IndexOutOfRangeException() = default;
};

} // namespace System
