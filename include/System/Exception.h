
#pragma once
/*
 * Exception.h
 * C++ implementation of .NET's System.Exception class hierarchy
 * 
 * Key .NET Exception characteristics implemented:
 * - Base class for all exceptions
 * - Dual inheritance from std::exception and System::Object
 * - Properties: Message, InnerException, StackTrace, Data, Source
 * - Methods: ToString(), GetBaseException(), what() override
 * - Exception chaining support
 */
#include <memory>
#include <string>
#include <exception>
#include <map>
#include <sstream>
#include "Object.h"
#include "String.h"

namespace System {

// Forward declarations
class String;

// Data dictionary type for additional exception data (using map instead of unordered_map)
using ExceptionData = std::map<std::string, std::string>;

class Exception : public std::exception, public Object {
private:
    String m_message;
    std::shared_ptr<Exception> m_innerException;
    String m_stackTrace;
    String m_source;
    std::shared_ptr<ExceptionData> m_data;
    
    // Helper method to build stack trace (simplified)
    String BuildStackTrace() const;

public:
    // Constructors
    Exception();
    explicit Exception(const String& message);
    Exception(const String& message, std::shared_ptr<Exception> innerException);
    
    // Copy constructor and assignment
    Exception(const Exception& other) = default;
    Exception& operator=(const Exception& other) = default;
    
    // Virtual destructor
    virtual ~Exception() = default;
    
    // Properties
    [[nodiscard]] virtual String Message() const { return m_message; }
    [[nodiscard]] std::shared_ptr<Exception> InnerException() const { return m_innerException; }
    [[nodiscard]] virtual String StackTrace() const { return m_stackTrace; }
    [[nodiscard]] String Source() const { return m_source; }
    void SetSource(const String& source) { m_source = source; }
    [[nodiscard]] std::shared_ptr<ExceptionData> Data() const { return m_data; }
    
    // Methods from Object
    [[nodiscard]] String ToString() const override;
    [[nodiscard]] bool Equals(const Object& obj) const override;
    [[nodiscard]] int GetHashCode() const override;
    
    // Exception-specific methods
    [[nodiscard]] virtual std::shared_ptr<Exception> GetBaseException() const;
    
    // std::exception interface
    [[nodiscard]] const char* what() const noexcept override;
    
private:
    mutable std::string m_whatCache; // Cache for what() method
};

// Type alias for shared_ptr<Exception>
using ExceptionPtr = std::shared_ptr<Exception>;

} // namespace System
