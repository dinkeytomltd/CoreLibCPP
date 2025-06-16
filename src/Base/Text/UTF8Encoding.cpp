
/*
 * UTF8Encoding.cpp
 * Implementation file for System.Text.UTF8Encoding
 */
#include "../../../include/System/Text/UTF8Encoding.h"
#include <algorithm>
#include <stdexcept>

namespace System {
namespace Text {

std::vector<uint8_t> UTF8Encoding::GetBytes(const String& s) const {
    std::vector<uint8_t> result;
    std::string str = s.ToStdString();
    
    for (char c : str) {
        uint32_t codePoint = static_cast<uint8_t>(c);
        EncodeUTF8Char(codePoint, result);
    }
    
    return result;
}

String UTF8Encoding::GetString(const std::vector<uint8_t>& bytes) const {
    if (!IsValidUTF8(bytes)) {
        throw std::invalid_argument("Invalid UTF-8 byte sequence");
    }
    
    std::string result;
    size_t i = 0;
    
    while (i < bytes.size()) {
        size_t bytesConsumed = 0;
        uint32_t codePoint = DecodeUTF8Char(&bytes[i], bytesConsumed);
        
        if (bytesConsumed == 0) {
            throw std::invalid_argument("Invalid UTF-8 byte sequence");
        }
        
        // For simplicity, we'll only handle ASCII range in this basic implementation
        if (codePoint <= 0x7F) {
            result += static_cast<char>(codePoint);
        } else {
            // For non-ASCII characters, we'll use a replacement character
            result += '?';
        }
        
        i += bytesConsumed;
    }
    
    return String(result);
}

bool UTF8Encoding::IsValidUTF8(const std::vector<uint8_t>& bytes) {
    size_t i = 0;
    
    while (i < bytes.size()) {
        uint8_t byte = bytes[i];
        
        if (byte <= 0x7F) {
            // ASCII character
            i++;
        } else if ((byte & 0xE0) == 0xC0) {
            // 2-byte sequence
            if (i + 1 >= bytes.size() || (bytes[i + 1] & 0xC0) != 0x80) {
                return false;
            }
            i += 2;
        } else if ((byte & 0xF0) == 0xE0) {
            // 3-byte sequence
            if (i + 2 >= bytes.size() || 
                (bytes[i + 1] & 0xC0) != 0x80 || 
                (bytes[i + 2] & 0xC0) != 0x80) {
                return false;
            }
            i += 3;
        } else if ((byte & 0xF8) == 0xF0) {
            // 4-byte sequence
            if (i + 3 >= bytes.size() || 
                (bytes[i + 1] & 0xC0) != 0x80 || 
                (bytes[i + 2] & 0xC0) != 0x80 || 
                (bytes[i + 3] & 0xC0) != 0x80) {
                return false;
            }
            i += 4;
        } else {
            return false;
        }
    }
    
    return true;
}

uint32_t UTF8Encoding::DecodeUTF8Char(const uint8_t* bytes, size_t& bytesConsumed) {
    uint8_t firstByte = bytes[0];
    
    if (firstByte <= 0x7F) {
        // ASCII character
        bytesConsumed = 1;
        return firstByte;
    } else if ((firstByte & 0xE0) == 0xC0) {
        // 2-byte sequence
        bytesConsumed = 2;
        return ((firstByte & 0x1F) << 6) | (bytes[1] & 0x3F);
    } else if ((firstByte & 0xF0) == 0xE0) {
        // 3-byte sequence
        bytesConsumed = 3;
        return ((firstByte & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
    } else if ((firstByte & 0xF8) == 0xF0) {
        // 4-byte sequence
        bytesConsumed = 4;
        return ((firstByte & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) | 
               ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
    }
    
    bytesConsumed = 0;
    return 0;
}

void UTF8Encoding::EncodeUTF8Char(uint32_t codePoint, std::vector<uint8_t>& output) {
    if (codePoint <= 0x7F) {
        // ASCII character
        output.push_back(static_cast<uint8_t>(codePoint));
    } else if (codePoint <= 0x7FF) {
        // 2-byte sequence
        output.push_back(0xC0 | (codePoint >> 6));
        output.push_back(0x80 | (codePoint & 0x3F));
    } else if (codePoint <= 0xFFFF) {
        // 3-byte sequence
        output.push_back(0xE0 | (codePoint >> 12));
        output.push_back(0x80 | ((codePoint >> 6) & 0x3F));
        output.push_back(0x80 | (codePoint & 0x3F));
    } else if (codePoint <= 0x10FFFF) {
        // 4-byte sequence
        output.push_back(0xF0 | (codePoint >> 18));
        output.push_back(0x80 | ((codePoint >> 12) & 0x3F));
        output.push_back(0x80 | ((codePoint >> 6) & 0x3F));
        output.push_back(0x80 | (codePoint & 0x3F));
    }
}

} // namespace Text
} // namespace System
