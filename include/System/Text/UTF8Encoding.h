
#pragma once

#include "System/Text/Encoding.h"
#include <memory>

namespace System {
namespace Text {

/**
 * @brief High-performance UTF-8 encoding implementation with SIMD optimization
 * 
 * This implementation focuses on maximum performance using SIMD instructions
 * where available and optimized algorithms for UTF-8 validation and conversion.
 */
class UTF8Encoding : public Encoding {
private:
    bool m_encoderShouldEmitUTF8Identifier;
    bool m_throwOnInvalidBytes;
    
    // Internal SIMD-optimized methods
    static bool ValidateUTF8_SIMD(const uint8_t* bytes, size_t length);
    static size_t ConvertUTF8ToUTF16_SIMD(const uint8_t* utf8, size_t utf8Length, 
                                         char16_t* utf16, size_t utf16Capacity);
    static size_t ConvertUTF16ToUTF8_SIMD(const char16_t* utf16, size_t utf16Length,
                                         uint8_t* utf8, size_t utf8Capacity);
    
    // Fallback methods for non-SIMD systems
    static bool ValidateUTF8_Scalar(const uint8_t* bytes, size_t length);
    static size_t ConvertUTF8ToUTF16_Scalar(const uint8_t* utf8, size_t utf8Length,
                                           char16_t* utf16, size_t utf16Capacity);
    static size_t ConvertUTF16ToUTF8_Scalar(const char16_t* utf16, size_t utf16Length,
                                           uint8_t* utf8, size_t utf8Capacity);
    
public:
    // Constructors
    UTF8Encoding();
    UTF8Encoding(bool encoderShouldEmitUTF8Identifier);
    UTF8Encoding(bool encoderShouldEmitUTF8Identifier, bool throwOnInvalidBytes);
    
    // Copy constructor and assignment
    UTF8Encoding(const UTF8Encoding& other);
    UTF8Encoding& operator=(const UTF8Encoding& other);
    
    // Move constructor and assignment
    UTF8Encoding(UTF8Encoding&& other) noexcept;
    UTF8Encoding& operator=(UTF8Encoding&& other) noexcept;
    
    // Destructor
    virtual ~UTF8Encoding() = default;
    
    // Properties
    virtual std::string GetEncodingName() const override { return "Unicode (UTF-8)"; }
    virtual std::string GetBodyName() const override { return "utf-8"; }
    virtual std::string GetHeaderName() const override { return "utf-8"; }
    virtual std::string GetWebName() const override { return "utf-8"; }
    virtual bool GetIsSingleByte() const override { return false; }
    virtual bool GetIsBrowserDisplay() const override { return true; }
    virtual bool GetIsBrowserSave() const override { return true; }
    virtual bool GetIsMailNewsDisplay() const override { return true; }
    virtual bool GetIsMailNewsSave() const override { return true; }
    
    // Preamble (BOM)
    virtual std::vector<uint8_t> GetPreamble() const override;
    
    // Core encoding/decoding methods - optimized implementations
    virtual int GetByteCount(const char16_t* chars, int count) const override;
    virtual int GetByteCount(const std::string& s) const override;
    virtual int GetByteCount(const std::u16string& s) const override;
    
    virtual int GetBytes(const char16_t* chars, int charCount, uint8_t* bytes, int byteCount) const override;
    virtual std::vector<uint8_t> GetBytes(const std::string& s) const override;
    virtual std::vector<uint8_t> GetBytes(const std::u16string& s) const override;
    virtual int GetBytes(const std::string& s, int charIndex, int charCount, 
                        uint8_t* bytes, int byteIndex) const override;
    
    virtual int GetCharCount(const uint8_t* bytes, int count) const override;
    virtual int GetCharCount(const std::vector<uint8_t>& bytes) const override;
    
    virtual int GetChars(const uint8_t* bytes, int byteCount, char16_t* chars, int charCount) const override;
    virtual std::u16string GetChars(const std::vector<uint8_t>& bytes) const override;
    virtual int GetChars(const uint8_t* bytes, int byteCount, int byteIndex,
                        char16_t* chars, int charIndex) const override;
    
    virtual std::string GetString(const uint8_t* bytes, int count) const override;
    virtual std::string GetString(const std::vector<uint8_t>& bytes) const override;
    virtual std::string GetString(const uint8_t* bytes, int index, int count) const override;
    
    // Maximum count methods
    virtual int GetMaxByteCount(int charCount) const override;
    virtual int GetMaxCharCount(int byteCount) const override;
    
    // Decoder and Encoder creation
    virtual std::unique_ptr<Decoder> GetDecoder() const override;
    virtual std::unique_ptr<Encoder> GetEncoder() const override;
    
    // Clone method
    virtual std::unique_ptr<Encoding> Clone() const override;
    
    // UTF-8 specific validation methods
    static bool IsValidUTF8(const uint8_t* bytes, size_t length);
    static bool IsValidUTF8(const std::vector<uint8_t>& bytes);
    static bool IsValidUTF8(const std::string& str);
    
    // High-performance conversion methods
    static std::u16string UTF8ToUTF16(const std::string& utf8);
    static std::string UTF16ToUTF8(const std::u16string& utf16);
    static std::string UTF16ToUTF8(const char16_t* utf16, size_t length);
    
    // RTTR registration
    RTTR_ENABLE(Encoding)
};

} // namespace Text
} // namespace System
