
#include "MockDependencies.h"
#pragma once

#include <memory>
#include <sstream>

// Forward declarations
namespace pugi { class xml_node; }

namespace System {
namespace Xml {

class XmlDocument;
class XmlAttribute;
class XmlElement;
class XmlNodeList;

class XmlNode : public std::enable_shared_from_this<XmlNode> {
protected:
    pugi::xml_node* pugiNode_;
    GCPtr<XmlDocument> ownerDocument_;
    GCPtr<XmlNode> parentNode_;

public:
    XmlNode();
    XmlNode(pugi::xml_node* node, GCPtr<XmlDocument> doc);
    virtual ~XmlNode() = default;

    // Properties
    virtual XmlNodeType NodeType() const = 0;
    virtual String Name() const;
    virtual String LocalName() const;
    virtual String NamespaceURI() const;
    virtual String Prefix() const;
    virtual String Value() const;
    virtual void SetValue(const String& value);
    virtual String InnerText() const;
    virtual void SetInnerText(const String& text);
    virtual String InnerXml() const;
    virtual void SetInnerXml(const String& xml);
    virtual String OuterXml() const;

    // Navigation
    GCPtr<XmlNode> ParentNode() const { return parentNode_; }
    GCPtr<XmlNode> FirstChild() const;
    GCPtr<XmlNode> LastChild() const;
    GCPtr<XmlNode> NextSibling() const;
    GCPtr<XmlNode> PreviousSibling() const;
    GCPtr<XmlNodeList> ChildNodes() const;
    bool HasChildNodes() const;

    // Document
    GCPtr<XmlDocument> OwnerDocument() const { return ownerDocument_; }

    // Attributes (for elements)
    virtual GCPtr<XmlNodeList> Attributes() const;
    virtual bool HasAttributes() const;

    // Manipulation
    virtual GCPtr<XmlNode> AppendChild(GCPtr<XmlNode> newChild);
    virtual GCPtr<XmlNode> PrependChild(GCPtr<XmlNode> newChild);
    virtual GCPtr<XmlNode> InsertBefore(GCPtr<XmlNode> newChild, GCPtr<XmlNode> refChild);
    virtual GCPtr<XmlNode> InsertAfter(GCPtr<XmlNode> newChild, GCPtr<XmlNode> refChild);
    virtual GCPtr<XmlNode> ReplaceChild(GCPtr<XmlNode> newChild, GCPtr<XmlNode> oldChild);
    virtual GCPtr<XmlNode> RemoveChild(GCPtr<XmlNode> oldChild);
    virtual void RemoveAll();

    // Selection
    GCPtr<XmlNode> SelectSingleNode(const String& xpath) const;
    GCPtr<XmlNodeList> SelectNodes(const String& xpath) const;

    // Cloning
    virtual GCPtr<XmlNode> Clone() const;
    virtual GCPtr<XmlNode> CloneNode(bool deep) const;

    // Internal
    pugi::xml_node* GetPugiNode() const { return pugiNode_; }

protected:
    void SetParent(GCPtr<XmlNode> parent) { parentNode_ = parent; }
    GCPtr<XmlNode> WrapNode(pugi::xml_node node) const;
};

} // namespace Xml
} // namespace System
