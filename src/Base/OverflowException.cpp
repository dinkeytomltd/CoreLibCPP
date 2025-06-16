
/*
 * OverflowException.cpp
 * Implementation of System.OverflowException
 */
#include "System/OverflowException.h"

namespace System {

OverflowException::OverflowException() 
    : SystemException("Arithmetic operation resulted in an overflow.") {
}

OverflowException::OverflowException(const String& message)
    : SystemException(message) {
}

OverflowException::OverflowException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
