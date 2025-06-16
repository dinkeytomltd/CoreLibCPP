
#pragma once

#include <memory>
#include <string>
#include <rttr/registration>

namespace System {
namespace Text {

// Forward declarations
class DecoderFallbackBuffer;

/**
 * @brief Abstract base class for decoder fallback strategies
 * 
 * Provides error handling when byte sequences cannot be decoded to characters
 */
class DecoderFallback {
public:
    virtual ~DecoderFallback() = default;
    
    // Abstract methods
    virtual std::unique_ptr<DecoderFallbackBuffer> CreateFallbackBuffer() const = 0;
    virtual int GetMaxCharCount() const = 0;
    
    // Static fallback instances
    static const DecoderFallback& ExceptionFallback();
    static const DecoderFallback& ReplacementFallback();
    
    // Equality
    virtual bool Equals(const DecoderFallback& other) const = 0;
    virtual size_t GetHashCode() const = 0;
    
    // RTTR registration
    RTTR_ENABLE()
};

} // namespace Text
} // namespace System
