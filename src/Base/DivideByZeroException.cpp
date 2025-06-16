
/*
 * DivideByZeroException.cpp
 * Implementation of System.DivideByZeroException
 */
#include "System/DivideByZeroException.h"

namespace System {

DivideByZeroException::DivideByZeroException() 
    : SystemException("Attempted to divide by zero.") {
}

DivideByZeroException::DivideByZeroException(const String& message)
    : SystemException(message) {
}

DivideByZeroException::DivideByZeroException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
