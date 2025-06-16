
#pragma once

#include <rttr/registration>
#include <rttr/type.h>
#include <memory>

namespace System::Reflection
{
    /// <summary>
    /// Provides macros and utilities for registering types with RTTR for reflection support.
    /// </summary>

    /// <summary>
    /// Macro to register a type with RTTR reflection system.
    /// Usage: REGISTER_TYPE(MyClass)
    ///           .property("PropertyName", &MyClass::PropertyName)
    ///           .method("MethodName", &MyClass::MethodName)
    ///           .constructor<>()
    ///           .constructor<int, std::string>();
    /// </summary>
    #define REGISTER_TYPE(ClassType) \
        rttr::registration::class_<ClassType>(#ClassType)

    /// <summary>
    /// Macro to register a type with a custom name.
    /// Usage: REGISTER_TYPE_AS(MyClass, "CustomName")
    /// </summary>
    #define REGISTER_TYPE_AS(ClassType, CustomName) \
        rttr::registration::class_<ClassType>(CustomName)

    /// <summary>
    /// Macro to register a generic type template.
    /// Usage: REGISTER_GENERIC_TYPE(std::vector, "Vector")
    /// </summary>
    #define REGISTER_GENERIC_TYPE(TemplateType, Name) \
        rttr::registration::class_<TemplateType>(Name)

    /// <summary>
    /// Macro to register an enumeration.
    /// Usage: REGISTER_ENUM(MyEnum)
    ///           .value("Value1", MyEnum::Value1)
    ///           .value("Value2", MyEnum::Value2);
    /// </summary>
    #define REGISTER_ENUM(EnumType) \
        rttr::registration::enumeration<EnumType>(#EnumType)

    /// <summary>
    /// Macro to register a global function.
    /// Usage: REGISTER_GLOBAL_METHOD("FunctionName", &FunctionName)
    /// </summary>
    #define REGISTER_GLOBAL_METHOD(Name, Function) \
        rttr::registration::method(Name, Function)

    /// <summary>
    /// Macro to register a property with getter and setter.
    /// Usage: REGISTER_PROPERTY(MyClass, "PropertyName", &MyClass::GetProperty, &MyClass::SetProperty)
    /// </summary>
    #define REGISTER_PROPERTY(ClassType, Name, Getter, Setter) \
        .property(Name, Getter, Setter)

    /// <summary>
    /// Macro to register a read-only property.
    /// Usage: REGISTER_READONLY_PROPERTY(MyClass, "PropertyName", &MyClass::GetProperty)
    /// </summary>
    #define REGISTER_READONLY_PROPERTY(ClassType, Name, Getter) \
        .property_readonly(Name, Getter)

    /// <summary>
    /// Macro to register a method with automatic overload resolution.
    /// Usage: REGISTER_METHOD("MethodName", rttr::select_overload<ReturnType(Args...)>(&MyClass::Method))
    /// </summary>
    #define REGISTER_METHOD(Name, Method) \
        .method(Name, Method)

    /// <summary>
    /// Macro to register a constructor.
    /// Usage: REGISTER_CONSTRUCTOR() for default constructor
    ///        REGISTER_CONSTRUCTOR(int, std::string) for parameterized constructor
    /// </summary>
    #define REGISTER_CONSTRUCTOR(...) \
        .constructor<__VA_ARGS__>()

    /// <summary>
    /// Macro to register metadata/attributes.
    /// Usage: REGISTER_METADATA("key", value)
    /// </summary>
    #define REGISTER_METADATA(Key, Value) \
        .metadata(Key, Value)

    /// <summary>
    /// Macro to register a custom attribute.
    /// Usage: REGISTER_ATTRIBUTE(AttributeType())
    /// </summary>
    #define REGISTER_ATTRIBUTE(AttributeInstance) \
        .metadata(rttr::metadata(#AttributeInstance, AttributeInstance))

    /// <summary>
    /// Helper class for automatic type registration.
    /// </summary>
    template<typename T>
    class AutoRegister
    {
    public:
        AutoRegister()
        {
            // This will be specialized for each type that needs automatic registration
            static_assert(sizeof(T) == 0, "AutoRegister must be specialized for each type");
        }
    };

    /// <summary>
    /// Macro to create an automatic registrar for a type.
    /// Usage: AUTO_REGISTER_TYPE(MyClass)
    /// This will create a static instance that registers the type at program startup.
    /// </summary>
    #define AUTO_REGISTER_TYPE(ClassType) \
        namespace { \
            struct ClassType##Registrar { \
                ClassType##Registrar(); \
            }; \
            static ClassType##Registrar ClassType##RegistrarInstance; \
        } \
        ClassType##Registrar::ClassType##Registrar()

    /// <summary>
    /// Function to ensure all types are registered.
    /// This should be called at program startup to guarantee registration.
    /// </summary>
    void EnsureTypesRegistered();

    /// <summary>
    /// Function to register core system types.
    /// This registers basic types like String, Object, Int32, etc.
    /// </summary>
    void RegisterCoreTypes();

    /// <summary>
    /// Function to register collection types.
    /// This registers List<T>, Dictionary<K,V>, etc.
    /// </summary>
    void RegisterCollectionTypes();

    /// <summary>
    /// Function to register exception types.
    /// This registers Exception hierarchy.
    /// </summary>
    void RegisterExceptionTypes();

    /// <summary>
    /// Function to register all primitive types.
    /// This registers Int32, Boolean, Double, etc.
    /// </summary>
    void RegisterPrimitiveTypes();

    /// <summary>
    /// Template function to check if a type is registered.
    /// </summary>
    template<typename T>
    bool IsTypeRegistered()
    {
        return rttr::type::get<T>().is_valid();
    }

    /// <summary>
    /// Function to check if a type is registered by name.
    /// </summary>
    bool IsTypeRegistered(const std::string& typeName);

    /// <summary>
    /// Function to get all registered types.
    /// </summary>
    std::vector<rttr::type> GetAllRegisteredTypes();
}

/// <summary>
/// Main registration block for all CoreLib types.
/// This should be included in a source file to register all types.
/// </summary>
#define CORELIB_REGISTER_ALL_TYPES() \
    RTTR_REGISTRATION \
    { \
        System::Reflection::RegisterCoreTypes(); \
        System::Reflection::RegisterPrimitiveTypes(); \
        System::Reflection::RegisterCollectionTypes(); \
        System::Reflection::RegisterExceptionTypes(); \
    }
