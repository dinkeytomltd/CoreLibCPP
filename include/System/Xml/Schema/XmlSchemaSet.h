
#pragma once

#include "../../String.h"
#include "../../Collections/List.h"
#include "../../GC/GCPtr.h"
#include "XmlSchema.h"

namespace System {
namespace Xml {
namespace Schema {

class XmlSchemaSet {
private:
    Collections::List<GCPtr<XmlSchema>> schemas_;
    bool isCompiled_;

public:
    XmlSchemaSet();
    virtual ~XmlSchemaSet() = default;

    // Properties
    int Count() const { return schemas_.Count(); }
    bool IsCompiled() const { return isCompiled_; }

    // Schema management
    GCPtr<XmlSchema> Add(const String& targetNamespace, const String& schemaUri);
    GCPtr<XmlSchema> Add(const String& targetNamespace, GCPtr<XmlReader> schemaDocument);
    GCPtr<XmlSchema> Add(GCPtr<XmlSchema> schema);
    void Remove(GCPtr<XmlSchema> schema);
    void RemoveRecursive(GCPtr<XmlSchema> schemaToRemove);
    bool Contains(const String& targetNamespace) const;
    bool Contains(GCPtr<XmlSchema> schema) const;

    // Compilation
    void Compile();
    void Reprocess(GCPtr<XmlSchema> schema);

    // Access
    Collections::List<GCPtr<XmlSchema>>& Schemas() { return schemas_; }
    const Collections::List<GCPtr<XmlSchema>>& Schemas() const { return schemas_; }

    // Events (simplified - would need event system in full implementation)
    // ValidationEventHandler ValidationEventHandler;
};

} // namespace Schema
} // namespace Xml
} // namespace System
