
#pragma once

#include "System/Object.h"
#include "System/String.h"
#include <memory>
#include <vector>
#include <typeinfo>
#include <unordered_map>

namespace System::Reflection
{
    /// <summary>
    /// Represents the base class for custom attributes.
    /// </summary>
    class Attribute : public System::Object
    {
    public:
        /// <summary>
        /// Initializes a new instance of the Attribute class.
        /// </summary>
        Attribute() = default;

        /// <summary>
        /// Virtual destructor for proper inheritance.
        /// </summary>
        virtual ~Attribute() = default;

        /// <summary>
        /// When overridden in a derived class, returns a unique identifier for this Attribute.
        /// </summary>
        /// <returns>An Object that is a unique identifier for the attribute.</returns>
        virtual System::Object TypeId() const
        {
            return System::Object();
        }

        /// <summary>
        /// When overridden in a derived class, indicates whether the value of this instance is the default value for the derived class.
        /// </summary>
        /// <returns>true if this instance is the default attribute for the class; otherwise, false.</returns>
        virtual bool IsDefaultAttribute() const
        {
            return false;
        }

        /// <summary>
        /// Returns a value that indicates whether this instance is equal to a specified object.
        /// </summary>
        /// <param name="obj">An Object to compare with this instance or null.</param>
        /// <returns>true if obj equals the type and value of this instance; otherwise, false.</returns>
        virtual bool Equals(const System::Object& obj) const override
        {
            return typeid(*this) == typeid(obj);
        }

        /// <summary>
        /// Returns the hash code for this instance.
        /// </summary>
        /// <returns>A 32-bit signed integer hash code.</returns>
        virtual int GetHashCode() const override
        {
            return typeid(*this).hash_code();
        }

        /// <summary>
        /// When overridden in a derived class, returns a value that indicates whether this instance equals a specified object.
        /// </summary>
        /// <param name="obj">An Object to compare with this instance of Attribute.</param>
        /// <returns>true if this instance equals obj; otherwise, false.</returns>
        virtual bool Match(const System::Object& obj) const
        {
            return Equals(obj);
        }
    };

    /// <summary>
    /// Specifies the application elements on which it is valid to apply an attribute.
    /// </summary>
    enum class AttributeTargets
    {
        /// <summary>
        /// Attribute can be applied to an assembly.
        /// </summary>
        Assembly = 1,

        /// <summary>
        /// Attribute can be applied to a module.
        /// </summary>
        Module = 2,

        /// <summary>
        /// Attribute can be applied to a class.
        /// </summary>
        Class = 4,

        /// <summary>
        /// Attribute can be applied to a structure; that is, a value type.
        /// </summary>
        Struct = 8,

        /// <summary>
        /// Attribute can be applied to an enumeration.
        /// </summary>
        Enum = 16,

        /// <summary>
        /// Attribute can be applied to a constructor.
        /// </summary>
        Constructor = 32,

        /// <summary>
        /// Attribute can be applied to a method.
        /// </summary>
        Method = 64,

        /// <summary>
        /// Attribute can be applied to a property.
        /// </summary>
        Property = 128,

        /// <summary>
        /// Attribute can be applied to a field.
        /// </summary>
        Field = 256,

        /// <summary>
        /// Attribute can be applied to an event.
        /// </summary>
        Event = 512,

        /// <summary>
        /// Attribute can be applied to an interface.
        /// </summary>
        Interface = 1024,

        /// <summary>
        /// Attribute can be applied to a parameter.
        /// </summary>
        Parameter = 2048,

        /// <summary>
        /// Attribute can be applied to a delegate.
        /// </summary>
        Delegate = 4096,

        /// <summary>
        /// Attribute can be applied to a return value.
        /// </summary>
        ReturnValue = 8192,

        /// <summary>
        /// Attribute can be applied to a generic parameter.
        /// </summary>
        GenericParameter = 16384,

        /// <summary>
        /// Attribute can be applied to any application element.
        /// </summary>
        All = Assembly | Module | Class | Struct | Enum | Constructor | Method | Property | Field | Event | Interface | Parameter | Delegate | ReturnValue | GenericParameter
    };

