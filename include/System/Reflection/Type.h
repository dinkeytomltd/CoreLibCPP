
#pragma once

#include "System/Reflection/MemberInfo.h"
#include "System/Reflection/BindingFlags.h"
#include "System/String.h"
#include <vector>
#include <memory>
#include <typeinfo>
#include <rttr/type.h>

namespace System::Reflection
{
    // Forward declarations
    class MethodInfo;
    class PropertyInfo;
    class FieldInfo;
    class ConstructorInfo;
    class Assembly;

    /// <summary>
    /// Represents type declarations: class types, interface types, array types, value types, enumeration types, type parameters, generic type definitions, and open or closed constructed generic types.
    /// </summary>
    class Type : public MemberInfo
    {
    private:
        rttr::type _rttrType;
        mutable System::String _fullName;
        mutable System::String _namespace;
        mutable bool _fullNameCached = false;
        mutable bool _namespaceCached = false;

    public:
        /// <summary>
        /// Initializes a new instance of the Type class.
        /// </summary>
        Type() = default;

        /// <summary>
        /// Initializes a new instance of the Type class with an RTTR type.
        /// </summary>
        /// <param name="rttrType">The RTTR type to wrap.</param>
        explicit Type(const rttr::type& rttrType);

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~Type() = default;

        // Static factory methods

        /// <summary>
        /// Gets the Type of the current instance.
        /// </summary>
        /// <returns>The exact runtime type of the current instance.</returns>
        static Type GetTypeInfo();

        /// <summary>
        /// Gets the Type of the specified name.
        /// </summary>
        /// <param name="typeName">The assembly-qualified name of the type to get.</param>
        /// <returns>The type with the specified name.</returns>
        static Type GetType(const System::String& typeName);

        /// <summary>
        /// Gets the Type of the specified template type.
        /// </summary>
        /// <typeparam name="T">The type to get the Type object for.</typeparam>
        /// <returns>The Type object that represents the specified type.</returns>
        template<typename T>
        static Type GetType()
        {
            return Type(rttr::type::get<T>());
        }

        // Type information properties

        /// <summary>
        /// Gets the name of the current Type.
        /// </summary>
        /// <returns>A String representing the name of the current Type.</returns>
        virtual System::String GetName() const override;

        /// <summary>
        /// Gets the fully qualified name of the Type, including the namespace of the Type.
        /// </summary>
        /// <returns>A String representing the fully qualified name of the Type.</returns>
        virtual System::String GetFullName() const;

        /// <summary>
        /// Gets the namespace of the Type.
        /// </summary>
        /// <returns>A String representing the namespace of the Type.</returns>
        virtual System::String GetNamespace() const;

        /// <summary>
        /// Gets the Assembly in which the type is declared.
        /// </summary>
        /// <returns>An Assembly instance that describes the assembly containing the current type.</returns>
        virtual Assembly GetAssembly() const;

        /// <summary>
        /// Gets the Type from which the current Type directly inherits.
        /// </summary>
        /// <returns>The Type from which the current Type directly inherits, or null if the current Type represents the Object class or an interface.</returns>
        virtual Type GetBaseType() const;

        // Type classification properties

        /// <summary>
        /// Gets a value indicating whether the Type is abstract and must be overridden.
        /// </summary>
        /// <returns>true if the Type is abstract; otherwise, false.</returns>
        virtual bool IsAbstract() const;

        /// <summary>
        /// Gets a value indicating whether the Type is a class or a delegate; that is, not a value type or interface.
        /// </summary>
        /// <returns>true if the Type is a class; otherwise, false.</returns>
        virtual bool IsClass() const;

        /// <summary>
        /// Gets a value indicating whether the current Type represents an enumeration.
        /// </summary>
        /// <returns>true if the current Type represents an enumeration; otherwise, false.</returns>
        virtual bool IsEnum() const;

        /// <summary>
        /// Gets a value indicating whether the Type is an interface; that is, not a class or a value type.
        /// </summary>
        /// <returns>true if the Type is an interface; otherwise, false.</returns>
        virtual bool IsInterface() const;

        /// <summary>
        /// Gets a value indicating whether the Type is a value type.
        /// </summary>
        /// <returns>true if the Type is a value type; otherwise, false.</returns>
        virtual bool IsValueType() const;

        /// <summary>
        /// Gets a value indicating whether the Type is a primitive.
        /// </summary>
        /// <returns>true if the Type is one of the primitive types; otherwise, false.</returns>
        virtual bool IsPrimitive() const;

        /// <summary>
        /// Gets a value indicating whether the Type is public.
        /// </summary>
        /// <returns>true if the Type is declared public and is not a nested type; otherwise, false.</returns>
        virtual bool IsPublic() const;

