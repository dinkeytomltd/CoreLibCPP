
#pragma once

#include "../../String.h"
#include "../../GC/GCPtr.h"
#include "../XmlNodeType.h"
#include "XPathNodeIterator.h"
#include "XPathExpression.h"
#include <memory>

// Forward declarations
namespace pugi { class xml_node; class xml_document; }

namespace System {
namespace Xml {
namespace XPath {

class XPathNavigator {
private:
    pugi::xml_node* currentNode_;
    pugi::xml_document* document_;

public:
    XPathNavigator(pugi::xml_node* node, pugi::xml_document* doc);
    XPathNavigator(const XPathNavigator& other);
    virtual ~XPathNavigator() = default;

    // Properties
    String Name() const;
    String LocalName() const;
    String NamespaceURI() const;
    String Prefix() const;
    String Value() const;
    XmlNodeType NodeType() const;
    bool HasAttributes() const;
    bool HasChildren() const;
    bool IsEmptyElement() const;

    // Navigation
    bool MoveToRoot();
    bool MoveToParent();
    bool MoveToFirstChild();
    bool MoveToLastChild();
    bool MoveToNext();
    bool MoveToPrevious();
    bool MoveToNextSibling();
    bool MoveToPreviousSibling();
    bool MoveToFirstAttribute();
    bool MoveToNextAttribute();
    bool MoveToAttribute(const String& localName, const String& namespaceURI);

    // Selection
    GCPtr<XPathNodeIterator> Select(const String& xpath);
    GCPtr<XPathNodeIterator> Select(GCPtr<XPathExpression> expr);
    GCPtr<XPathNavigator> SelectSingleNode(const String& xpath);
    GCPtr<XPathNavigator> SelectSingleNode(GCPtr<XPathExpression> expr);

    // Evaluation
    String Evaluate(const String& xpath);
    String Evaluate(GCPtr<XPathExpression> expr);

    // Compilation
    GCPtr<XPathExpression> Compile(const String& xpath);

    // Comparison
    bool IsSamePosition(GCPtr<XPathNavigator> other) const;

    // Cloning
    GCPtr<XPathNavigator> Clone() const;

    // String representation
    String OuterXml() const;
    String InnerXml() const;

    // Internal
    pugi::xml_node* GetNode() const { return currentNode_; }
};

} // namespace XPath
} // namespace Xml
} // namespace System