    /// <summary>
    /// Bitwise OR operator for AttributeTargets.
    /// </summary>
    inline AttributeTargets operator|(AttributeTargets left, AttributeTargets right)
    {
        return static_cast<AttributeTargets>(static_cast<int>(left) | static_cast<int>(right));
    }

    /// <summary>
    /// Bitwise AND operator for AttributeTargets.
    /// </summary>
    inline AttributeTargets operator&(AttributeTargets left, AttributeTargets right)
    {
        return static_cast<AttributeTargets>(static_cast<int>(left) & static_cast<int>(right));
    }

    /// <summary>
    /// Specifies the usage of another attribute class.
    /// </summary>
    class AttributeUsageAttribute : public Attribute
    {
    private:
        AttributeTargets _validOn;
        bool _allowMultiple;
        bool _inherited;

    public:
        /// <summary>
        /// Initializes a new instance of the AttributeUsageAttribute class with the specified list of AttributeTargets.
        /// </summary>
        /// <param name="validOn">The set of values combined using a bitwise OR operation to indicate which program elements are valid.</param>
        explicit AttributeUsageAttribute(AttributeTargets validOn)
            : _validOn(validOn), _allowMultiple(false), _inherited(true) {}

        /// <summary>
        /// Gets a set of values identifying which program elements that the indicated attribute can be applied to.
        /// </summary>
        /// <returns>One or several AttributeTargets values.</returns>
        AttributeTargets ValidOn() const { return _validOn; }

        /// <summary>
        /// Gets or sets a Boolean value indicating whether more than one instance of the indicated attribute can be specified for a single program element.
        /// </summary>
        /// <returns>true if more than one instance is allowed to be specified; otherwise, false.</returns>
        bool AllowMultiple() const { return _allowMultiple; }
        void SetAllowMultiple(bool value) { _allowMultiple = value; }

        /// <summary>
        /// Gets or sets a Boolean value indicating whether the indicated attribute can be inherited by derived classes and overriding members.
        /// </summary>
        /// <returns>true if the attribute can be inherited by derived classes and overriding members; otherwise, false.</returns>
        bool Inherited() const { return _inherited; }
        void SetInherited(bool value) { _inherited = value; }
    };

    /// <summary>
    /// Indicates that a class can be serialized.
    /// </summary>
    class SerializableAttribute : public Attribute
    {
    public:
        /// <summary>
        /// Initializes a new instance of the SerializableAttribute class.
        /// </summary>
        SerializableAttribute() = default;
    };

    /// <summary>
    /// Indicates that a field of a serializable class should not be serialized.
    /// </summary>
    class NonSerializedAttribute : public Attribute
    {
    public:
        /// <summary>
        /// Initializes a new instance of the NonSerializedAttribute class.
        /// </summary>
        NonSerializedAttribute() = default;
    };

    /// <summary>
    /// Marks a program element as obsolete.
    /// </summary>
    class ObsoleteAttribute : public Attribute
    {
    private:
        System::String _message;
        bool _isError;

    public:
        /// <summary>
        /// Initializes a new instance of the ObsoleteAttribute class with default properties.
        /// </summary>
        ObsoleteAttribute() : _isError(false) {}

        /// <summary>
        /// Initializes a new instance of the ObsoleteAttribute class with a specified workaround message.
        /// </summary>
        /// <param name="message">The text string that describes alternative workarounds.</param>
        explicit ObsoleteAttribute(const System::String& message) : _message(message), _isError(false) {}

        /// <summary>
        /// Initializes a new instance of the ObsoleteAttribute class with a workaround message and a Boolean value indicating whether the obsolete element usage is considered an error.
        /// </summary>
        /// <param name="message">The text string that describes alternative workarounds.</param>
        /// <param name="error">The Boolean value that indicates whether the obsolete element usage is considered an error.</param>
        ObsoleteAttribute(const System::String& message, bool error) : _message(message), _isError(error) {}

        /// <summary>
        /// Gets the workaround message.
        /// </summary>
        /// <returns>The workaround text string.</returns>
        System::String Message() const { return _message; }

        /// <summary>
        /// Gets a Boolean value indicating whether the compiler will treat usage of the obsolete program element as an error.
        /// </summary>
        /// <returns>true if the obsolete element usage is considered an error; otherwise, false.</returns>
        bool IsError() const { return _isError; }
    };
}
