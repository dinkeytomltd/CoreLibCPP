
#pragma once

#include "../../String.h"
#include "../../Collections/List.h"
#include "../../GC/GCPtr.h"

namespace System {
namespace Xml {
namespace Schema {

class XmlSchemaElement;
class XmlSchemaAttribute;
class XmlSchemaType;

class XmlSchema {
private:
    String targetNamespace_;
    String version_;
    Collections::List<GCPtr<XmlSchemaElement>> elements_;
    Collections::List<GCPtr<XmlSchemaAttribute>> attributes_;
    Collections::List<GCPtr<XmlSchemaType>> types_;

public:
    XmlSchema();
    virtual ~XmlSchema() = default;

    // Properties
    String TargetNamespace() const { return targetNamespace_; }
    void SetTargetNamespace(const String& value) { targetNamespace_ = value; }

    String Version() const { return version_; }
    void SetVersion(const String& value) { version_ = value; }

    Collections::List<GCPtr<XmlSchemaElement>>& Elements() { return elements_; }
    const Collections::List<GCPtr<XmlSchemaElement>>& Elements() const { return elements_; }

    Collections::List<GCPtr<XmlSchemaAttribute>>& Attributes() { return attributes_; }
    const Collections::List<GCPtr<XmlSchemaAttribute>>& Attributes() const { return attributes_; }

    Collections::List<GCPtr<XmlSchemaType>>& Types() { return types_; }
    const Collections::List<GCPtr<XmlSchemaType>>& Types() const { return types_; }

    // Loading
    static GCPtr<XmlSchema> Read(GCPtr<XmlReader> reader);
    static GCPtr<XmlSchema> Read(GCPtr<IO::Stream> stream);

    // Writing
    void Write(GCPtr<XmlWriter> writer);
    void Write(GCPtr<IO::Stream> stream);

    // Compilation
    void Compile();
    bool IsCompiled() const;
};

// Schema element
class XmlSchemaElement {
private:
    String name_;
    String type_;
    bool isNillable_;
    int minOccurs_;
    int maxOccurs_;

public:
    XmlSchemaElement();
    virtual ~XmlSchemaElement() = default;

    String Name() const { return name_; }
    void SetName(const String& value) { name_ = value; }

    String Type() const { return type_; }
    void SetType(const String& value) { type_ = value; }

    bool IsNillable() const { return isNillable_; }
    void SetIsNillable(bool value) { isNillable_ = value; }

    int MinOccurs() const { return minOccurs_; }
    void SetMinOccurs(int value) { minOccurs_ = value; }

    int MaxOccurs() const { return maxOccurs_; }
    void SetMaxOccurs(int value) { maxOccurs_ = value; }
};

// Schema attribute
class XmlSchemaAttribute {
private:
    String name_;
    String type_;
    bool isRequired_;
    String defaultValue_;

public:
    XmlSchemaAttribute();
    virtual ~XmlSchemaAttribute() = default;

    String Name() const { return name_; }
    void SetName(const String& value) { name_ = value; }

    String Type() const { return type_; }
    void SetType(const String& value) { type_ = value; }

    bool IsRequired() const { return isRequired_; }
    void SetIsRequired(bool value) { isRequired_ = value; }

    String DefaultValue() const { return defaultValue_; }
    void SetDefaultValue(const String& value) { defaultValue_ = value; }
};

// Schema type
class XmlSchemaType {
private:
    String name_;
    String baseType_;

public:
    XmlSchemaType();
    virtual ~XmlSchemaType() = default;

    String Name() const { return name_; }
    void SetName(const String& value) { name_ = value; }

    String BaseType() const { return baseType_; }
    void SetBaseType(const String& value) { baseType_ = value; }
};

} // namespace Schema
} // namespace Xml
} // namespace System
