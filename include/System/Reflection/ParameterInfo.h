
#pragma once

#include "System/Object.h"
#include "System/String.h"

namespace System::Reflection
{
    // Forward declarations
    class Type;

    /// <summary>
    /// Discovers the attributes of a parameter and provides access to parameter metadata.
    /// </summary>
    class ParameterInfo : public System::Object
    {
    private:
        System::String _name;
        int _position;

    public:
        /// <summary>
        /// Initializes a new instance of the ParameterInfo class.
        /// </summary>
        ParameterInfo() = default;

        /// <summary>
        /// Initializes a new instance of the ParameterInfo class.
        /// </summary>
        /// <param name="name">The name of the parameter.</param>
        /// <param name="position">The position of the parameter.</param>
        ParameterInfo(const System::String& name, int position);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~ParameterInfo() = default;

        /// <summary>
        /// Gets the name of the parameter.
        /// </summary>
        /// <returns>A String containing the name of this parameter.</returns>
        virtual System::String GetName() const { return _name; }

        /// <summary>
        /// Gets the zero-based position of the parameter in the formal parameter list.
        /// </summary>
        /// <returns>An integer representing the position this parameter occupies in the parameter list.</returns>
        virtual int GetPosition() const { return _position; }

        /// <summary>
        /// Gets the Type of this parameter.
        /// </summary>
        /// <returns>The Type object that represents the Type of this parameter.</returns>
        virtual Type GetParameterType() const;
    };
}
