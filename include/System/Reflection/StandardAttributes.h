
#pragma once

#include "System/Reflection/Attribute.h"
#include "System/Reflection/AttributeMacros.h"
#include "System/String.h"

namespace System::Reflection
{
    /// <summary>
    /// Specifies that a field of a serializable class should be required.
    /// </summary>
    class RequiredAttribute : public Attribute
    {
    public:
        RequiredAttribute() = default;
        virtual ~RequiredAttribute() = default;
    };

    /// <summary>
    /// Specifies the minimum and maximum length of characters that are allowed in a data field.
    /// </summary>
    class StringLengthAttribute : public Attribute
    {
    private:
        int _maximumLength;
        int _minimumLength;
        System::String _errorMessage;

    public:
        explicit StringLengthAttribute(int maximumLength)
            : _maximumLength(maximumLength), _minimumLength(0) {}

        int MaximumLength() const { return _maximumLength; }
        
        int MinimumLength() const { return _minimumLength; }
        void SetMinimumLength(int value) { _minimumLength = value; }
        
        System::String ErrorMessage() const { return _errorMessage; }
        void SetErrorMessage(const System::String& value) { _errorMessage = value; }
    };

    /// <summary>
    /// Specifies the numeric range constraints for the value of a data field.
    /// </summary>
    class RangeAttribute : public Attribute
    {
    private:
        double _minimum;
        double _maximum;
        System::String _errorMessage;

    public:
        RangeAttribute(double minimum, double maximum)
            : _minimum(minimum), _maximum(maximum) {}

        double Minimum() const { return _minimum; }
        double Maximum() const { return _maximum; }
        
        System::String ErrorMessage() const { return _errorMessage; }
        void SetErrorMessage(const System::String& value) { _errorMessage = value; }
    };

    /// <summary>
    /// Specifies the display name for a property, event, or public void method.
    /// </summary>
    class DisplayNameAttribute : public Attribute
    {
    private:
        System::String _displayName;

    public:
        explicit DisplayNameAttribute(const System::String& displayName)
            : _displayName(displayName) {}

        System::String DisplayName() const { return _displayName; }
    };

    /// <summary>
    /// Specifies a description for a property or event.
    /// </summary>
    class DescriptionAttribute : public Attribute
    {
    private:
        System::String _description;

    public:
        explicit DescriptionAttribute(const System::String& description)
            : _description(description) {}

        System::String Description() const { return _description; }
    };

    /// <summary>
    /// Specifies the default value for a property.
    /// </summary>
    class DefaultValueAttribute : public Attribute
    {
    private:
        System::Object _value;

    public:
        template<typename T>
        explicit DefaultValueAttribute(const T& value) : _value(value) {}

        System::Object Value() const { return _value; }
    };

    /// <summary>
    /// Indicates the name of the JSON property that corresponds to a class property.
    /// </summary>
    class JsonPropertyAttribute : public Attribute
    {
    private:
        System::String _propertyName;
        bool _required;

    public:
        explicit JsonPropertyAttribute(const System::String& propertyName)
            : _propertyName(propertyName), _required(false) {}

        System::String PropertyName() const { return _propertyName; }
        
        bool Required() const { return _required; }
        void SetRequired(bool value) { _required = value; }
    };

    /// <summary>
    /// Controls XML serialization of the attribute target as an XML element.
    /// </summary>
    class XmlElementAttribute : public Attribute
    {
    private:
        System::String _elementName;
        System::String _namespace;

    public:
        XmlElementAttribute() = default;
        
        explicit XmlElementAttribute(const System::String& elementName)
            : _elementName(elementName) {}

        System::String ElementName() const { return _elementName; }
        void SetElementName(const System::String& value) { _elementName = value; }
        
        System::String Namespace() const { return _namespace; }
        void SetNamespace(const System::String& value) { _namespace = value; }
    };

    /// <summary>
    /// Identifies an action that supports the HTTP GET method.
    /// </summary>
    class HttpGetAttribute : public Attribute
    {
    private:
        System::String _template;

    public:
        HttpGetAttribute() = default;
        
        explicit HttpGetAttribute(const System::String& template)
            : _template(template) {}

        System::String Template() const { return _template; }
    };

