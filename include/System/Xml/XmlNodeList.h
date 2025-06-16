
#include "MockDependencies.h"
#pragma once

#include <vector>

namespace System {
namespace Xml {

class XmlNode;

class XmlNodeList {
private:
    std::vector<GCPtr<XmlNode>> nodes_;

public:
    XmlNodeList();
    XmlNodeList(const std::vector<GCPtr<XmlNode>>& nodes);
    virtual ~XmlNodeList() = default;

    // Properties
    int Count() const { return static_cast<int>(nodes_.size()); }
    bool IsEmpty() const { return nodes_.empty(); }

    // Indexing
    GCPtr<XmlNode> Item(int index) const;
    GCPtr<XmlNode> operator[](int index) const { return Item(index); }

    // Iteration
    std::vector<GCPtr<XmlNode>>::const_iterator begin() const { return nodes_.begin(); }
    std::vector<GCPtr<XmlNode>>::const_iterator end() const { return nodes_.end(); }

    // Internal
    void Add(GCPtr<XmlNode> node);
    void Clear();
};

} // namespace Xml
} // namespace System
