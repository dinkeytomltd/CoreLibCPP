
#pragma once
/*
 * EndOfStreamException.h
 * C++ implementation of .NET's System.IO.EndOfStreamException class
 * 
 * The exception that is thrown when reading is attempted past the end of a stream.
 */
#include "IOException.h"

namespace System::IO {

class EndOfStreamException : public IOException {
public:
    // Constructors
    EndOfStreamException();
    explicit EndOfStreamException(const String& message);
    EndOfStreamException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~EndOfStreamException() = default;
    
    // Copy constructor and assignment
    EndOfStreamException(const EndOfStreamException& other) = default;
    EndOfStreamException& operator=(const EndOfStreamException& other) = default;
};

} // namespace System::IO
