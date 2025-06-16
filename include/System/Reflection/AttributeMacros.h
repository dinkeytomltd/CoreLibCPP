
#pragma once

#include "System/Reflection/Attribute.h"
#include "System/Reflection/Type.h"
#include <rttr/registration>
#include <memory>
#include <vector>
#include <type_traits>

namespace System::Reflection
{
    // Forward declarations for attribute storage
    class AttributeStorage;
    
    /// <summary>
    /// Internal attribute storage and management
    /// </summary>
    class AttributeStorage
    {
    private:
        static std::unordered_map<std::string, std::vector<std::shared_ptr<Attribute>>> _typeAttributes;
        static std::unordered_map<std::string, std::vector<std::shared_ptr<Attribute>>> _memberAttributes;
        
    public:
        static void RegisterTypeAttribute(const std::string& typeName, std::shared_ptr<Attribute> attribute);
        static void RegisterMemberAttribute(const std::string& memberName, std::shared_ptr<Attribute> attribute);
        static std::vector<std::shared_ptr<Attribute>> GetTypeAttributes(const std::string& typeName);
        static std::vector<std::shared_ptr<Attribute>> GetMemberAttributes(const std::string& memberName);
        
        template<typename T>
        static std::vector<std::shared_ptr<T>> GetTypeAttributes(const std::string& typeName)
        {
            static_assert(std::is_base_of_v<Attribute, T>, "T must derive from Attribute");
            auto allAttributes = GetTypeAttributes(typeName);
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
        
        template<typename T>
        static std::vector<std::shared_ptr<T>> GetMemberAttributes(const std::string& memberName)
        {
            static_assert(std::is_base_of_v<Attribute, T>, "T must derive from Attribute");
            auto allAttributes = GetMemberAttributes(memberName);
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
    };
}

// Compile-time attribute traits
namespace System::Reflection::Traits
{
    /// <summary>
    /// Compile-time trait to check if a type has a specific attribute
    /// </summary>
    template<typename T, typename AttributeType>
    struct has_attribute : std::false_type {};
    
    /// <summary>
    /// Compile-time trait to get attribute from a type
    /// </summary>
    template<typename T, typename AttributeType>
    struct get_attribute
    {
        static constexpr bool has_value = false;
        using type = void;
    };
    
    /// <summary>
    /// Helper variable template for has_attribute
    /// </summary>
    template<typename T, typename AttributeType>
    constexpr bool has_attribute_v = has_attribute<T, AttributeType>::value;
    
    /// <summary>
    /// Helper type alias for get_attribute
    /// </summary>
    template<typename T, typename AttributeType>
    using get_attribute_t = typename get_attribute<T, AttributeType>::type;
}

// Attribute registration macros
#define CORELIB_INTERNAL_ATTRIBUTE_STORAGE_KEY(type, member) \
    (std::string(#type) + "::" + std::string(#member))

#define CORELIB_INTERNAL_TYPE_ATTRIBUTE_STORAGE_KEY(type) \
    std::string(#type)

// Single attribute macro
#define ATTRIBUTE(AttributeType, ...) \
    []() { \
        static auto attr = std::make_shared<AttributeType>(__VA_ARGS__); \
        return attr; \
    }()

// Multiple attributes macro
#define ATTRIBUTES(...) \
    { __VA_ARGS__ }

// Class-specific attribute macros
#define CLASS_ATTRIBUTES(ClassName, ...) \
    namespace { \
        struct ClassName##_AttributeRegistrar { \
            ClassName##_AttributeRegistrar() { \
                auto attributes = ATTRIBUTES(__VA_ARGS__); \
                for (auto& attr : attributes) { \
                    System::Reflection::AttributeStorage::RegisterTypeAttribute( \
                        CORELIB_INTERNAL_TYPE_ATTRIBUTE_STORAGE_KEY(ClassName), attr); \
                } \
            } \
        }; \
        static ClassName##_AttributeRegistrar ClassName##_attr_reg; \
    }

// Method-specific attribute macros
#define METHOD_ATTRIBUTES(ClassName, MethodName, ...) \
    namespace { \
        struct ClassName##_##MethodName##_AttributeRegistrar { \
            ClassName##_##MethodName##_AttributeRegistrar() { \
                auto attributes = ATTRIBUTES(__VA_ARGS__); \
                for (auto& attr : attributes) { \
                    System::Reflection::AttributeStorage::RegisterMemberAttribute( \
                        CORELIB_INTERNAL_ATTRIBUTE_STORAGE_KEY(ClassName, MethodName), attr); \
                } \
            } \
        }; \
        static ClassName##_##MethodName##_AttributeRegistrar ClassName##_##MethodName##_attr_reg; \
    }

// Property-specific attribute macros
#define PROPERTY_ATTRIBUTES(ClassName, PropertyName, ...) \
    namespace { \
        struct ClassName##_##PropertyName##_AttributeRegistrar { \
            ClassName##_##PropertyName##_AttributeRegistrar() { \
                auto attributes = ATTRIBUTES(__VA_ARGS__); \
                for (auto& attr : attributes) { \
                    System::Reflection::AttributeStorage::RegisterMemberAttribute( \
                        CORELIB_INTERNAL_ATTRIBUTE_STORAGE_KEY(ClassName, PropertyName), attr); \
                } \
            } \
        }; \
        static ClassName##_##PropertyName##_AttributeRegistrar ClassName##_##PropertyName##_attr_reg; \
    }

