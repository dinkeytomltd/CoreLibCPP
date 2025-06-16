
#pragma once

namespace System::Reflection
{
    /// <summary>
    /// Defines the valid calling conventions for a method.
    /// </summary>
    enum class CallingConventions
    {
        /// <summary>
        /// Specifies the default calling convention as determined by the common language runtime.
        /// </summary>
        Standard = 1,

        /// <summary>
        /// Specifies the calling convention for methods with variable arguments.
        /// </summary>
        VarArgs = 2,

        /// <summary>
        /// Specifies that either the Standard or the VarArgs calling convention may be used.
        /// </summary>
        Any = Standard | VarArgs,

        /// <summary>
        /// Specifies an instance or virtual method (not a static method).
        /// </summary>
        HasThis = 32,

        /// <summary>
        /// Specifies that the signature is a function pointer signature, representing a call to an instance or virtual method (not a static method).
        /// </summary>
        ExplicitThis = 64
    };

    /// <summary>
    /// Bitwise OR operator for CallingConventions.
    /// </summary>
    inline CallingConventions operator|(CallingConventions left, CallingConventions right)
    {
        return static_cast<CallingConventions>(static_cast<int>(left) | static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise AND operator for CallingConventions.
    /// </summary>
    inline CallingConventions operator&(CallingConventions left, CallingConventions right)
    {
        return static_cast<CallingConventions>(static_cast<int>(left) & static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise XOR operator for CallingConventions.
    /// </summary>
    inline CallingConventions operator^(CallingConventions left, CallingConventions right)
    {
        return static_cast<CallingConventions>(static_cast<int>(left) ^ static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise NOT operator for CallingConventions.
    /// </summary>
    inline CallingConventions operator~(CallingConventions value)
    {
        return static_cast<CallingConventions>(~static_cast<int>(value));
    }
}
