
#include "MockDependencies.h"
#pragma once

#include "XmlNode.h"

namespace System {
namespace Xml {

class XmlElement;

class XmlAttribute : public XmlNode {
private:
    GCPtr<XmlElement> ownerElement_;

public:
    XmlAttribute();
    XmlAttribute(pugi::xml_node* node, GCPtr<XmlDocument> doc, GCPtr<XmlElement> owner);
    virtual ~XmlAttribute() = default;

    // XmlNode overrides
    XmlNodeType NodeType() const override { return XmlNodeType::Attribute; }

    // Attribute-specific properties
    GCPtr<XmlElement> OwnerElement() const { return ownerElement_; }
    bool Specified() const { return true; } // Always true in our implementation

    // Value access
    String Value() const override;
    void SetValue(const String& value) override;
};

} // namespace Xml
} // namespace System
