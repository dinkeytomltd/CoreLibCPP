
#pragma once

#include "System/Reflection/Type.h"
#include "System/Reflection/GenericTypeCache.hpp"
#include <type_traits>
#include <memory>
#include <string>
#include <typeinfo>

namespace System::Reflection
{
    /// <summary>
    /// Core typeof implementation that provides compile-time type resolution
    /// with efficient caching and seamless RTTR integration.
    /// </summary>
    namespace TypeOf
    {
        // Forward declarations
        template<typename T>
        struct TypeResolver;
        
        template<typename T>
        struct ArrayTypeResolver;
        
        template<typename T>
        struct PointerTypeResolver;
        
        template<typename T>
        struct ReferenceTypeResolver;
        
        template<typename T>
        struct GenericTypeResolver;

        /// <summary>
        /// Type traits for detecting various type categories
        /// </summary>
        template<typename T>
        struct TypeTraits
        {
            static constexpr bool is_array = std::is_array_v<T>;
            static constexpr bool is_pointer = std::is_pointer_v<T>;
            static constexpr bool is_reference = std::is_reference_v<T>;
            static constexpr bool is_const = std::is_const_v<std::remove_reference_t<T>>;
            static constexpr bool is_volatile = std::is_volatile_v<std::remove_reference_t<T>>;
            static constexpr bool is_function = std::is_function_v<T>;
            static constexpr bool is_member_function = std::is_member_function_pointer_v<T>;
            static constexpr bool is_void = std::is_void_v<T>;
            static constexpr bool is_nullable = false; // Will be specialized for Nullable<T>
            
            using base_type = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<T>>>;
            using clean_type = std::remove_cv_t<std::remove_reference_t<T>>;
        };

        /// <summary>
        /// Specialization for detecting nullable types
        /// </summary>
        template<typename T>
        struct TypeTraits<System::Nullable<T>>
        {
            static constexpr bool is_nullable = true;
            using underlying_type = T;
            using base_type = T;
            using clean_type = System::Nullable<T>;
        };

        /// <summary>
        /// Core type resolution implementation
        /// </summary>
        template<typename T>
        struct TypeResolver
        {
            static Type Resolve()
            {
                // Use compile-time type hashing for cache lookup
                constexpr auto type_hash = std::hash<std::string>{}(typeid(T).name());
                
                // Check cache first
                if (auto cached = GenericTypeCache::Instance().GetCachedType<T>())
                {
                    return *cached;
                }
                
                // Resolve based on type category
                Type result;
                if constexpr (TypeTraits<T>::is_array)
                {
                    result = ArrayTypeResolver<T>::Resolve();
                }
                else if constexpr (TypeTraits<T>::is_pointer)
                {
                    result = PointerTypeResolver<T>::Resolve();
                }
                else if constexpr (TypeTraits<T>::is_reference)
                {
                    result = ReferenceTypeResolver<T>::Resolve();
                }
                else if constexpr (TypeTraits<T>::is_nullable)
                {
                    result = ResolveNullableType<T>();
                }
                else
                {
                    // Standard type resolution through RTTR
                    result = Type::GetType<T>();
                }
                
                // Cache the result
                GenericTypeCache::Instance().CacheType<T>(result);
                return result;
            }
            
        private:
            template<typename U>
            static Type ResolveNullableType()
            {
                static_assert(TypeTraits<U>::is_nullable, "Type must be nullable");
                
                // Create nullable type representation
                auto underlyingType = TypeResolver<typename TypeTraits<U>::underlying_type>::Resolve();
                return CreateNullableType(underlyingType);
            }
            
        };

        // Forward declarations for helper functions
        Type CreateNullableType(const Type& underlyingType);
        Type CreateArrayType(const Type& elementType, size_t rank);
        Type CreatePointerType(const Type& pointeeType);
        Type CreateReferenceType(const Type& referencedType, bool isLValueRef);

        /// <summary>
        /// Array type resolution
        /// </summary>
        template<typename T>
        struct ArrayTypeResolver
        {
            static Type Resolve()
            {
                using ElementType = std::remove_extent_t<T>;
                constexpr size_t rank = GetArrayRank<T>();
                
                auto elementType = TypeResolver<ElementType>::Resolve();
                return CreateArrayType(elementType, rank);
            }
            
        private:
            template<typename U>
            static constexpr size_t GetArrayRank()
            {
                if constexpr (std::is_array_v<U>)
                {
                    return 1 + GetArrayRank<std::remove_extent_t<U>>();
                }
                return 0;
            }
        };

        /// <summary>
        /// Pointer type resolution
        /// </summary>
        template<typename T>
        struct PointerTypeResolver
        {
            static Type Resolve()
            {
                using PointeeType = std::remove_pointer_t<T>;
                auto pointeeType = TypeResolver<PointeeType>::Resolve();
                return CreatePointerType(pointeeType);
            }
        };

        /// <summary>
        /// Reference type resolution
        /// </summary>
        template<typename T>
        struct ReferenceTypeResolver
        {
            static Type Resolve()
            {
                using ReferencedType = std::remove_reference_t<T>;
                auto referencedType = TypeResolver<ReferencedType>::Resolve();
                return CreateReferenceType(referencedType, std::is_lvalue_reference_v<T>);
            }
        };

        /// <summary>
        /// Generic type detection and resolution
        /// </summary>
        template<typename T>
        struct GenericTypeResolver
        {
            static constexpr bool IsGenericType()
            {
                return IsTemplateSpecialization<T>();
            }
            
