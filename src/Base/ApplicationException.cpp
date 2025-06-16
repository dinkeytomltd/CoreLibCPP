
/*
 * ApplicationException.cpp
 * Implementation of System.ApplicationException
 */
#include "System/ApplicationException.h"

namespace System {

ApplicationException::ApplicationException() 
    : Exception("Application exception.") {
}

ApplicationException::ApplicationException(const String& message)
    : Exception(message) {
}

ApplicationException::ApplicationException(const String& message, std::shared_ptr<Exception> innerException)
    : Exception(message, innerException) {
}

} // namespace System
