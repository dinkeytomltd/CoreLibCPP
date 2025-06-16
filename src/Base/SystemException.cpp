
/*
 * SystemException.cpp
 * Implementation of System.SystemException
 */
#include "System/SystemException.h"

namespace System {

SystemException::SystemException() 
    : Exception("System error.") {
}

SystemException::SystemException(const String& message)
    : Exception(message) {
}

SystemException::SystemException(const String& message, std::shared_ptr<Exception> innerException)
    : Exception(message, innerException) {
}

} // namespace System
