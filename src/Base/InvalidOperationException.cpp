
/*
 * InvalidOperationException.cpp
 * Implementation of System.InvalidOperationException
 */
#include "System/InvalidOperationException.h"

namespace System {

InvalidOperationException::InvalidOperationException() 
    : SystemException("Operation is not valid due to the current state of the object.") {
}

InvalidOperationException::InvalidOperationException(const String& message)
    : SystemException(message) {
}

InvalidOperationException::InvalidOperationException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
