
#pragma once

#include "System/Object.h"
#include "System/String.h"

namespace System::Reflection
{
    /// <summary>
    /// Represents an assembly, which is a compiled library of code.
    /// </summary>
    class Assembly : public System::Object
    {
    private:
        System::String _fullName;

    public:
        /// <summary>
        /// Initializes a new instance of the Assembly class.
        /// </summary>
        Assembly() = default;

        /// <summary>
        /// Initializes a new instance of the Assembly class with a name.
        /// </summary>
        /// <param name="fullName">The full name of the assembly.</param>
        explicit Assembly(const System::String& fullName);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~Assembly() = default;

        /// <summary>
        /// Gets the display name of the assembly.
        /// </summary>
        /// <returns>The display name of the assembly.</returns>
        virtual System::String GetFullName() const { return _fullName; }
    };
}
