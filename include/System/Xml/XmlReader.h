
#include "MockDependencies.h"
#pragma once

#include "XmlReaderSettings.h"

namespace System {
namespace Xml {

class XmlReader {
protected:
    XmlReader() = default;

public:
    virtual ~XmlReader() = default;

    // Factory methods
    static GCPtr<XmlReader> Create(const String& inputUri);
    static GCPtr<XmlReader> Create(const String& inputUri, GCPtr<XmlReaderSettings> settings);
    static GCPtr<XmlReader> Create(GCPtr<IO::Stream> input);
    static GCPtr<XmlReader> Create(GCPtr<IO::Stream> input, GCPtr<XmlReaderSettings> settings);

    // Reading
    virtual bool Read() = 0;
    virtual bool ReadToFollowing(const String& name) = 0;
    virtual bool ReadToDescendant(const String& name) = 0;
    virtual bool ReadToNextSibling(const String& name) = 0;
    virtual void Skip() = 0;
    virtual void Close() = 0;

    // Properties
    virtual XmlNodeType NodeType() const = 0;
    virtual String Name() const = 0;
    virtual String LocalName() const = 0;
    virtual String NamespaceURI() const = 0;
    virtual String Prefix() const = 0;
    virtual String Value() const = 0;
    virtual int Depth() const = 0;
    virtual bool IsEmptyElement() const = 0;
    virtual bool HasValue() const = 0;
    virtual bool HasAttributes() const = 0;
    virtual int AttributeCount() const = 0;
    virtual bool EOF() const = 0;

    // Attributes
    virtual String GetAttribute(const String& name) const = 0;
    virtual String GetAttribute(const String& name, const String& namespaceURI) const = 0;
    virtual String GetAttribute(int i) const = 0;
    virtual bool MoveToAttribute(const String& name) = 0;
    virtual bool MoveToAttribute(const String& name, const String& namespaceURI) = 0;
    virtual void MoveToAttribute(int i) = 0;
    virtual bool MoveToFirstAttribute() = 0;
    virtual bool MoveToNextAttribute() = 0;
    virtual bool MoveToElement() = 0;

    // Content reading
    virtual String ReadString() = 0;
    virtual String ReadInnerXml() = 0;
    virtual String ReadOuterXml() = 0;
    virtual void ReadStartElement() = 0;
    virtual void ReadStartElement(const String& name) = 0;
    virtual void ReadStartElement(const String& localname, const String& ns) = 0;
    virtual void ReadEndElement() = 0;
    virtual String ReadElementString() = 0;
    virtual String ReadElementString(const String& name) = 0;
    virtual String ReadElementString(const String& localname, const String& ns) = 0;
};

} // namespace Xml
} // namespace System
