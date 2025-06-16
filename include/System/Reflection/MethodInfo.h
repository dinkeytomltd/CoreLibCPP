
#pragma once

#include "System/Reflection/MemberInfo.h"
#include "System/Reflection/ParameterInfo.h"
#include <rttr/method.h>
#include <vector>

namespace System::Reflection
{
    // Forward declarations
    class Type;

    /// <summary>
    /// Discovers the attributes of a method and provides access to method metadata.
    /// </summary>
    class MethodInfo : public MemberInfo
    {
    private:
        rttr::method _rttrMethod;

    public:
        /// <summary>
        /// Initializes a new instance of the MethodInfo class.
        /// </summary>
        MethodInfo() = default;

        /// <summary>
        /// Initializes a new instance of the MethodInfo class with an RTTR method.
        /// </summary>
        /// <param name="rttrMethod">The RTTR method to wrap.</param>
        /// <param name="name">The name of the method.</param>
        MethodInfo(const rttr::method& rttrMethod, const System::String& name);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~MethodInfo() = default;

        /// <summary>
        /// Gets the return type of this method.
        /// </summary>
        /// <returns>A Type object that represents the return type of this method.</returns>
        virtual Type GetReturnType() const;

        /// <summary>
        /// Gets the parameters of the specified method or constructor.
        /// </summary>
        /// <returns>An array of ParameterInfo objects representing the parameters of the method.</returns>
        virtual std::vector<ParameterInfo> GetParameters() const;

        /// <summary>
        /// Gets a value indicating whether the method is static.
        /// </summary>
        /// <returns>true if this method is static; otherwise, false.</returns>
        virtual bool IsStatic() const;

        /// <summary>
        /// Gets a value indicating whether this method is public.
        /// </summary>
        /// <returns>true if this method is public; otherwise, false.</returns>
        virtual bool IsPublic() const;

        /// <summary>
        /// Gets a value indicating whether this method is private.
        /// </summary>
        /// <returns>true if this method is private; otherwise, false.</returns>
        virtual bool IsPrivate() const;

        /// <summary>
        /// Gets a value indicating whether this method is virtual.
        /// </summary>
        /// <returns>true if this method is virtual; otherwise, false.</returns>
        virtual bool IsVirtual() const;

        /// <summary>
        /// Gets the underlying RTTR method.
        /// </summary>
        /// <returns>The RTTR method object.</returns>
        const rttr::method& GetRttrMethod() const { return _rttrMethod; }
    };
}
