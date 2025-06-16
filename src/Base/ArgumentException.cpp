
/*
 * ArgumentException.cpp
 * Implementation of System.ArgumentException
 */
#include "System/ArgumentException.h"
#include <sstream>
#include <typeinfo>

namespace System {

ArgumentException::ArgumentException() 
    : SystemException("Value does not fall within the expected range."),
      m_paramName("") {
}

ArgumentException::ArgumentException(const String& message)
    : SystemException(message),
      m_paramName("") {
}

ArgumentException::ArgumentException(const String& message, const String& paramName)
    : SystemException(message),
      m_paramName(paramName) {
}

ArgumentException::ArgumentException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException),
      m_paramName("") {
}

ArgumentException::ArgumentException(const String& message, const String& paramName, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException),
      m_paramName(paramName) {
}

String ArgumentException::ToString() const {
    std::ostringstream oss;
    
    // Exception type name and message
    oss << typeid(*this).name() << ": " << Message().ToStdString();
    
    // Parameter name
    if (!m_paramName.ToStdString().empty()) {
        oss << "\nParameter name: " << m_paramName.ToStdString();
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
