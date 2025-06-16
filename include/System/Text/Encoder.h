
#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <rttr/registration>

namespace System {
namespace Text {

// Forward declarations
class EncoderFallback;
class EncoderFallbackBuffer;

/**
 * @brief Abstract base class for stateful character encoders
 */
class Encoder {
protected:
    std::unique_ptr<EncoderFallback> m_fallback;
    std::unique_ptr<EncoderFallbackBuffer> m_fallbackBuffer;
    
public:
    virtual ~Encoder() = default;
    
    // Properties
    EncoderFallback* GetFallback() const { return m_fallback.get(); }
    void SetFallback(std::unique_ptr<EncoderFallback> value);
    
    EncoderFallbackBuffer* GetFallbackBuffer() const { return m_fallbackBuffer.get(); }
    
    // Core encoding methods
    virtual int GetByteCount(const char16_t* chars, int count, bool flush) = 0;
    virtual int GetBytes(const char16_t* chars, int charCount, uint8_t* bytes, int byteCount, bool flush) = 0;
    
    // Convenience overloads
    int GetByteCount(const std::vector<char16_t>& chars, bool flush);
    std::vector<uint8_t> GetBytes(const std::vector<char16_t>& chars, bool flush);
    
    // State management
    virtual void Reset() = 0;
    
    // Conversion methods
    virtual void Convert(const char16_t* chars, int charCount, uint8_t* bytes, int byteCount,
                        bool flush, int& charsUsed, int& bytesUsed, bool& completed) = 0;
    
    // RTTR registration
    RTTR_ENABLE()
};

} // namespace Text
} // namespace System