            static Type ResolveGenericType()
            {
                if constexpr (IsGenericType())
                {
                    return ResolveTemplateSpecialization<T>();
                }
                else
                {
                    return TypeResolver<T>::Resolve();
                }
            }
            
        private:
            template<typename U>
            static constexpr bool IsTemplateSpecialization()
            {
                // This would need template metaprogramming to detect template specializations
                // For now, we'll use a simpler approach
                return false; // Will be enhanced with specific template detection
            }
            
            template<typename U>
            static Type ResolveTemplateSpecialization()
            {
                // Template specialization resolution logic
                return Type::GetType<U>();
            }
        };

        /// <summary>
        /// Expression type deduction for typeof(expression)
        /// </summary>
        template<typename T>
        constexpr Type DeduceExpressionType(T&& expr)
        {
            using ExprType = std::decay_t<decltype(expr)>;
            return TypeResolver<ExprType>::Resolve();
        }

        /// <summary>
        /// Compile-time type name generation
        /// </summary>
        template<typename T>
        constexpr const char* GetTypeName()
        {
            return typeid(T).name();
        }

        /// <summary>
        /// Type equality checking at compile time
        /// </summary>
        template<typename T, typename U>
        constexpr bool IsSameType()
        {
            return std::is_same_v<T, U>;
        }

        /// <summary>
        /// Check if type is generic at compile time
        /// </summary>
        template<typename T>
        constexpr bool IsGenericType()
        {
            return GenericTypeResolver<T>::IsGenericType();
        }
    }

    /// <summary>
    /// Main typeof function - template-based type resolution
    /// </summary>
    template<typename T>
    constexpr Type typeof()
    {
        return TypeOf::TypeResolver<T>::Resolve();
    }

    /// <summary>
    /// typeof for expressions - deduces type from expression
    /// </summary>
    template<typename T>
    constexpr Type typeof(T&& expr)
    {
        return TypeOf::DeduceExpressionType(std::forward<T>(expr));
    }

    /// <summary>
    /// typeof for type names (compile-time string)
    /// </summary>
    template<const char* TypeName>
    Type typeof()
    {
        return Type::GetType(System::String(TypeName));
    }

    /// <summary>
    /// Runtime typeof for string type names
    /// </summary>
    inline Type typeof(const System::String& typeName)
    {
        return Type::GetType(typeName);
    }

    /// <summary>
    /// Runtime typeof for C-style string type names
    /// </summary>
    inline Type typeof(const char* typeName)
    {
        return Type::GetType(System::String(typeName));
    }

    /// <summary>
    /// typeof for objects - gets runtime type
    /// </summary>
    template<typename T>
    Type typeof(const T& obj)
    {
        if constexpr (std::is_base_of_v<System::Object, T>)
        {
            // Use virtual GetType() method for polymorphic objects
            return obj.GetType();
        }
        else
        {
            // Use compile-time type resolution for non-Object types
            return typeof<T>();
        }
    }

    /// <summary>
    /// Specialized typeof for System::Object and derived classes
    /// </summary>
    inline Type typeof(const System::Object& obj)
    {
        return obj.GetType();
    }

    /// <summary>
    /// typeof for shared_ptr objects
    /// </summary>
    template<typename T>
    Type typeof(const std::shared_ptr<T>& ptr)
    {
        if (ptr)
        {
            return typeof(*ptr);
        }
        return typeof<T>();
    }

    /// <summary>
    /// typeof for unique_ptr objects
    /// </summary>
    template<typename T>
    Type typeof(const std::unique_ptr<T>& ptr)
    {
        if (ptr)
        {
            return typeof(*ptr);
        }
        return typeof<T>();
    }

    /// <summary>
    /// Macro for typeof with type names (for C# compatibility)
    /// Usage: TYPEOF(int), TYPEOF(List<String>)
    /// </summary>
    #define TYPEOF(TypeName) (System::Reflection::typeof<TypeName>())

    /// <summary>
    /// Macro for typeof with expressions
    /// Usage: TYPEOF_EXPR(variable), TYPEOF_EXPR(obj.Property)
    /// </summary>
    #define TYPEOF_EXPR(expr) (System::Reflection::typeof(expr))

    /// <summary>
    /// Compile-time typeof that can be used in constexpr contexts
    /// </summary>
    template<typename T>
    constexpr auto constexpr_typeof()
    {
        return TypeOf::TypeResolver<T>{};
    }

    /// <summary>
    /// Type comparison utilities
    /// </summary>
    namespace TypeComparison
    {
        template<typename T, typename U>
        constexpr bool IsSameType()
        {
            return std::is_same_v<T, U>;
        }

        template<typename T, typename U>
        constexpr bool IsConvertible()
        {
            return std::is_convertible_v<T, U>;
        }

        template<typename Base, typename Derived>
        constexpr bool IsBaseOf()
        {
            return std::is_base_of_v<Base, Derived>;
        }

        template<typename T>
        constexpr bool IsPointer()
        {
            return std::is_pointer_v<T>;
        }

        template<typename T>
        constexpr bool IsReference()
        {
            return std::is_reference_v<T>;
        }

        template<typename T>
        constexpr bool IsArray()
        {
            return std::is_array_v<T>;
        }
    }
}

/// <summary>
/// Global typeof function for convenience (C# style)
/// </summary>
template<typename T>
constexpr System::Reflection::Type typeof()
{
    return System::Reflection::typeof<T>();
}

/// <summary>
/// Global typeof function for expressions
/// </summary>
template<typename T>
constexpr System::Reflection::Type typeof(T&& expr)
{
    return System::Reflection::typeof(std::forward<T>(expr));
}
