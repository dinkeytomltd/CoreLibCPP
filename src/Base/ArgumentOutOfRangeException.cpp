
/*
 * ArgumentOutOfRangeException.cpp
 * Implementation of System.ArgumentOutOfRangeException
 */
#include "System/ArgumentOutOfRangeException.h"
#include <sstream>
#include <typeinfo>

namespace System {

ArgumentOutOfRangeException::ArgumentOutOfRangeException() 
    : ArgumentException("Specified argument was out of the range of valid values."),
      m_actualValue("") {
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const String& paramName)
    : ArgumentException("Specified argument was out of the range of valid values.", paramName),
      m_actualValue("") {
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const String& paramName, const String& message)
    : ArgumentException(message, paramName),
      m_actualValue("") {
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const String& paramName, const String& actualValue, const String& message)
    : ArgumentException(message, paramName),
      m_actualValue(actualValue) {
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(const String& message, std::shared_ptr<Exception> innerException)
    : ArgumentException(message, innerException),
      m_actualValue("") {
}

String ArgumentOutOfRangeException::ToString() const {
    std::ostringstream oss;
    
    // Exception type name and message
    oss << typeid(*this).name() << ": " << Message().ToStdString();
    
    // Parameter name
    if (!ParamName().ToStdString().empty()) {
        oss << "\nParameter name: " << ParamName().ToStdString();
    }
    
    // Actual value
    if (!m_actualValue.ToStdString().empty()) {
        oss << "\nActual value was " << m_actualValue.ToStdString() << ".";
    }
    
    // Stack trace
    if (!StackTrace().ToStdString().empty()) {
        oss << "\n" << StackTrace().ToStdString();
    }
    
    // Inner exception
    if (InnerException()) {
        oss << "\n ---> " << InnerException()->ToString().ToStdString();
        oss << "\n   --- End of inner exception stack trace ---";
    }
    
    return String(oss.str());
}

} // namespace System
