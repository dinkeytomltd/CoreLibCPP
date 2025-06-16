
#pragma once

#include "../../String.h"
#include "../../GC/GCPtr.h"
#include <memory>

// Forward declarations
namespace pugi { class xpath_query; }

namespace System {
namespace Xml {
namespace XPath {

class XPathExpression {
private:
    std::unique_ptr<pugi::xpath_query> query_;
    String expression_;

public:
    XPathExpression(const String& xpath);
    virtual ~XPathExpression();

    // Properties
    String Expression() const { return expression_; }

    // Internal
    pugi::xpath_query* GetQuery() const { return query_.get(); }
};

} // namespace XPath
} // namespace Xml
} // namespace System
