
#pragma once

#include "System/Reflection/MemberInfo.h"
#include <rttr/property.h>

namespace System::Reflection
{
    // Forward declarations
    class Type;

    /// <summary>
    /// Discovers the attributes of a property and provides access to property metadata.
    /// </summary>
    class PropertyInfo : public MemberInfo
    {
    private:
        rttr::property _rttrProperty;

    public:
        /// <summary>
        /// Initializes a new instance of the PropertyInfo class.
        /// </summary>
        PropertyInfo() = default;

        /// <summary>
        /// Initializes a new instance of the PropertyInfo class with an RTTR property.
        /// </summary>
        /// <param name="rttrProperty">The RTTR property to wrap.</param>
        /// <param name="name">The name of the property.</param>
        PropertyInfo(const rttr::property& rttrProperty, const System::String& name);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~PropertyInfo() = default;

        /// <summary>
        /// Gets the type of this property.
        /// </summary>
        /// <returns>The type of this property.</returns>
        virtual Type GetPropertyType() const;

        /// <summary>
        /// Gets a value indicating whether the property can be read.
        /// </summary>
        /// <returns>true if this property can be read; otherwise, false.</returns>
        virtual bool CanRead() const;

        /// <summary>
        /// Gets a value indicating whether the property can be written to.
        /// </summary>
        /// <returns>true if this property can be written to; otherwise, false.</returns>
        virtual bool CanWrite() const;

        /// <summary>
        /// Gets the underlying RTTR property.
        /// </summary>
        /// <returns>The RTTR property object.</returns>
        const rttr::property& GetRttrProperty() const { return _rttrProperty; }
    };
}
