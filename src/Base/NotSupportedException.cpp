
/*
 * NotSupportedException.cpp
 * Implementation of System.NotSupportedException
 */
#include "System/NotSupportedException.h"

namespace System {

NotSupportedException::NotSupportedException() 
    : SystemException("Specified method is not supported.") {
}

NotSupportedException::NotSupportedException(const String& message)
    : SystemException(message) {
}

NotSupportedException::NotSupportedException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
