
/*
 * FormatException.cpp
 * Implementation of System.FormatException
 */
#include "System/FormatException.h"

namespace System {

FormatException::FormatException() 
    : SystemException("One of the identified items was in an invalid format.") {
}

FormatException::FormatException(const String& message)
    : SystemException(message) {
}

FormatException::FormatException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
