
#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <rttr/registration>

namespace System {
namespace Text {

// Forward declarations
class DecoderFallback;
class DecoderFallbackBuffer;

/**
 * @brief Abstract base class for stateful character decoders
 */
class Decoder {
protected:
    std::unique_ptr<DecoderFallback> m_fallback;
    std::unique_ptr<DecoderFallbackBuffer> m_fallbackBuffer;
    
public:
    virtual ~Decoder() = default;
    
    // Properties
    DecoderFallback* GetFallback() const { return m_fallback.get(); }
    void SetFallback(std::unique_ptr<DecoderFallback> value);
    
    DecoderFallbackBuffer* GetFallbackBuffer() const { return m_fallbackBuffer.get(); }
    
    // Core decoding methods
    virtual int GetCharCount(const uint8_t* bytes, int count, bool flush) = 0;
    virtual int GetChars(const uint8_t* bytes, int byteCount, char16_t* chars, int charCount, bool flush) = 0;
    
    // Convenience overloads
    int GetCharCount(const std::vector<uint8_t>& bytes, bool flush);
    std::vector<char16_t> GetChars(const std::vector<uint8_t>& bytes, bool flush);
    
    // State management
    virtual void Reset() = 0;
    
    // Conversion methods
    virtual void Convert(const uint8_t* bytes, int byteCount, char16_t* chars, int charCount,
                        bool flush, int& bytesUsed, int& charsUsed, bool& completed) = 0;
    
    // RTTR registration
    RTTR_ENABLE()
};

} // namespace Text
} // namespace System
