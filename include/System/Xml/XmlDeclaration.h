
#include "MockDependencies.h"
#pragma once

#include "XmlNode.h"

namespace System {
namespace Xml {

class XmlDeclaration : public XmlNode {
public:
    XmlDeclaration();
    XmlDeclaration(pugi::xml_node* node, GCPtr<XmlDocument> doc);
    virtual ~XmlDeclaration() = default;

    // XmlNode overrides
    XmlNodeType NodeType() const override { return XmlNodeType::XmlDeclaration; }
    String Name() const override { return "xml"; }

    // Declaration properties
    String Version() const;
    void SetVersion(const String& version);
    String Encoding() const;
    void SetEncoding(const String& encoding);
    String Standalone() const;
    void SetStandalone(const String& standalone);
};

} // namespace Xml
} // namespace System
