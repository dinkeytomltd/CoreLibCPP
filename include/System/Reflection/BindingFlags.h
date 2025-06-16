
#pragma once

namespace System::Reflection
{
    /// <summary>
    /// Specifies flags that control binding and the way in which the search for members and types is conducted by reflection.
    /// </summary>
    enum class BindingFlags
    {
        /// <summary>
        /// Specifies no binding flag.
        /// </summary>
        Default = 0,

        /// <summary>
        /// Specifies that the case of the member name should not be considered when binding.
        /// </summary>
        IgnoreCase = 1,

        /// <summary>
        /// Specifies that only members declared at the level of the supplied type's hierarchy should be considered.
        /// </summary>
        DeclaredOnly = 2,

        /// <summary>
        /// Specifies that instance members are to be included in the search.
        /// </summary>
        Instance = 4,

        /// <summary>
        /// Specifies that static members are to be included in the search.
        /// </summary>
        Static = 8,

        /// <summary>
        /// Specifies that public members are to be included in the search.
        /// </summary>
        Public = 16,

        /// <summary>
        /// Specifies that non-public members are to be included in the search.
        /// </summary>
        NonPublic = 32,

        /// <summary>
        /// Specifies that public and protected static members up the hierarchy should be returned.
        /// </summary>
        FlattenHierarchy = 64,

        /// <summary>
        /// Specifies that a method is to be invoked.
        /// </summary>
        InvokeMethod = 256,

        /// <summary>
        /// Specifies that Reflection should create an instance of the specified type.
        /// </summary>
        CreateInstance = 512,

        /// <summary>
        /// Specifies that the value of the specified field should be returned.
        /// </summary>
        GetField = 1024,

        /// <summary>
        /// Specifies that the value of the specified field should be set.
        /// </summary>
        SetField = 2048,

        /// <summary>
        /// Specifies that the value of the specified property should be returned.
        /// </summary>
        GetProperty = 4096,

        /// <summary>
        /// Specifies that the value of the specified property should be set.
        /// </summary>
        SetProperty = 8192,

        /// <summary>
        /// Specifies that the PROPPUT member on a COM object should be invoked.
        /// </summary>
        PutDispProperty = 16384,

        /// <summary>
        /// Specifies that the PROPPUTREF member on a COM object should be invoked.
        /// </summary>
        PutRefDispProperty = 32768,

        /// <summary>
        /// Specifies that types should be returned exactly as they are declared.
        /// </summary>
        ExactBinding = 65536,

        /// <summary>
        /// Specifies that a COM object should be invoked.
        /// </summary>
        SuppressChangeType = 131072,

        /// <summary>
        /// Specifies that the return value of the member should be ignored.
        /// </summary>
        OptionalParamBinding = 262144,

        /// <summary>
        /// Used in COM interop to specify that the return value of the member can be ignored.
        /// </summary>
        IgnoreReturn = 16777216
    };

    /// <summary>
    /// Bitwise OR operator for BindingFlags.
    /// </summary>
    inline BindingFlags operator|(BindingFlags left, BindingFlags right)
    {
        return static_cast<BindingFlags>(static_cast<int>(left) | static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise AND operator for BindingFlags.
    /// </summary>
    inline BindingFlags operator&(BindingFlags left, BindingFlags right)
    {
        return static_cast<BindingFlags>(static_cast<int>(left) & static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise XOR operator for BindingFlags.
    /// </summary>
    inline BindingFlags operator^(BindingFlags left, BindingFlags right)
    {
        return static_cast<BindingFlags>(static_cast<int>(left) ^ static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise NOT operator for BindingFlags.
    /// </summary>
    inline BindingFlags operator~(BindingFlags value)
    {
        return static_cast<BindingFlags>(~static_cast<int>(value));
    }
}
