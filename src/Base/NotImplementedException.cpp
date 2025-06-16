
/*
 * NotImplementedException.cpp
 * Implementation of System.NotImplementedException
 */
#include "System/NotImplementedException.h"

namespace System {

NotImplementedException::NotImplementedException() 
    : SystemException("The method or operation is not implemented.") {
}

NotImplementedException::NotImplementedException(const String& message)
    : SystemException(message) {
}

NotImplementedException::NotImplementedException(const String& message, std::shared_ptr<Exception> innerException)
    : SystemException(message, innerException) {
}

} // namespace System
