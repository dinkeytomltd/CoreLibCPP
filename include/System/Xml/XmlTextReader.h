
#include "MockDependencies.h"
#pragma once

#include "XmlReader.h"
#include <memory>

// Forward declarations
namespace pugi { class xml_document; class xml_node; }

namespace System {
namespace Xml {

class XmlTextReader : public XmlReader {
private:
    std::unique_ptr<pugi::xml_document> doc_;
    pugi::xml_node* currentNode_;
    GCPtr<IO::TextReader> textReader_;
    String inputUri_;
    int depth_;
    bool eof_;

public:
    XmlTextReader(const String& url);
    XmlTextReader(GCPtr<IO::Stream> input);
    XmlTextReader(GCPtr<IO::TextReader> input);
    virtual ~XmlTextReader();

    // XmlReader implementation
    bool Read() override;
    bool ReadToFollowing(const String& name) override;
    bool ReadToDescendant(const String& name) override;
    bool ReadToNextSibling(const String& name) override;
    void Skip() override;
    void Close() override;

    // Properties
    XmlNodeType NodeType() const override;
    String Name() const override;
    String LocalName() const override;
    String NamespaceURI() const override;
    String Prefix() const override;
    String Value() const override;
    int Depth() const override { return depth_; }
    bool IsEmptyElement() const override;
    bool HasValue() const override;
    bool HasAttributes() const override;
    int AttributeCount() const override;
    bool EOF() const override { return eof_; }

    // Attributes
    String GetAttribute(const String& name) const override;
    String GetAttribute(const String& name, const String& namespaceURI) const override;
    String GetAttribute(int i) const override;
    bool MoveToAttribute(const String& name) override;
    bool MoveToAttribute(const String& name, const String& namespaceURI) override;
    void MoveToAttribute(int i) override;
    bool MoveToFirstAttribute() override;
    bool MoveToNextAttribute() override;
    bool MoveToElement() override;

    // Content reading
    String ReadString() override;
    String ReadInnerXml() override;
    String ReadOuterXml() override;
    void ReadStartElement() override;
    void ReadStartElement(const String& name) override;
    void ReadStartElement(const String& localname, const String& ns) override;
    void ReadEndElement() override;
    String ReadElementString() override;
    String ReadElementString(const String& name) override;
    String ReadElementString(const String& localname, const String& ns) override;

private:
    void Initialize();
    void LoadDocument();
};

} // namespace Xml
} // namespace System