        /// <summary>
        /// Gets a value indicating whether the Type is not declared public.
        /// </summary>
        /// <returns>true if the Type is not declared public and is not a nested type; otherwise, false.</returns>
        virtual bool IsNotPublic() const;

        /// <summary>
        /// Gets a value indicating whether the Type is sealed.
        /// </summary>
        /// <returns>true if the Type is sealed; otherwise, false.</returns>
        virtual bool IsSealed() const;

        // Generic type support

        /// <summary>
        /// Gets a value indicating whether the current Type represents a generic type definition.
        /// </summary>
        /// <returns>true if the Type object represents a generic type definition; otherwise, false.</returns>
        virtual bool IsGenericTypeDefinition() const;

        /// <summary>
        /// Gets a value indicating whether the current type is a generic type.
        /// </summary>
        /// <returns>true if the current type is a generic type; otherwise, false.</returns>
        virtual bool IsGenericType() const;

        /// <summary>
        /// Gets a value indicating whether the current Type contains generic type parameters.
        /// </summary>
        /// <returns>true if the Type object contains generic type parameters; otherwise, false.</returns>
        virtual bool ContainsGenericParameters() const;

        /// <summary>
        /// Returns an array of Type objects that represent the type arguments of a closed generic type or the type parameters of a generic type definition.
        /// </summary>
        /// <returns>An array of Type objects that represent the type arguments of a generic type.</returns>
        virtual std::vector<Type> GetGenericArguments() const;

        /// <summary>
        /// Returns a Type object that represents a generic type definition from which the current generic type can be constructed.
        /// </summary>
        /// <returns>A Type object representing a generic type from which the current type can be constructed.</returns>
        virtual Type GetGenericTypeDefinition() const;

        /// <summary>
        /// Substitutes the elements of an array of types for the type parameters of the current generic type definition and returns a Type object representing the resulting constructed type.
        /// </summary>
        /// <param name="typeArguments">An array of types to be substituted for the type parameters of the current generic type.</param>
        /// <returns>A Type representing the constructed type formed by substituting the elements of typeArguments for the type parameters of the current generic type.</returns>
        virtual Type MakeGenericType(const std::vector<Type>& typeArguments) const;

        // Member access methods

        /// <summary>
        /// Returns all the public members of the current Type.
        /// </summary>
        /// <returns>An array of MemberInfo objects representing all the public members of the current Type.</returns>
        virtual std::vector<std::shared_ptr<MemberInfo>> GetMembers() const;

        /// <summary>
        /// Searches for the members defined for the current Type, using the specified binding constraints.
        /// </summary>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An array of MemberInfo objects representing all members defined for the current Type that match the specified binding constraints.</returns>
        virtual std::vector<std::shared_ptr<MemberInfo>> GetMembers(BindingFlags bindingAttr) const;

        /// <summary>
        /// Returns all the public methods of the current Type.
        /// </summary>
        /// <returns>An array of MethodInfo objects representing all the public methods defined for the current Type.</returns>
        virtual std::vector<std::shared_ptr<MethodInfo>> GetMethods() const;

        /// <summary>
        /// Searches for the methods defined for the current Type, using the specified binding constraints.
        /// </summary>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An array of MethodInfo objects representing all methods defined for the current Type that match the specified binding constraints.</returns>
        virtual std::vector<std::shared_ptr<MethodInfo>> GetMethods(BindingFlags bindingAttr) const;

        /// <summary>
        /// Searches for the public method with the specified name.
        /// </summary>
        /// <param name="name">The string containing the name of the public method to get.</param>
        /// <returns>An object that represents the public method with the specified name, if found; otherwise, null.</returns>
        virtual std::shared_ptr<MethodInfo> GetMethod(const System::String& name) const;

        /// <summary>
        /// Searches for the specified method, using the specified binding constraints.
        /// </summary>
        /// <param name="name">The string containing the name of the method to get.</param>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An object that represents the method that matches the specified requirements, if found; otherwise, null.</returns>
        virtual std::shared_ptr<MethodInfo> GetMethod(const System::String& name, BindingFlags bindingAttr) const;

        /// <summary>
        /// Returns all the public properties of the current Type.
        /// </summary>
        /// <returns>An array of PropertyInfo objects representing all public properties of the current Type.</returns>
        virtual std::vector<std::shared_ptr<PropertyInfo>> GetProperties() const;

        /// <summary>
        /// Searches for the properties of the current Type, using the specified binding constraints.
        /// </summary>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An array of PropertyInfo objects representing all properties of the current Type that match the specified binding constraints.</returns>
        virtual std::vector<std::shared_ptr<PropertyInfo>> GetProperties(BindingFlags bindingAttr) const;