    /// <summary>
    /// Identifies an action that supports the HTTP POST method.
    /// </summary>
    class HttpPostAttribute : public Attribute
    {
    private:
        System::String _template;

    public:
        HttpPostAttribute() = default;
        
        explicit HttpPostAttribute(const System::String& template)
            : _template(template) {}

        System::String Template() const { return _template; }
    };

    /// <summary>
    /// Specifies the route template for an action.
    /// </summary>
    class RouteAttribute : public Attribute
    {
    private:
        System::String _template;
        int _order;

    public:
        explicit RouteAttribute(const System::String& template)
            : _template(template), _order(0) {}

        System::String Template() const { return _template; }
        
        int Order() const { return _order; }
        void SetOrder(int value) { _order = value; }
    };

    /// <summary>
    /// Specifies that the class or method that this attribute is applied to requires authorization.
    /// </summary>
    class AuthorizeAttribute : public Attribute
    {
    private:
        System::String _policy;
        System::String _roles;

    public:
        AuthorizeAttribute() = default;
        
        explicit AuthorizeAttribute(const System::String& policy)
            : _policy(policy) {}

        System::String Policy() const { return _policy; }
        void SetPolicy(const System::String& value) { _policy = value; }
        
        System::String Roles() const { return _roles; }
        void SetRoles(const System::String& value) { _roles = value; }
    };

    /// <summary>
    /// Specifies the database table that a class is mapped to.
    /// </summary>
    class TableAttribute : public Attribute
    {
    private:
        System::String _name;
        System::String _schema;

    public:
        explicit TableAttribute(const System::String& name)
            : _name(name) {}

        System::String Name() const { return _name; }
        
        System::String Schema() const { return _schema; }
        void SetSchema(const System::String& value) { _schema = value; }
    };

    /// <summary>
    /// Specifies the database column that a property is mapped to.
    /// </summary>
    class ColumnAttribute : public Attribute
    {
    private:
        System::String _name;
        System::String _typeName;
        int _order;

    public:
        ColumnAttribute() : _order(-1) {}
        
        explicit ColumnAttribute(const System::String& name)
            : _name(name), _order(-1) {}

        System::String Name() const { return _name; }
        void SetName(const System::String& value) { _name = value; }
        
        System::String TypeName() const { return _typeName; }
        void SetTypeName(const System::String& value) { _typeName = value; }
        
        int Order() const { return _order; }
        void SetOrder(int value) { _order = value; }
    };

    /// <summary>
    /// Denotes one or more properties that uniquely identify an entity.
    /// </summary>
    class KeyAttribute : public Attribute
    {
    public:
        KeyAttribute() = default;
        virtual ~KeyAttribute() = default;
    };

    /// <summary>
    /// Specifies how the foreign key constraint is formed when there is a relationship between two entities.
    /// </summary>
    class ForeignKeyAttribute : public Attribute
    {
    private:
        System::String _name;

    public:
        explicit ForeignKeyAttribute(const System::String& name)
            : _name(name) {}

        System::String Name() const { return _name; }
    };
}

// Attribute usage specifications
ATTRIBUTE_USAGE(System::Reflection::RequiredAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::StringLengthAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::RangeAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::DisplayNameAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Method | System::Reflection::AttributeTargets::Event, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::DescriptionAttribute, 
    System::Reflection::AttributeTargets::All, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::DefaultValueAttribute, 
    System::Reflection::AttributeTargets::Property, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::JsonPropertyAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::XmlElementAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::HttpGetAttribute, 
    System::Reflection::AttributeTargets::Method, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::HttpPostAttribute, 
    System::Reflection::AttributeTargets::Method, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::RouteAttribute, 
    System::Reflection::AttributeTargets::Class | System::Reflection::AttributeTargets::Method, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::AuthorizeAttribute, 
    System::Reflection::AttributeTargets::Class | System::Reflection::AttributeTargets::Method, 
    true, true)

ATTRIBUTE_USAGE(System::Reflection::TableAttribute, 
    System::Reflection::AttributeTargets::Class, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::ColumnAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::KeyAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)

ATTRIBUTE_USAGE(System::Reflection::ForeignKeyAttribute, 
    System::Reflection::AttributeTargets::Property | System::Reflection::AttributeTargets::Field, 
    false, true)
