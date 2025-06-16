
#pragma once

#include "System/Reflection/MemberInfo.h"
#include "System/Reflection/ParameterInfo.h"
#include <rttr/constructor.h>
#include <vector>

namespace System::Reflection
{
    /// <summary>
    /// Discovers the attributes of a class constructor and provides access to constructor metadata.
    /// </summary>
    class ConstructorInfo : public MemberInfo
    {
    private:
        rttr::constructor _rttrConstructor;

    public:
        /// <summary>
        /// Initializes a new instance of the ConstructorInfo class.
        /// </summary>
        ConstructorInfo() = default;

        /// <summary>
        /// Initializes a new instance of the ConstructorInfo class with an RTTR constructor.
        /// </summary>
        /// <param name="rttrConstructor">The RTTR constructor to wrap.</param>
        /// <param name="name">The name of the constructor.</param>
        ConstructorInfo(const rttr::constructor& rttrConstructor, const System::String& name);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~ConstructorInfo() = default;

        /// <summary>
        /// Gets the parameters of the constructor.
        /// </summary>
        /// <returns>An array of ParameterInfo objects representing the parameters of the constructor.</returns>
        virtual std::vector<ParameterInfo> GetParameters() const;

        /// <summary>
        /// Gets a value indicating whether this constructor is public.
        /// </summary>
        /// <returns>true if this constructor is public; otherwise, false.</returns>
        virtual bool IsPublic() const;

        /// <summary>
        /// Gets a value indicating whether this constructor is private.
        /// </summary>
        /// <returns>true if this constructor is private; otherwise, false.</returns>
        virtual bool IsPrivate() const;

        /// <summary>
        /// Gets the underlying RTTR constructor.
        /// </summary>
        /// <returns>The RTTR constructor object.</returns>
        const rttr::constructor& GetRttrConstructor() const { return _rttrConstructor; }
    };
}
