
#include "MockDependencies.h"
#pragma once

#include "XmlWriter.h"
#include <stack>

namespace System {
namespace Xml {

class XmlTextWriter : public XmlWriter {
private:
    GCPtr<IO::TextWriter> textWriter_;
    GCPtr<Text::StringBuilder> stringBuilder_;
    WriteState writeState_;
    std::stack<String> elementStack_;
    bool formatting_;
    String indentChars_;
    int indentation_;
    int currentIndentLevel_;
    bool writeEndDocumentOnClose_;

public:
    XmlTextWriter(GCPtr<IO::Stream> w, GCPtr<Text::Encoding> encoding);
    XmlTextWriter(GCPtr<IO::TextWriter> w);
    XmlTextWriter(const String& filename, GCPtr<Text::Encoding> encoding);
    virtual ~XmlTextWriter();

    // Properties
    bool Formatting() const { return formatting_; }
    void SetFormatting(bool value) { formatting_ = value; }

    String IndentChars() const { return indentChars_; }
    void SetIndentChars(const String& value) { indentChars_ = value; }

    int Indentation() const { return indentation_; }
    void SetIndentation(int value) { indentation_ = value; }

    WriteState GetWriteState() const override { return writeState_; }

    // XmlWriter implementation
    void WriteStartDocument() override;
    void WriteStartDocument(bool standalone) override;
    void WriteStartDocument(const String& version, const String& encoding, bool standalone) override;
    void WriteEndDocument() override;

    void WriteStartElement(const String& localName) override;
    void WriteStartElement(const String& localName, const String& ns) override;
    void WriteStartElement(const String& prefix, const String& localName, const String& ns) override;
    void WriteEndElement() override;
    void WriteFullEndElement() override;

    void WriteStartAttribute(const String& localName) override;
    void WriteStartAttribute(const String& localName, const String& ns) override;
    void WriteStartAttribute(const String& prefix, const String& localName, const String& ns) override;
    void WriteEndAttribute() override;

    void WriteAttributeString(const String& localName, const String& value) override;
    void WriteAttributeString(const String& localName, const String& ns, const String& value) override;
    void WriteAttributeString(const String& prefix, const String& localName, const String& ns, const String& value) override;

    void WriteElementString(const String& localName, const String& value) override;
    void WriteElementString(const String& localName, const String& ns, const String& value) override;
    void WriteElementString(const String& prefix, const String& localName, const String& ns, const String& value) override;

    void WriteString(const String& text) override;
    void WriteRaw(const String& data) override;
    void WriteCharEntity(char ch) override;
    void WriteEntityRef(const String& name) override;
    void WriteProcessingInstruction(const String& name, const String& text) override;
    void WriteComment(const String& text) override;
    void WriteCData(const String& text) override;
    void WriteWhitespace(const String& ws) override;

    void WriteNode(GCPtr<XmlReader> reader, bool defattr) override;
    void WriteAttributes(GCPtr<XmlReader> reader, bool defattr) override;

    void Flush() override;
    void Close() override;

    String LookupPrefix(const String& ns) override;

private:
    void WriteIndent();
    void WriteEscapedString(const String& text, bool isAttribute);
    String EscapeXmlChars(const String& text, bool isAttribute);
};

} // namespace Xml
} // namespace System