        /// <summary>
        /// Searches for the public property with the specified name.
        /// </summary>
        /// <param name="name">The string containing the name of the public property to get.</param>
        /// <returns>An object that represents the public property with the specified name, if found; otherwise, null.</returns>
        virtual std::shared_ptr<PropertyInfo> GetProperty(const System::String& name) const;

        /// <summary>
        /// Searches for the specified property, using the specified binding constraints.
        /// </summary>
        /// <param name="name">The string containing the name of the property to get.</param>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An object that represents the property that matches the specified requirements, if found; otherwise, null.</returns>
        virtual std::shared_ptr<PropertyInfo> GetProperty(const System::String& name, BindingFlags bindingAttr) const;

        /// <summary>
        /// Returns all the public fields of the current Type.
        /// </summary>
        /// <returns>An array of FieldInfo objects representing all the public fields defined for the current Type.</returns>
        virtual std::vector<std::shared_ptr<FieldInfo>> GetFields() const;

        /// <summary>
        /// Searches for the fields defined for the current Type, using the specified binding constraints.
        /// </summary>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An array of FieldInfo objects representing all fields defined for the current Type that match the specified binding constraints.</returns>
        virtual std::vector<std::shared_ptr<FieldInfo>> GetFields(BindingFlags bindingAttr) const;

        /// <summary>
        /// Searches for the public field with the specified name.
        /// </summary>
        /// <param name="name">The string containing the name of the data field to get.</param>
        /// <returns>An object that represents the public field with the specified name, if found; otherwise, null.</returns>
        virtual std::shared_ptr<FieldInfo> GetField(const System::String& name) const;

        /// <summary>
        /// Searches for the specified field, using the specified binding constraints.
        /// </summary>
        /// <param name="name">The string containing the name of the data field to get.</param>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An object that represents the field that matches the specified requirements, if found; otherwise, null.</returns>
        virtual std::shared_ptr<FieldInfo> GetField(const System::String& name, BindingFlags bindingAttr) const;

        /// <summary>
        /// Returns all the public constructors defined for the current Type.
        /// </summary>
        /// <returns>An array of ConstructorInfo objects representing all the public instance constructors defined for the current Type.</returns>
        virtual std::vector<std::shared_ptr<ConstructorInfo>> GetConstructors() const;

        /// <summary>
        /// Searches for the constructors defined for the current Type, using the specified BindingFlags.
        /// </summary>
        /// <param name="bindingAttr">A bitmask comprised of one or more BindingFlags that specify how the search is conducted.</param>
        /// <returns>An array of ConstructorInfo objects representing all constructors defined for the current Type that match the specified binding constraints.</returns>
        virtual std::vector<std::shared_ptr<ConstructorInfo>> GetConstructors(BindingFlags bindingAttr) const;

        // Type relationship methods

        /// <summary>
        /// Determines whether the specified object is an instance of the current Type.
        /// </summary>
        /// <param name="o">The object to compare with the current type.</param>
        /// <returns>true if the current Type is in the inheritance hierarchy of the object represented by o, or if the current Type is an interface that o supports.</returns>
        virtual bool IsInstanceOfType(const System::Object& o) const;

        /// <summary>
        /// Determines whether an instance of a specified type can be assigned to the current type instance.
        /// </summary>
        /// <param name="c">The type to compare with the current type.</param>
        /// <returns>true if any of the following conditions is true: c and the current instance represent the same type; c is derived either directly or indirectly from the current instance; the current instance is an interface that c implements; c is a generic type parameter, and the current instance represents one of the constraints of c.</returns>
        virtual bool IsAssignableFrom(const Type& c) const;

        /// <summary>
        /// Determines whether the current Type derives from the specified Type.
        /// </summary>
        /// <param name="c">The type to compare with the current type.</param>
        /// <returns>true if the current Type derives from c; otherwise, false.</returns>
        virtual bool IsSubclassOf(const Type& c) const;

        // Operators

        /// <summary>
        /// Indicates whether two Type objects are equal.
        /// </summary>
        /// <param name="left">The first object to compare.</param>
        /// <param name="right">The second object to compare.</param>
        /// <returns>true if left is equal to right; otherwise, false.</returns>
        friend bool operator==(const Type& left, const Type& right);

        /// <summary>
        /// Indicates whether two Type objects are not equal.
        /// </summary>
        /// <param name="left">The first object to compare.</param>
        /// <param name="right">The second object to compare.</param>
        /// <returns>true if left is not equal to right; otherwise, false.</returns>
        friend bool operator!=(const Type& left, const Type& right);

