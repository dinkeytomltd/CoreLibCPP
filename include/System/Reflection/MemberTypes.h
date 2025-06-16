
#pragma once

namespace System::Reflection
{
    /// <summary>
    /// Marks each type of member that is defined as a derived class of MemberInfo.
    /// </summary>
    enum class MemberTypes
    {
        /// <summary>
        /// Specifies that the member is a constructor.
        /// </summary>
        Constructor = 1,

        /// <summary>
        /// Specifies that the member is an event.
        /// </summary>
        Event = 2,

        /// <summary>
        /// Specifies that the member is a field.
        /// </summary>
        Field = 4,

        /// <summary>
        /// Specifies that the member is a method.
        /// </summary>
        Method = 8,

        /// <summary>
        /// Specifies that the member is a property.
        /// </summary>
        Property = 16,

        /// <summary>
        /// Specifies that the member is a type.
        /// </summary>
        TypeInfo = 32,

        /// <summary>
        /// Specifies that the member is a custom member type.
        /// </summary>
        Custom = 64,

        /// <summary>
        /// Specifies that the member is a nested type.
        /// </summary>
        NestedType = 128,

        /// <summary>
        /// Specifies all member types.
        /// </summary>
        All = Constructor | Event | Field | Method | Property | TypeInfo | Custom | NestedType
    };

    /// <summary>
    /// Bitwise OR operator for MemberTypes.
    /// </summary>
    inline MemberTypes operator|(MemberTypes left, MemberTypes right)
    {
        return static_cast<MemberTypes>(static_cast<int>(left) | static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise AND operator for MemberTypes.
    /// </summary>
    inline MemberTypes operator&(MemberTypes left, MemberTypes right)
    {
        return static_cast<MemberTypes>(static_cast<int>(left) & static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise XOR operator for MemberTypes.
    /// </summary>
    inline MemberTypes operator^(MemberTypes left, MemberTypes right)
    {
        return static_cast<MemberTypes>(static_cast<int>(left) ^ static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise NOT operator for MemberTypes.
    /// </summary>
    inline MemberTypes operator~(MemberTypes value)
    {
        return static_cast<MemberTypes>(~static_cast<int>(value));
    }
}
