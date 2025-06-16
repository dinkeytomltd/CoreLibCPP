
#include "MockDependencies.h"
#pragma once

#include "XmlNode.h"

namespace System {
namespace Xml {

class XmlComment : public XmlNode {
public:
    XmlComment();
    XmlComment(pugi::xml_node* node, GCPtr<XmlDocument> doc);
    virtual ~XmlComment() = default;

    // XmlNode overrides
    XmlNodeType NodeType() const override { return XmlNodeType::Comment; }
    String Name() const override { return "#comment"; }
};

} // namespace Xml
} // namespace System
