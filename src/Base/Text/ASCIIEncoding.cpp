
/*
 * ASCIIEncoding.cpp
 * Implementation file for System.Text.ASCIIEncoding
 */
#include "../../../include/System/Text/ASCIIEncoding.h"
#include <algorithm>

namespace System {
namespace Text {

std::vector<uint8_t> ASCIIEncoding::GetBytes(const String& s) const {
    std::vector<uint8_t> result;
    std::string str = s.ToStdString();
    
    result.reserve(str.length());
    
    for (char c : str) {
        // ASCII encoding: only characters 0-127 are valid
        // Non-ASCII characters are replaced with '?'
        if (static_cast<unsigned char>(c) <= 127) {
            result.push_back(static_cast<uint8_t>(c));
        } else {
            result.push_back(static_cast<uint8_t>('?'));
        }
    }
    
    return result;
}

String ASCIIEncoding::GetString(const std::vector<uint8_t>& bytes) const {
    std::string result;
    result.reserve(bytes.size());
    
    for (uint8_t byte : bytes) {
        // ASCII decoding: only bytes 0-127 are valid ASCII
        // Non-ASCII bytes are replaced with '?'
        if (byte <= 127) {
            result += static_cast<char>(byte);
        } else {
            result += '?';
        }
    }
    
    return String(result);
}

} // namespace Text
} // namespace System
