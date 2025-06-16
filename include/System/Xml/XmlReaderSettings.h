
#include "MockDependencies.h"
#pragma once


namespace System {
namespace Xml {

enum class ConformanceLevel {
    Auto = 0,
    Fragment = 1,
    Document = 2
};

enum class DtdProcessing {
    Prohibit = 0,
    Ignore = 1,
    Parse = 2
};

class XmlReaderSettings {
private:
    bool checkCharacters_;
    bool closeInput_;
    ConformanceLevel conformanceLevel_;
    DtdProcessing dtdProcessing_;
    bool ignoreComments_;
    bool ignoreProcessingInstructions_;
    bool ignoreWhitespace_;
    int lineNumberOffset_;
    int linePositionOffset_;
    long maxCharactersFromEntities_;
    long maxCharactersInDocument_;

public:
    XmlReaderSettings();
    virtual ~XmlReaderSettings() = default;

    // Properties
    bool CheckCharacters() const { return checkCharacters_; }
    void SetCheckCharacters(bool value) { checkCharacters_ = value; }

    bool CloseInput() const { return closeInput_; }
    void SetCloseInput(bool value) { closeInput_ = value; }

    ConformanceLevel GetConformanceLevel() const { return conformanceLevel_; }
    void SetConformanceLevel(ConformanceLevel value) { conformanceLevel_ = value; }

    DtdProcessing GetDtdProcessing() const { return dtdProcessing_; }
    void SetDtdProcessing(DtdProcessing value) { dtdProcessing_ = value; }

    bool IgnoreComments() const { return ignoreComments_; }
    void SetIgnoreComments(bool value) { ignoreComments_ = value; }

    bool IgnoreProcessingInstructions() const { return ignoreProcessingInstructions_; }
    void SetIgnoreProcessingInstructions(bool value) { ignoreProcessingInstructions_ = value; }

    bool IgnoreWhitespace() const { return ignoreWhitespace_; }
    void SetIgnoreWhitespace(bool value) { ignoreWhitespace_ = value; }

    int LineNumberOffset() const { return lineNumberOffset_; }
    void SetLineNumberOffset(int value) { lineNumberOffset_ = value; }

    int LinePositionOffset() const { return linePositionOffset_; }
    void SetLinePositionOffset(int value) { linePositionOffset_ = value; }

    long MaxCharactersFromEntities() const { return maxCharactersFromEntities_; }
    void SetMaxCharactersFromEntities(long value) { maxCharactersFromEntities_ = value; }

    long MaxCharactersInDocument() const { return maxCharactersInDocument_; }
    void SetMaxCharactersInDocument(long value) { maxCharactersInDocument_ = value; }

    // Methods
    GCPtr<XmlReaderSettings> Clone() const;
    void Reset();
};

} // namespace Xml
} // namespace System
