
#pragma once

#include "System/Exception.h"
#include "System/String.h"

namespace System::Reflection
{
    /// <summary>
    /// The exception that is thrown when an attempt is made to dynamically access a method that does not exist.
    /// </summary>
    class TargetException : public System::Exception
    {
    public:
        /// <summary>
        /// Initializes a new instance of the TargetException class with a default message.
        /// </summary>
        TargetException() : Exception("Exception has been thrown by the target of an invocation.") {}

        /// <summary>
        /// Initializes a new instance of the TargetException class with a specified error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        explicit TargetException(const System::String& message) : Exception(message) {}

        /// <summary>
        /// Initializes a new instance of the TargetException class with a specified error message and a reference to the inner exception.
        /// </summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        TargetException(const System::String& message, const std::exception& innerException) 
            : Exception(message, innerException) {}
    };

    /// <summary>
    /// The exception that is thrown by methods invoked through reflection.
    /// </summary>
    class TargetInvocationException : public System::Exception
    {
    public:
        /// <summary>
        /// Initializes a new instance of the TargetInvocationException class with a default message.
        /// </summary>
        TargetInvocationException() : Exception("Exception has been thrown by the target of an invocation.") {}

        /// <summary>
        /// Initializes a new instance of the TargetInvocationException class with a specified error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        explicit TargetInvocationException(const System::String& message) : Exception(message) {}

        /// <summary>
        /// Initializes a new instance of the TargetInvocationException class with a specified error message and a reference to the inner exception.
        /// </summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        TargetInvocationException(const System::String& message, const std::exception& innerException) 
            : Exception(message, innerException) {}
    };

    /// <summary>
    /// The exception that is thrown when the number of parameters for an invocation does not match the number expected.
    /// </summary>
    class TargetParameterCountException : public System::Exception
    {
    public:
        /// <summary>
        /// Initializes a new instance of the TargetParameterCountException class with a default message.
        /// </summary>
        TargetParameterCountException() : Exception("Parameter count mismatch.") {}

        /// <summary>
        /// Initializes a new instance of the TargetParameterCountException class with a specified error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        explicit TargetParameterCountException(const System::String& message) : Exception(message) {}

        /// <summary>
        /// Initializes a new instance of the TargetParameterCountException class with a specified error message and a reference to the inner exception.
        /// </summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        TargetParameterCountException(const System::String& message, const std::exception& innerException) 
            : Exception(message, innerException) {}
    };

    /// <summary>
    /// The exception that is thrown when binding to a member results in more than one member matching the binding criteria.
    /// </summary>
    class AmbiguousMatchException : public System::Exception
    {
    public:
        /// <summary>
        /// Initializes a new instance of the AmbiguousMatchException class with a default message.
        /// </summary>
        AmbiguousMatchException() : Exception("Ambiguous match found.") {}

        /// <summary>
        /// Initializes a new instance of the AmbiguousMatchException class with a specified error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        explicit AmbiguousMatchException(const System::String& message) : Exception(message) {}

        /// <summary>
        /// Initializes a new instance of the AmbiguousMatchException class with a specified error message and a reference to the inner exception.
        /// </summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        AmbiguousMatchException(const System::String& message, const std::exception& innerException) 
            : Exception(message, innerException) {}
    };

    /// <summary>
    /// The exception that is thrown when a requested method or operation is not implemented.
    /// </summary>
    class ReflectionTypeLoadException : public System::Exception
    {
    public:
        /// <summary>
        /// Initializes a new instance of the ReflectionTypeLoadException class with a default message.
        /// </summary>
        ReflectionTypeLoadException() : Exception("Unable to load one or more of the requested types.") {}

        /// <summary>
        /// Initializes a new instance of the ReflectionTypeLoadException class with a specified error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        explicit ReflectionTypeLoadException(const System::String& message) : Exception(message) {}

        /// <summary>
        /// Initializes a new instance of the ReflectionTypeLoadException class with a specified error message and a reference to the inner exception.
        /// </summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        ReflectionTypeLoadException(const System::String& message, const std::exception& innerException) 
            : Exception(message, innerException) {}
    };
}
