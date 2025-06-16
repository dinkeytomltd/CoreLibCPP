
#pragma once
/*
 * FileNotFoundException.h
 * C++ implementation of .NET's System.IO.FileNotFoundException class
 * 
 * The exception that is thrown when an attempt to access a file that does not exist on disk fails.
 */
#include "IOException.h"

namespace System::IO {

class FileNotFoundException : public IOException {
private:
    String m_fileName;

public:
    // Constructors
    FileNotFoundException();
    explicit FileNotFoundException(const String& message);
    FileNotFoundException(const String& message, const String& fileName);
    FileNotFoundException(const String& message, std::shared_ptr<Exception> innerException);
    FileNotFoundException(const String& message, const String& fileName, std::shared_ptr<Exception> innerException);
    
    // Virtual destructor
    virtual ~FileNotFoundException() = default;
    
    // Copy constructor and assignment
    FileNotFoundException(const FileNotFoundException& other) = default;
    FileNotFoundException& operator=(const FileNotFoundException& other) = default;
    
    // Properties
    [[nodiscard]] String FileName() const { return m_fileName; }
    
    // Override Message to include file name
    [[nodiscard]] String Message() const override;
};

} // namespace System::IO
