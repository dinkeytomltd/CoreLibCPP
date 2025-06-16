
#pragma once

#include "../../String.h"
#include "../../GC/GCPtr.h"
#include <vector>
#include <memory>

// Forward declarations
namespace pugi { class xpath_node_set; class xml_node; class xml_document; }

namespace System {
namespace Xml {
namespace XPath {

class XPathNavigator;

class XPathNodeIterator {
private:
    std::unique_ptr<pugi::xpath_node_set> nodeSet_;
    pugi::xml_document* document_;
    int currentPosition_;

public:
    XPathNodeIterator(std::unique_ptr<pugi::xpath_node_set> nodeSet, pugi::xml_document* doc);
    virtual ~XPathNodeIterator();

    // Properties
    int Count() const;
    int CurrentPosition() const { return currentPosition_; }
    GCPtr<XPathNavigator> Current() const;

    // Iteration
    bool MoveNext();
    GCPtr<XPathNodeIterator> Clone() const;

    // Internal
    void Reset() { currentPosition_ = 0; }
};

} // namespace XPath
} // namespace Xml
} // namespace System
