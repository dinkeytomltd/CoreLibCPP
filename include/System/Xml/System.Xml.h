
#pragma once

// Core XML DOM classes
#include "XmlException.h"
#include "XmlNodeType.h"
#include "XmlNode.h"
#include "XmlNodeList.h"
#include "XmlDocument.h"
#include "XmlElement.h"
#include "XmlAttribute.h"
#include "XmlText.h"
#include "XmlComment.h"
#include "XmlCDataSection.h"
#include "XmlDeclaration.h"

// Streaming XML processing
#include "XmlReader.h"
#include "XmlTextReader.h"
#include "XmlReaderSettings.h"
#include "XmlWriter.h"
#include "XmlTextWriter.h"
#include "XmlWriterSettings.h"

// XPath support
#include "XPath/XPathDocument.h"
#include "XPath/XPathNavigator.h"
#include "XPath/XPathNodeIterator.h"
#include "XPath/XPathExpression.h"

// Schema and validation
#include "Schema/XmlSchema.h"
#include "Schema/XmlSchemaSet.h"

// XML serialization
#include "Serialization/XmlSerializer.h"

namespace System {
namespace Xml {

// Utility functions for XML processing
class XmlConvert {
public:
    // Encoding/decoding XML names
    static String EncodeName(const String& name);
    static String DecodeName(const String& name);
    static String EncodeLocalName(const String& name);

    // Data type conversions
    static String ToString(bool value);
    static String ToString(int value);
    static String ToString(double value);
    static String ToString(float value);
    static String ToString(long value);

    static bool ToBoolean(const String& s);
    static int ToInt32(const String& s);
    static double ToDouble(const String& s);
    static float ToSingle(const String& s);
    static long ToInt64(const String& s);

    // DateTime conversions (would need DateTime implementation)
    // static String ToString(const DateTime& value);
    // static DateTime ToDateTime(const String& s);
};

} // namespace Xml
} // namespace System
