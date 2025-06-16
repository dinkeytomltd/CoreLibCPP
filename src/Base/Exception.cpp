
/*
 * Exception.cpp
 * Implementation of System.Exception
 */
#include "System/Exception.h"
#include <sstream>
#include <typeinfo>

namespace System {

Exception::Exception() 
    : m_message("Exception of type 'System.Exception' was thrown."),
      m_innerException(nullptr),
      m_stackTrace(BuildStackTrace()),
      m_source(""),
      m_data(std::make_shared<ExceptionData>()) {
}

Exception::Exception(const String& message)
    : m_message(message),
      m_innerException(nullptr),
      m_stackTrace(BuildStackTrace()),
      m_source(""),
      m_data(std::make_shared<ExceptionData>()) {
}

Exception::Exception(const String& message, std::shared_ptr<Exception> innerException)
    : m_message(message),
      m_innerException(innerException),
      m_stackTrace(BuildStackTrace()),
      m_source(""),
      m_data(std::make_shared<ExceptionData>()) {
}

String Exception::BuildStackTrace() const {
    // Simplified stack trace - in a real implementation, this would use
    // platform-specific APIs to get actual stack information
    return String("   at System.Exception..ctor()");
}

String Exception::ToString() const {
    std::ostringstream oss;
    
    // Exception type name
    oss << typeid(*this).name() << ": " << m_message.ToStdString();
    
    // Stack trace
    if (!m_stackTrace.ToStdString().empty()) {
        oss << "\n" << m_stackTrace.ToStdString();
    }
    
    // Inner exception
    if (m_innerException) {
        oss << "\n ---> " << m_innerException->ToString().ToStdString();
        oss << "\n   --- End of inner exception stack trace ---";
    }
    
    return String(oss.str());
}

bool Exception::Equals(const Object& obj) const {
    const Exception* other = dynamic_cast<const Exception*>(&obj);
    if (!other) return false;
    
    return m_message.Equals(other->m_message) &&
           ((m_innerException == nullptr && other->m_innerException == nullptr) ||
            (m_innerException != nullptr && other->m_innerException != nullptr &&
             m_innerException->Equals(*other->m_innerException)));
}

int Exception::GetHashCode() const {
    size_t hash = std::hash<std::string>{}(m_message.ToStdString());
    if (m_innerException) {
        hash ^= static_cast<size_t>(m_innerException->GetHashCode()) << 1;
    }
    return static_cast<int>(hash);
}

std::shared_ptr<Exception> Exception::GetBaseException() const {
    // Start with the innermost exception and traverse to find the base
    auto current = m_innerException;
    if (!current) {
        // If no inner exception, this is the base exception
        // We can't use shared_from_this here, so we'll create a new shared_ptr
        // In practice, this method would typically be called on a shared_ptr already
        return nullptr; // Caller should handle this case
    }
    
    while (current->m_innerException) {
        current = current->m_innerException;
    }
    return current;
}

const char* Exception::what() const noexcept {
    try {
        m_whatCache = m_message.ToStdString();
        return m_whatCache.c_str();
    } catch (...) {
        return "Exception occurred";
    }
}

} // namespace System
