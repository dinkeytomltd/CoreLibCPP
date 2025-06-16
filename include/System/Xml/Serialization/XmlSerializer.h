
#pragma once

#include "../../String.h"
#include "../../IO/Stream.h"
#include "../../GC/GCPtr.h"
#include "../XmlReader.h"
#include "../XmlWriter.h"
#include "../../Reflection/Type.h"

namespace System {
namespace Xml {
namespace Serialization {

class XmlSerializerNamespaces;

class XmlSerializer {
private:
    GCPtr<Reflection::Type> type_;
    String defaultNamespace_;

public:
    XmlSerializer(GCPtr<Reflection::Type> type);
    XmlSerializer(GCPtr<Reflection::Type> type, const String& defaultNamespace);
    virtual ~XmlSerializer() = default;

    // Serialization
    void Serialize(GCPtr<XmlWriter> xmlWriter, GCPtr<void> o);
    void Serialize(GCPtr<XmlWriter> xmlWriter, GCPtr<void> o, GCPtr<XmlSerializerNamespaces> namespaces);
    void Serialize(GCPtr<IO::Stream> stream, GCPtr<void> o);
    void Serialize(GCPtr<IO::Stream> stream, GCPtr<void> o, GCPtr<XmlSerializerNamespaces> namespaces);

    // Deserialization
    GCPtr<void> Deserialize(GCPtr<XmlReader> xmlReader);
    GCPtr<void> Deserialize(GCPtr<IO::Stream> stream);

    // Properties
    GCPtr<Reflection::Type> GetType() const { return type_; }
    String DefaultNamespace() const { return defaultNamespace_; }

private:
    void SerializeObject(GCPtr<XmlWriter> writer, GCPtr<void> obj, const String& elementName);
    GCPtr<void> DeserializeObject(GCPtr<XmlReader> reader);
};

class XmlSerializerNamespaces {
private:
    std::map<String, String> namespaces_;

public:
    XmlSerializerNamespaces();
    virtual ~XmlSerializerNamespaces() = default;

    // Namespace management
    void Add(const String& prefix, const String& ns);
    String GetNamespace(const String& prefix) const;
    bool Contains(const String& prefix) const;
    void Clear();

    // Access
    const std::map<String, String>& GetNamespaces() const { return namespaces_; }
};

} // namespace Serialization
} // namespace Xml
} // namespace System
