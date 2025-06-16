
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <rttr/registration>

namespace System {
namespace Text {

// Forward declarations
class DecoderFallback;
class EncoderFallback;
class Decoder;
class Encoder;

/**
 * @brief Abstract base class for character encodings
 * 
 * Provides the foundation for all encoding implementations with .NET compatibility
 */
class Encoding {
protected:
    int m_codePage;
    std::unique_ptr<DecoderFallback> m_decoderFallback;
    std::unique_ptr<EncoderFallback> m_encoderFallback;
    bool m_isReadOnly;
    
    // Protected constructor
    Encoding();
    Encoding(int codePage);
    Encoding(int codePage, std::unique_ptr<EncoderFallback> encoderFallback, 
             std::unique_ptr<DecoderFallback> decoderFallback);
    
public:
    virtual ~Encoding() = default;
    
    // Static encoding instances
    static const Encoding& ASCII();
    static const Encoding& UTF8();
    static const Encoding& Unicode();      // UTF-16LE
    static const Encoding& BigEndianUnicode(); // UTF-16BE
    static const Encoding& UTF32();
    static const Encoding& Latin1();
    static const Encoding& Default();
    
    // Static factory methods
    static std::unique_ptr<Encoding> GetEncoding(int codePage);
    static std::unique_ptr<Encoding> GetEncoding(const std::string& name);
    static std::unique_ptr<Encoding> GetEncoding(int codePage, 
        std::unique_ptr<EncoderFallback> encoderFallback,
        std::unique_ptr<DecoderFallback> decoderFallback);
    static std::unique_ptr<Encoding> GetEncoding(const std::string& name,
        std::unique_ptr<EncoderFallback> encoderFallback,
        std::unique_ptr<DecoderFallback> decoderFallback);
    
    // Properties
    virtual int GetCodePage() const { return m_codePage; }
    virtual std::string GetEncodingName() const = 0;
    virtual std::string GetBodyName() const = 0;
    virtual std::string GetHeaderName() const = 0;
    virtual std::string GetWebName() const = 0;
    virtual int GetWindowsCodePage() const { return m_codePage; }
    virtual bool GetIsSingleByte() const = 0;
    virtual bool GetIsBrowserDisplay() const = 0;
    virtual bool GetIsBrowserSave() const = 0;
    virtual bool GetIsMailNewsDisplay() const = 0;
    virtual bool GetIsMailNewsSave() const = 0;
    virtual bool GetIsReadOnly() const { return m_isReadOnly; }
    
    // Fallback properties
    DecoderFallback* GetDecoderFallback() const { return m_decoderFallback.get(); }
    void SetDecoderFallback(std::unique_ptr<DecoderFallback> value);
    
    EncoderFallback* GetEncoderFallback() const { return m_encoderFallback.get(); }
    void SetEncoderFallback(std::unique_ptr<EncoderFallback> value);
    
    // Preamble
    virtual std::vector<uint8_t> GetPreamble() const = 0;
    
    // Core encoding/decoding methods
    virtual int GetByteCount(const char16_t* chars, int count) const = 0;
    virtual int GetByteCount(const std::string& s) const = 0;
    virtual int GetByteCount(const std::u16string& s) const = 0;
    
    virtual int GetBytes(const char16_t* chars, int charCount, uint8_t* bytes, int byteCount) const = 0;
    virtual std::vector<uint8_t> GetBytes(const std::string& s) const = 0;
    virtual std::vector<uint8_t> GetBytes(const std::u16string& s) const = 0;
    virtual int GetBytes(const std::string& s, int charIndex, int charCount, 
                        uint8_t* bytes, int byteIndex) const = 0;
    
    virtual int GetCharCount(const uint8_t* bytes, int count) const = 0;
    virtual int GetCharCount(const std::vector<uint8_t>& bytes) const = 0;
    
    virtual int GetChars(const uint8_t* bytes, int byteCount, char16_t* chars, int charCount) const = 0;
    virtual std::u16string GetChars(const std::vector<uint8_t>& bytes) const = 0;
    virtual int GetChars(const uint8_t* bytes, int byteCount, int byteIndex,
                        char16_t* chars, int charIndex) const = 0;
    
    virtual std::string GetString(const uint8_t* bytes, int count) const = 0;
    virtual std::string GetString(const std::vector<uint8_t>& bytes) const = 0;
    virtual std::string GetString(const uint8_t* bytes, int index, int count) const = 0;
    
    // Maximum count methods
    virtual int GetMaxByteCount(int charCount) const = 0;
    virtual int GetMaxCharCount(int byteCount) const = 0;
    
    // Decoder and Encoder creation
    virtual std::unique_ptr<Decoder> GetDecoder() const = 0;
    virtual std::unique_ptr<Encoder> GetEncoder() const = 0;
    
    // Conversion methods
    static std::vector<uint8_t> Convert(const Encoding& srcEncoding, const Encoding& dstEncoding,
                                       const std::vector<uint8_t>& bytes);
    static std::vector<uint8_t> Convert(const Encoding& srcEncoding, const Encoding& dstEncoding,
                                       const uint8_t* bytes, int index, int count);
    
    // Equality and hashing
    virtual bool Equals(const Encoding& other) const;
    virtual size_t GetHashCode() const;
    
    // Clone method
    virtual std::unique_ptr<Encoding> Clone() const = 0;
    
    // RTTR registration
    RTTR_ENABLE()
};

} // namespace Text
} // namespace System
