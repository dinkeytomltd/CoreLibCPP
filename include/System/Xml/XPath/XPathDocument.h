
#pragma once

#include "../../String.h"
#include "../../IO/Stream.h"
#include "../../GC/GCPtr.h"
#include "XPathNavigator.h"
#include <memory>

// Forward declarations
namespace pugi { class xml_document; }

namespace System {
namespace Xml {
namespace XPath {

class XPathDocument {
private:
    std::unique_ptr<pugi::xml_document> doc_;

public:
    XPathDocument(const String& uri);
    XPathDocument(GCPtr<IO::Stream> stream);
    XPathDocument(GCPtr<XmlReader> reader);
    virtual ~XPathDocument();

    // Navigation
    GCPtr<XPathNavigator> CreateNavigator();

    // Internal
    pugi::xml_document* GetDocument() const { return doc_.get(); }
};

} // namespace XPath
} // namespace Xml
} // namespace System