// Custom attribute declaration macro
#define DECLARE_ATTRIBUTE(AttributeName, BaseAttribute, ...) \
    class AttributeName : public BaseAttribute \
    { \
    public: \
        AttributeName(__VA_ARGS__) {} \
        virtual ~AttributeName() = default; \
    }; \
    \
    namespace System::Reflection::Traits { \
        template<typename T> \
        struct has_attribute<T, AttributeName> : std::false_type {}; \
        \
        template<typename T> \
        struct get_attribute<T, AttributeName> \
        { \
            static constexpr bool has_value = false; \
            using type = void; \
        }; \
    }

// Attribute usage specification macro
#define ATTRIBUTE_USAGE(AttributeName, Targets, AllowMultiple, Inherited) \
    namespace { \
        struct AttributeName##_UsageRegistrar { \
            AttributeName##_UsageRegistrar() { \
                auto usage = std::make_shared<System::Reflection::AttributeUsageAttribute>(Targets); \
                usage->SetAllowMultiple(AllowMultiple); \
                usage->SetInherited(Inherited); \
                System::Reflection::AttributeStorage::RegisterTypeAttribute( \
                    CORELIB_INTERNAL_TYPE_ATTRIBUTE_STORAGE_KEY(AttributeName), usage); \
            } \
        }; \
        static AttributeName##_UsageRegistrar AttributeName##_usage_reg; \
    }

// Compile-time attribute specialization macro
#define SPECIALIZE_ATTRIBUTE(TypeName, AttributeType, ...) \
    namespace System::Reflection::Traits { \
        template<> \
        struct has_attribute<TypeName, AttributeType> : std::true_type {}; \
        \
        template<> \
        struct get_attribute<TypeName, AttributeType> \
        { \
            static constexpr bool has_value = true; \
            using type = AttributeType; \
            static AttributeType get() { return AttributeType(__VA_ARGS__); } \
        }; \
    }

// Runtime attribute checking helpers
#define HAS_ATTRIBUTE(TypeOrInstance, AttributeType) \
    (System::Reflection::Type::GetType<std::decay_t<decltype(TypeOrInstance)>>().IsDefined<AttributeType>())

#define GET_ATTRIBUTE(TypeOrInstance, AttributeType) \
    (System::Reflection::Type::GetType<std::decay_t<decltype(TypeOrInstance)>>().GetCustomAttribute<AttributeType>())

#define GET_ATTRIBUTES(TypeOrInstance, AttributeType) \
    (System::Reflection::Type::GetType<std::decay_t<decltype(TypeOrInstance)>>().GetCustomAttributes<AttributeType>())
