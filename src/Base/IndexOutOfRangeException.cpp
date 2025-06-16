
/*
 * IndexOutOfRangeException.cpp
 * Implementation of System.IndexOutOfRangeException
 */
#include "System/IndexOutOfRangeException.h"

namespace System {

IndexOutOfRangeException::IndexOutOfRangeException() 
    : SystemException("Index was outside the bounds of the array.") {
}

IndexOutOfRangeException::IndexOutOfRangeException(const String& message)
    : SystemException(message) {
}

IndexOutOfRangeException::IndexOutOfRangeException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
