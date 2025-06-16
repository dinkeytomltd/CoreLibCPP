
#include "MockDependencies.h"
#pragma once

#include <stdexcept>
#include <string>
#include "MockDependencies.h"

namespace System {
namespace Xml {

class XmlException : public std::exception {
private:
    String message_;
    int lineNumber_;
    int linePosition_;

public:
    XmlException();
    XmlException(const String& message);
    XmlException(const String& message, int lineNumber, int linePosition);
    virtual ~XmlException() noexcept = default;

    virtual const char* what() const noexcept override;
    const String& Message() const { return message_; }
    int LineNumber() const { return lineNumber_; }
    int LinePosition() const { return linePosition_; }
};

class XmlSchemaException : public XmlException {
public:
    XmlSchemaException();
    XmlSchemaException(const String& message);
    XmlSchemaException(const String& message, int lineNumber, int linePosition);
};

class XPathException : public XmlException {
public:
    XPathException();
    XPathException(const String& message);
    XPathException(const String& message, int lineNumber, int linePosition);
};

} // namespace Xml
} // namespace System
