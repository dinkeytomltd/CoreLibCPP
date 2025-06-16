
#include "MockDependencies.h"
#pragma once

#include "XmlWriterSettings.h"

namespace System {
namespace Xml {

enum class WriteState {
    Start = 0,
    Prolog = 1,
    Element = 2,
    Attribute = 3,
    Content = 4,
    Closed = 5,
    Error = 6
};

class XmlWriter {
protected:
    XmlWriter() = default;

public:
    virtual ~XmlWriter() = default;

    // Factory methods
    static GCPtr<XmlWriter> Create(const String& outputFileName);
    static GCPtr<XmlWriter> Create(const String& outputFileName, GCPtr<XmlWriterSettings> settings);
    static GCPtr<XmlWriter> Create(GCPtr<IO::Stream> output);
    static GCPtr<XmlWriter> Create(GCPtr<IO::Stream> output, GCPtr<XmlWriterSettings> settings);

    // Writing methods
    virtual void WriteStartDocument() = 0;
    virtual void WriteStartDocument(bool standalone) = 0;
    virtual void WriteStartDocument(const String& version, const String& encoding, bool standalone) = 0;
    virtual void WriteEndDocument() = 0;

    virtual void WriteStartElement(const String& localName) = 0;
    virtual void WriteStartElement(const String& localName, const String& ns) = 0;
    virtual void WriteStartElement(const String& prefix, const String& localName, const String& ns) = 0;
    virtual void WriteEndElement() = 0;
    virtual void WriteFullEndElement() = 0;

    virtual void WriteStartAttribute(const String& localName) = 0;
    virtual void WriteStartAttribute(const String& localName, const String& ns) = 0;
    virtual void WriteStartAttribute(const String& prefix, const String& localName, const String& ns) = 0;
    virtual void WriteEndAttribute() = 0;

    virtual void WriteAttributeString(const String& localName, const String& value) = 0;
    virtual void WriteAttributeString(const String& localName, const String& ns, const String& value) = 0;
    virtual void WriteAttributeString(const String& prefix, const String& localName, const String& ns, const String& value) = 0;

    virtual void WriteElementString(const String& localName, const String& value) = 0;
    virtual void WriteElementString(const String& localName, const String& ns, const String& value) = 0;
    virtual void WriteElementString(const String& prefix, const String& localName, const String& ns, const String& value) = 0;

    virtual void WriteString(const String& text) = 0;
    virtual void WriteRaw(const String& data) = 0;
    virtual void WriteCharEntity(char ch) = 0;
    virtual void WriteEntityRef(const String& name) = 0;
    virtual void WriteProcessingInstruction(const String& name, const String& text) = 0;
    virtual void WriteComment(const String& text) = 0;
    virtual void WriteCData(const String& text) = 0;
    virtual void WriteWhitespace(const String& ws) = 0;

    // Node writing
    virtual void WriteNode(GCPtr<XmlReader> reader, bool defattr) = 0;
    virtual void WriteAttributes(GCPtr<XmlReader> reader, bool defattr) = 0;

    // State and control
    virtual void Flush() = 0;
    virtual void Close() = 0;

    // Properties
    virtual String LookupPrefix(const String& ns) = 0;
    virtual WriteState GetWriteState() const = 0;
};

} // namespace Xml
} // namespace System
