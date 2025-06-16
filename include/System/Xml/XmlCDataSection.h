
#include "MockDependencies.h"
#pragma once

#include "XmlText.h"

namespace System {
namespace Xml {

class XmlCDataSection : public XmlText {
public:
    XmlCDataSection();
    XmlCDataSection(pugi::xml_node* node, GCPtr<XmlDocument> doc);
    virtual ~XmlCDataSection() = default;

    // XmlNode overrides
    XmlNodeType NodeType() const override { return XmlNodeType::CDATA; }
    String Name() const override { return "#cdata-section"; }
};

} // namespace Xml
} // namespace System
