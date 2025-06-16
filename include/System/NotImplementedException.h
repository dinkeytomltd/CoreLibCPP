
#pragma once
/*
 * NotImplementedException.h
 * C++ implementation of .NET's System.NotImplementedException class
 * 
 * Exception for not implemented features
 */
#include "SystemException.h"

namespace System {

class NotImplementedException : public SystemException {
public:
    // Constructors
    NotImplementedException();
    explicit NotImplementedException(const String& message);
    NotImplementedException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~NotImplementedException() = default;
};

} // namespace System
