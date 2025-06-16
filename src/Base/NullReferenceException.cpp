
/*
 * NullReferenceException.cpp
 * Implementation of System.NullReferenceException
 */
#include "System/NullReferenceException.h"

namespace System {

NullReferenceException::NullReferenceException() 
    : SystemException("Object reference not set to an instance of an object.") {
}

NullReferenceException::NullReferenceException(const String& message)
    : SystemException(message) {
}

NullReferenceException::NullReferenceException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
