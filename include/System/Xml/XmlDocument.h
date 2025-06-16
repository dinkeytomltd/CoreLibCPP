
#include "MockDependencies.h"
#pragma once

#include "XmlNode.h"
#include "XmlElement.h"
#include <memory>

// Forward declarations
namespace pugi { class xml_document; }

namespace System {
namespace Xml {

class XmlDocument : public XmlNode {
private:
    std::unique_ptr<pugi::xml_document> pugiDoc_;

public:
    XmlDocument();
    virtual ~XmlDocument();

    // XmlNode overrides
    XmlNodeType NodeType() const override { return XmlNodeType::Document; }
    String Name() const override { return "#document"; }

    // Document properties
    GCPtr<XmlElement> DocumentElement() const;

    // Loading
    void Load(const String& filename);
    void Load(GCPtr<IO::Stream> stream);
    void LoadXml(const String& xml);

    // Saving
    void Save(const String& filename) const;
    void Save(GCPtr<IO::Stream> stream) const;

    // Creation
    GCPtr<XmlElement> CreateElement(const String& name);
    GCPtr<XmlElement> CreateElement(const String& qualifiedName, const String& namespaceURI);
    GCPtr<XmlAttribute> CreateAttribute(const String& name);
    GCPtr<XmlAttribute> CreateAttribute(const String& qualifiedName, const String& namespaceURI);
    GCPtr<XmlNode> CreateTextNode(const String& text);
    GCPtr<XmlNode> CreateComment(const String& data);
    GCPtr<XmlNode> CreateCDataSection(const String& data);

    // Selection
    GCPtr<XmlNodeList> GetElementsByTagName(const String& name) const;
    GCPtr<XmlNodeList> GetElementsByTagName(const String& localName, const String& namespaceURI) const;
    GCPtr<XmlElement> GetElementById(const String& elementId) const;

    // Internal
    pugi::xml_document* GetPugiDocument() const { return pugiDoc_.get(); }
};

} // namespace Xml
} // namespace System
