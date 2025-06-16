
#pragma once
/*
 * IOException.h
 * C++ implementation of .NET's System.IO.IOException class
 * 
 * The exception that is thrown when an I/O error occurs.
 */
#include "../SystemException.h"

namespace System::IO {

class IOException : public SystemException {
public:
    // Constructors
    IOException();
    explicit IOException(const String& message);
    IOException(const String& message, std::shared_ptr<Exception> innerException);
    IOException(const String& message, int hresult);
    
    // Virtual destructor
    virtual ~IOException() = default;
    
    // Copy constructor and assignment
    IOException(const IOException& other) = default;
    IOException& operator=(const IOException& other) = default;
};

} // namespace System::IO
