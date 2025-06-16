
#include "MockDependencies.h"
#pragma once


namespace System {
namespace Xml {

enum class ConformanceLevel;

class XmlWriterSettings {
private:
    bool checkCharacters_;
    bool closeOutput_;
    ConformanceLevel conformanceLevel_;
    GCPtr<Text::Encoding> encoding_;
    bool indent_;
    String indentChars_;
    String newLineChars_;
    bool newLineOnAttributes_;
    bool omitXmlDeclaration_;

public:
    XmlWriterSettings();
    virtual ~XmlWriterSettings() = default;

    // Properties
    bool CheckCharacters() const { return checkCharacters_; }
    void SetCheckCharacters(bool value) { checkCharacters_ = value; }

    bool CloseOutput() const { return closeOutput_; }
    void SetCloseOutput(bool value) { closeOutput_ = value; }

    ConformanceLevel GetConformanceLevel() const { return conformanceLevel_; }
    void SetConformanceLevel(ConformanceLevel value) { conformanceLevel_ = value; }

    GCPtr<Text::Encoding> GetEncoding() const { return encoding_; }
    void SetEncoding(GCPtr<Text::Encoding> value) { encoding_ = value; }

    bool Indent() const { return indent_; }
    void SetIndent(bool value) { indent_ = value; }

    String IndentChars() const { return indentChars_; }
    void SetIndentChars(const String& value) { indentChars_ = value; }

    String NewLineChars() const { return newLineChars_; }
    void SetNewLineChars(const String& value) { newLineChars_ = value; }

    bool NewLineOnAttributes() const { return newLineOnAttributes_; }
    void SetNewLineOnAttributes(bool value) { newLineOnAttributes_ = value; }

    bool OmitXmlDeclaration() const { return omitXmlDeclaration_; }
    void SetOmitXmlDeclaration(bool value) { omitXmlDeclaration_ = value; }

    // Methods
    GCPtr<XmlWriterSettings> Clone() const;
    void Reset();
};

} // namespace Xml
} // namespace System
