
#pragma once
/*
 * DirectoryNotFoundException.h
 * C++ implementation of .NET's System.IO.DirectoryNotFoundException class
 * 
 * The exception that is thrown when part of a file or directory cannot be found.
 */
#include "IOException.h"

namespace System::IO {

class DirectoryNotFoundException : public IOException {
public:
    // Constructors
    DirectoryNotFoundException();
    explicit DirectoryNotFoundException(const String& message);
    DirectoryNotFoundException(const String& message, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~DirectoryNotFoundException() = default;
    
    // Copy constructor and assignment
    DirectoryNotFoundException(const DirectoryNotFoundException& other) = default;
    DirectoryNotFoundException& operator=(const DirectoryNotFoundException& other) = default;
};

} // namespace System::IO
