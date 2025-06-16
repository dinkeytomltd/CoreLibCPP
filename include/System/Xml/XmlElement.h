
#include "MockDependencies.h"
#pragma once

#include "XmlNode.h"
#include "XmlAttribute.h"

namespace System {
namespace Xml {

class XmlElement : public XmlNode, public std::enable_shared_from_this<XmlElement> {
public:
    XmlElement();
    XmlElement(pugi::xml_node* node, GCPtr<XmlDocument> doc);
    virtual ~XmlElement() = default;

    // XmlNode overrides
    XmlNodeType NodeType() const override { return XmlNodeType::Element; }

    // Element-specific properties
    String TagName() const { return Name(); }
    bool IsEmpty() const;

    // Attributes
    GCPtr<XmlNodeList> Attributes() const override;
    bool HasAttributes() const override;
    String GetAttribute(const String& name) const;
    String GetAttribute(const String& localName, const String& namespaceURI) const;
    void SetAttribute(const String& name, const String& value);
    void SetAttribute(const String& localName, const String& namespaceURI, const String& value);
    void RemoveAttribute(const String& name);
    void RemoveAttribute(const String& localName, const String& namespaceURI);
    bool HasAttribute(const String& name) const;
    bool HasAttribute(const String& localName, const String& namespaceURI) const;
    GCPtr<XmlAttribute> GetAttributeNode(const String& name) const;
    GCPtr<XmlAttribute> GetAttributeNode(const String& localName, const String& namespaceURI) const;
    GCPtr<XmlAttribute> SetAttributeNode(GCPtr<XmlAttribute> newAttr);
    GCPtr<XmlAttribute> RemoveAttributeNode(GCPtr<XmlAttribute> oldAttr);

    // Element selection
    GCPtr<XmlNodeList> GetElementsByTagName(const String& name) const;
    GCPtr<XmlNodeList> GetElementsByTagName(const String& localName, const String& namespaceURI) const;
};

} // namespace Xml
} // namespace System