        /// <summary>
        /// Returns a value that indicates whether this instance is equal to a specified object.
        /// </summary>
        /// <param name="obj">An object to compare with this instance.</param>
        /// <returns>true if obj is a Type and represents the same type as the current instance; otherwise, false.</returns>
        virtual bool Equals(const System::Object& obj) const override;

        /// <summary>
        /// Returns the hash code for this instance.
        /// </summary>
        /// <returns>A 32-bit signed integer hash code.</returns>
        virtual int GetHashCode() const override;

        /// <summary>
        /// Returns a String representing the name of the current Type.
        /// </summary>
        /// <returns>A String representing the name of the current Type.</returns>
        virtual System::String ToString() const override;

        /// <summary>
        /// Gets the underlying RTTR type.
        /// </summary>
        /// <returns>The RTTR type object.</returns>
        const rttr::type& GetRttrType() const { return _rttrType; }

        /// <summary>
        /// Checks if this Type instance is valid.
        /// </summary>
        /// <returns>true if the Type is valid; otherwise, false.</returns>
        bool IsValid() const { return _rttrType.is_valid(); }

        // Attribute support methods

        /// <summary>
        /// Returns all custom attributes applied to this type.
        /// </summary>
        /// <param name="inherit">true to search this type's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>An array that contains all the custom attributes applied to this type.</returns>
        virtual std::vector<std::shared_ptr<Attribute>> GetCustomAttributes(bool inherit = false) const override;

        /// <summary>
        /// Returns custom attributes applied to this type and identified by Type.
        /// </summary>
        /// <param name="attributeType">The type of attribute to search for.</param>
        /// <param name="inherit">true to search this type's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>An array of custom attributes applied to this type.</returns>
        virtual std::vector<std::shared_ptr<Attribute>> GetCustomAttributes(const Type& attributeType, bool inherit = false) const override;

        /// <summary>
        /// Returns custom attributes applied to this type and identified by template type.
        /// </summary>
        /// <typeparam name="T">The type of attribute to search for.</typeparam>
        /// <param name="inherit">true to search this type's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>An array of custom attributes applied to this type.</returns>
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
        /// Returns the first custom attribute of the specified type applied to this type.
        /// </summary>
        /// <typeparam name="T">The type of attribute to search for.</typeparam>
        /// <param name="inherit">true to search this type's inheritance chain to find the attribute; otherwise, false.</param>
        /// <returns>The first custom attribute of the specified type, or nullptr if not found.</returns>
        template<typename T>
        std::shared_ptr<T> GetCustomAttribute(bool inherit = false) const
        {
            static_assert(std::is_base_of_v<Attribute, T>, "T must derive from Attribute");
            auto attributes = GetCustomAttributes<T>(inherit);
            return attributes.empty() ? nullptr : attributes[0];
        }

        /// <summary>
        /// Indicates whether one or more attributes of the specified type or of its derived types is applied to this type.
        /// </summary>
        /// <param name="attributeType">The type of custom attribute to search for.</param>
        /// <param name="inherit">true to search this type's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>true if one or more instances of attributeType or any of its derived types is applied to this type; otherwise, false.</returns>
        virtual bool IsDefined(const Type& attributeType, bool inherit = false) const override;

        /// <summary>
        /// Indicates whether one or more attributes of the specified template type or of its derived types is applied to this type.
        /// </summary>
        /// <typeparam name="T">The type of custom attribute to search for.</typeparam>
        /// <param name="inherit">true to search this type's inheritance chain to find the attributes; otherwise, false.</param>
        /// <returns>true if one or more instances of T or any of its derived types is applied to this type; otherwise, false.</returns>
        template<typename T>
        bool IsDefined(bool inherit = false) const
        {
            static_assert(std::is_base_of_v<Attribute, T>, "T must derive from Attribute");
            auto attributes = GetCustomAttributes<T>(inherit);
            return !attributes.empty();
        }
    };

    /// <summary>
    /// Indicates whether two Type objects are equal.
    /// </summary>
    /// <param name="left">The first object to compare.</param>
    /// <param name="right">The second object to compare.</param>
    /// <returns>true if left is equal to right; otherwise, false.</returns>
    bool operator==(const Type& left, const Type& right);

    /// <summary>
    /// Indicates whether two Type objects are not equal.
    /// </summary>
    /// <param name="left">The first object to compare.</param>
    /// <param name="right">The second object to compare.</param>
    /// <returns>true if left is not equal to right; otherwise, false.</returns>
    bool operator!=(const Type& left, const Type& right);
}
