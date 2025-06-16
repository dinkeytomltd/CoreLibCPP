
#pragma once

#include <memory>
#include <string>
#include <rttr/registration>

namespace System {
namespace Text {

// Forward declarations
class EncoderFallbackBuffer;

/**
 * @brief Abstract base class for encoder fallback strategies
 * 
 * Provides error handling when characters cannot be encoded to byte sequences
 */
class EncoderFallback {
public:
    virtual ~EncoderFallback() = default;
    
    // Abstract methods
    virtual std::unique_ptr<EncoderFallbackBuffer> CreateFallbackBuffer() const = 0;
    virtual int GetMaxCharCount() const = 0;
    
    // Static fallback instances
    static const EncoderFallback& ExceptionFallback();
    static const EncoderFallback& ReplacementFallback();
    
    // Equality
    virtual bool Equals(const EncoderFallback& other) const = 0;
    virtual size_t GetHashCode() const = 0;
    
    // RTTR registration
    RTTR_ENABLE()
};

} // namespace Text
} // namespace System
