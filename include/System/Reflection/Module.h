
#pragma once

#include "System/Object.h"
#include "System/String.h"

namespace System::Reflection
{
    /// <summary>
    /// Performs reflection on a module.
    /// </summary>
    class Module : public System::Object
    {
    private:
        System::String _name;

    public:
        /// <summary>
        /// Initializes a new instance of the Module class.
        /// </summary>
        Module() = default;

        /// <summary>
        /// Initializes a new instance of the Module class with a name.
        /// </summary>
        /// <param name="name">The name of the module.</param>
        explicit Module(const System::String& name);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~Module() = default;

        /// <summary>
        /// Gets a string representing the name of the module.
        /// </summary>
        /// <returns>The name of the module.</returns>
        virtual System::String GetName() const { return _name; }
    };
}
