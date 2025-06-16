
#pragma once

#include "System/Object.h"
#include "System/String.h"
#include "System/Reflection/MemberTypes.h"
#include "System/Reflection/Attribute.h"
#include <vector>
#include <memory>
#include <rttr/type.h>

namespace System::Reflection
{
    // Forward declarations
    class Type;
    class Module;

    /// <summary>
    /// Obtains information about the attributes of a member and provides access to member metadata.
    /// </summary>
    class MemberInfo : public System::Object
    {
    protected:
        rttr::variant _rttrObject;
        System::String _name;
        MemberTypes _memberType;

    public:
        /// <summary>
        /// Initializes a new instance of the MemberInfo class.
        /// </summary>
        MemberInfo() = default;

        /// <summary>
        /// Initializes a new instance of the MemberInfo class with RTTR object.
        /// </summary>
        /// <param name="rttrObj">The RTTR object to wrap.</param>
        /// <param name="name">The name of the member.</param>
        /// <param name="memberType">The type of the member.</param>
        MemberInfo(const rttr::variant& rttrObj, const System::String& name, MemberTypes memberType);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~MemberInfo() = default;

        /// <summary>
        /// Gets the class that declares this member.
        /// </summary>
        /// <returns>The Type object for the class that declares this member.</returns>
        virtual Type DeclaringType() const;

        /// <summary>
        /// Gets a MemberTypes value indicating the type of the member.
        /// </summary>
        /// <returns>A MemberTypes value indicating the type of member.</returns>
        virtual MemberTypes GetMemberType() const { return _memberType; }

        /// <summary>
        /// Gets the name of the current member.
        /// </summary>
        /// <returns>A String containing the name of this member.</returns>
        virtual System::String GetName() const { return _name; }

        /// <summary>
        /// Gets the class object that was used to obtain this instance of MemberInfo.
        /// </summary>
        /// <returns>The Type object through which this MemberInfo object was obtained.</returns>
        virtual Type ReflectedType() const;

        /// <summary>
        /// Gets the module in which the type that declares the member represented by the current MemberInfo is defined.
        /// </summary>
        /// <returns>The Module in which the type that declares the member represented by the current MemberInfo is defined.</returns>
        virtual Module GetModule() const;

        /// <summary>
        /// When overridden in a derived class, returns an array of all custom attributes applied to this member.
        /// </summary>
        /// <param name="inherit">true to search this member's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>An array that contains all the custom attributes applied to this member.</returns>
        virtual std::vector<std::shared_ptr<Attribute>> GetCustomAttributes(bool inherit = false) const;

        /// <summary>
        /// When overridden in a derived class, returns an array of custom attributes applied to this member and identified by Type.
        /// </summary>
        /// <param name="attributeType">The type of attribute to search for.</param>
        /// <param name="inherit">true to search this member's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>An array of custom attributes applied to this member.</returns>
        virtual std::vector<std::shared_ptr<Attribute>> GetCustomAttributes(const Type& attributeType, bool inherit = false) const;

        /// <summary>
        /// Returns an array of custom attributes applied to this member and identified by template type.
        /// </summary>
        /// <typeparam name="T">The type of attribute to search for.</typeparam>
        /// <param name="inherit">true to search this member's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>An array of custom attributes applied to this member.</returns>
        template<typename T>
        std::vector<std::shared_ptr<T>> GetCustomAttributes(bool inherit = false) const
        {
            static_assert(std::is_base_of_v<Attribute, T>, "T must derive from Attribute");
            auto allAttributes = GetCustomAttributes(inherit);
            std::vector<std::shared_ptr<T>> result;
            
            for (const auto& attr : allAttributes)
            {
                if (auto typedAttr = std::dynamic_pointer_cast<T>(attr))
                {
                    result.push_back(typedAttr);
                }
            }
            
            return result;
        }

        /// <summary>
        /// When overridden in a derived class, indicates whether one or more attributes of the specified type or of its derived types is applied to this member.
        /// </summary>
        /// <param name="attributeType">The type of custom attribute to search for.</param>
        /// <param name="inherit">true to search this member's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>true if one or more instances of attributeType or any of its derived types is applied to this member; otherwise, false.</returns>
        virtual bool IsDefined(const Type& attributeType, bool inherit = false) const;

        /// <summary>
        /// Indicates whether one or more attributes of the specified template type or of its derived types is applied to this member.
        /// </summary>
        /// <typeparam name="T">The type of custom attribute to search for.</typeparam>
        /// <param name="inherit">true to search this member's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>true if one or more instances of T or any of its derived types is applied to this member; otherwise, false.</returns>
        template<typename T>
        bool IsDefined(bool inherit = false) const
        {
            static_assert(std::is_base_of_v<Attribute, T>, "T must derive from Attribute");
            auto attributes = GetCustomAttributes<T>(inherit);
            return !attributes.empty();
        }

        /// <summary>
        /// Returns a value that indicates whether this instance is equal to a specified object.
        /// </summary>
        /// <param name="obj">An object to compare with this instance.</param>
        /// <returns>true if obj equals the type and value of this instance; otherwise, false.</returns>
        virtual bool Equals(const System::Object& obj) const override;

        /// <summary>
        /// Returns the hash code for this instance.
        /// </summary>
        /// <returns>A 32-bit signed integer hash code.</returns>
        virtual int GetHashCode() const override;

        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        /// <returns>A string that represents the current object.</returns>
        virtual System::String ToString() const override;

    protected:
        /// <summary>
        /// Gets the underlying RTTR object.
        /// </summary>
        /// <returns>The RTTR variant object.</returns>
        const rttr::variant& GetRttrObject() const { return _rttrObject; }
    };
}
