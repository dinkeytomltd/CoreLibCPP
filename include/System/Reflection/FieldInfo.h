
#pragma once

#include "System/Reflection/MemberInfo.h"

namespace System::Reflection
{
    // Forward declarations
    class Type;

    /// <summary>
    /// Discovers the attributes of a field and provides access to field metadata.
    /// </summary>
    class FieldInfo : public MemberInfo
    {
    public:
        /// <summary>
        /// Initializes a new instance of the FieldInfo class.
        /// </summary>
        FieldInfo() = default;

        /// <summary>
        /// Initializes a new instance of the FieldInfo class.
        /// </summary>
        /// <param name="name">The name of the field.</param>
        FieldInfo(const System::String& name);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~FieldInfo() = default;

        /// <summary>
        /// Gets the type of this field object.
        /// </summary>
        /// <returns>The type of this field.</returns>
        virtual Type GetFieldType() const;

        /// <summary>
        /// Gets a value indicating whether the field is static.
        /// </summary>
        /// <returns>true if this field is static; otherwise, false.</returns>
        virtual bool IsStatic() const;

        /// <summary>
        /// Gets a value indicating whether this field is public.
        /// </summary>
        /// <returns>true if this field is public; otherwise, false.</returns>
        virtual bool IsPublic() const;

        /// <summary>
        /// Gets a value indicating whether this field is private.
        /// </summary>
        /// <returns>true if this field is private; otherwise, false.</returns>
        virtual bool IsPrivate() const;
    };
}
