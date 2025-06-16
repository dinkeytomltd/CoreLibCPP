
#pragma once

#include "System/Reflection/Type.h"
#include "System/Reflection/typeof.hpp"
#include <vector>
#include <memory>

namespace System::Reflection
{
    /// <summary>
    /// Extensions to the Type class to support typeof operator functionality
    /// </summary>
    class TypeExtensions
    {
    public:
        /// <summary>
        /// Enhanced generic type support for typeof operator
        /// </summary>
        static bool IsGenericType(const Type& type);
        
        /// <summary>
        /// Check if type is a generic type definition (open generic)
        /// </summary>
        static bool IsGenericTypeDefinition(const Type& type);
        
        /// <summary>
        /// Get generic arguments for closed generic types
        /// </summary>
        static std::vector<Type> GetGenericArguments(const Type& type);
        
        /// <summary>
        /// Create a generic type from a generic type definition and type arguments
        /// </summary>
        static Type MakeGenericType(const Type& genericTypeDefinition, const std::vector<Type>& typeArguments);
        
        /// <summary>
        /// Get the generic type definition from a closed generic type
        /// </summary>
        static Type GetGenericTypeDefinition(const Type& type);
        
        /// <summary>
        /// Check if type contains unbound generic parameters
        /// </summary>
        static bool ContainsGenericParameters(const Type& type);
        
        /// <summary>
        /// Array type support
        /// </summary>
        static bool IsArray(const Type& type);
        static Type GetElementType(const Type& arrayType);
        static int GetArrayRank(const Type& arrayType);
        static Type MakeArrayType(const Type& elementType, int rank = 1);
        
        /// <summary>
        /// Pointer type support
        /// </summary>
        static bool IsPointer(const Type& type);
        static Type MakePointerType(const Type& type);
        
        /// <summary>
        /// Reference type support
        /// </summary>
        static bool IsByRef(const Type& type);
        static Type MakeByRefType(const Type& type);
        
        /// <summary>
        /// Nullable type support
        /// </summary>
        static bool IsNullable(const Type& type);
        static Type GetUnderlyingType(const Type& nullableType);
        static Type MakeNullableType(const Type& valueType);
        
        /// <summary>
        /// Enhanced type comparison
        /// </summary>
        static bool IsAssignableFrom(const Type& baseType, const Type& derivedType);
        static bool IsSubclassOf(const Type& derivedType, const Type& baseType);
        static bool IsInstanceOfType(const Type& type, const System::Object& obj);
        
        /// <summary>
        /// Type hierarchy navigation
        /// </summary>
        static std::vector<Type> GetInterfaces(const Type& type);
        static Type GetBaseType(const Type& type);
        static std::vector<Type> GetInheritanceHierarchy(const Type& type);
        
        /// <summary>
        /// Type metadata
        /// </summary>
        static System::String GetAssemblyQualifiedName(const Type& type);
        static System::String GetTypeDisplayName(const Type& type);
        static bool HasDefaultConstructor(const Type& type);
        
        /// <summary>
        /// Type conversion support
        /// </summary>
        static bool CanConvertTo(const Type& fromType, const Type& toType);
        static bool HasImplicitConversion(const Type& fromType, const Type& toType);
        static bool HasExplicitConversion(const Type& fromType, const Type& toType);
        
        /// <summary>
        /// Attribute support for typeof
        /// </summary>
        template<typename TAttribute>
        static std::vector<std::shared_ptr<TAttribute>> GetCustomAttributes(const Type& type, bool inherit = false)
        {
            return type.GetCustomAttributes<TAttribute>(inherit);
        }
        
        template<typename TAttribute>
        static std::shared_ptr<TAttribute> GetCustomAttribute(const Type& type, bool inherit = false)
        {
            return type.GetCustomAttribute<TAttribute>(inherit);
        }
        
        template<typename TAttribute>
        static bool HasAttribute(const Type& type, bool inherit = false)
        {
            return type.IsDefined<TAttribute>(inherit);
        }
    };

    /// <summary>
    /// Type factory for creating types at runtime
    /// </summary>
    class TypeFactory
    {
    public:
        /// <summary>
        /// Create a type from a string representation
        /// </summary>
        static Type CreateType(const System::String& typeName);
        
        /// <summary>
        /// Create a generic type from type name and arguments
        /// </summary>
        static Type CreateGenericType(const System::String& genericTypeName, const std::vector<Type>& typeArguments);
        
        /// <summary>
        /// Create an array type
        /// </summary>
        static Type CreateArrayType(const Type& elementType, int rank = 1);
        
        /// <summary>
        /// Create a pointer type
        /// </summary>
        static Type CreatePointerType(const Type& pointeeType);
        
        /// <summary>
        /// Create a reference type
        /// </summary>
        static Type CreateReferenceType(const Type& referencedType);
        
        /// <summary>
        /// Create a nullable type
        /// </summary>
        static Type CreateNullableType(const Type& valueType);
        
        /// <summary>
        /// Register a custom type for typeof resolution
        /// </summary>
        static void RegisterType(const System::String& typeName, const Type& type);
        
        /// <summary>
        /// Unregister a custom type
        /// </summary>
        static void UnregisterType(const System::String& typeName);
        
        /// <summary>
        /// Check if a type is registered
        /// </summary>
        static bool IsTypeRegistered(const System::String& typeName);
        
        /// <summary>
        /// Get all registered type names
        /// </summary>
        static std::vector<System::String> GetRegisteredTypeNames();
    };

    /// <summary>
    /// Type utilities for advanced typeof scenarios
    /// </summary>
    class TypeUtilities
    {
    public:
        /// <summary>
        /// Get the default value for a type
        /// </summary>
        static System::Object GetDefaultValue(const Type& type);
        
        /// <summary>
        /// Check if a type has a default value
        /// </summary>
        static bool HasDefaultValue(const Type& type);
        
        /// <summary>
        /// Get the size of a type in bytes
        /// </summary>
        static size_t GetTypeSize(const Type& type);
        
        /// <summary>
        /// Get the alignment requirement for a type
        /// </summary>
        static size_t GetTypeAlignment(const Type& type);
        
        /// <summary>
        /// Check if a type is blittable (can be directly copied)
        /// </summary>
        static bool IsBlittable(const Type& type);
        
        /// <summary>
        /// Get the underlying type for enums
        /// </summary>
        static Type GetEnumUnderlyingType(const Type& enumType);
        
        /// <summary>
        /// Check if two types are equivalent (considering generic parameters)
        /// </summary>
        static bool AreTypesEquivalent(const Type& type1, const Type& type2);
        
        /// <summary>
        /// Get the most derived common base type
        /// </summary>
        static Type GetCommonBaseType(const std::vector<Type>& types);
        
        /// <summary>
        /// Type name parsing utilities
        /// </summary>
        static System::String GetSimpleName(const Type& type);
        static System::String GetNamespace(const Type& type);
        static System::String GetAssemblyName(const Type& type);
        
        /// <summary>
        /// Generic type name utilities
        /// </summary>
        static System::String GetGenericTypeName(const Type& type);
        static std::vector<System::String> GetGenericParameterNames(const Type& type);
        
        /// <summary>
        /// Type validation
        /// </summary>
        static bool IsValidType(const Type& type);
        static bool IsCompleteType(const Type& type);
        static bool IsInstantiableType(const Type& type);
    };
}
