
#include "MockDependencies.h"
#pragma once

#include "XmlNode.h"

namespace System {
namespace Xml {

class XmlText : public XmlNode {
public:
    XmlText();
    XmlText(pugi::xml_node* node, GCPtr<XmlDocument> doc);
    virtual ~XmlText() = default;

    // XmlNode overrides
    XmlNodeType NodeType() const override { return XmlNodeType::Text; }
    String Name() const override { return "#text"; }

    // Text-specific operations
    GCPtr<XmlText> SplitText(int offset);
};

} // namespace Xml
} // namespace System
