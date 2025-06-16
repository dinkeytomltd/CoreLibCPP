
/*
 * ArgumentNullException.cpp
 * Implementation of System.ArgumentNullException
 */
#include "System/ArgumentNullException.h"

namespace System {

ArgumentNullException::ArgumentNullException() 
    : ArgumentException("Value cannot be null.") {
}

ArgumentNullException::ArgumentNullException(const String& paramName)
    : ArgumentException("Value cannot be null.", paramName) {
}

ArgumentNullException::ArgumentNullException(const String& paramName, const String& message)
    : ArgumentException(message, paramName) {
}

ArgumentNullException::ArgumentNullException(const String& message, std::shared_ptr<Exception> innerException)
    : ArgumentException(message, innerException) {
}

} // namespace System
