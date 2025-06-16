
/*
 * Guid.cpp
 * C++ implementation of .NET's System.Guid struct
 */
#include "System/Guid.h"
#include <regex>
#include <chrono>

namespace System {

Guid::Guid(uint32_t a, uint16_t b, uint16_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h, uint8_t i, uint8_t j, uint8_t k) {
    // Store in little-endian format to match .NET behavior
    m_data[0] = static_cast<uint8_t>(a & 0xFF);
    m_data[1] = static_cast<uint8_t>((a >> 8) & 0xFF);
    m_data[2] = static_cast<uint8_t>((a >> 16) & 0xFF);
    m_data[3] = static_cast<uint8_t>((a >> 24) & 0xFF);
    
    m_data[4] = static_cast<uint8_t>(b & 0xFF);
    m_data[5] = static_cast<uint8_t>((b >> 8) & 0xFF);
    
    m_data[6] = static_cast<uint8_t>(c & 0xFF);
    m_data[7] = static_cast<uint8_t>((c >> 8) & 0xFF);
    
    m_data[8] = d;
    m_data[9] = e;
    m_data[10] = f;
    m_data[11] = g;
    m_data[12] = h;
    m_data[13] = i;
    m_data[14] = j;
    m_data[15] = k;
}

Guid Guid::NewGuid() {
    std::array<uint8_t, 16> bytes;
    
    // Use random_device for entropy
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local std::uniform_int_distribution<uint8_t> dis(0, 255);
    
    // Generate 16 random bytes
    for (auto& byte : bytes) {
        byte = dis(gen);
    }
    
    // Set version to 4 (random UUID) - RFC 4122
    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    
    // Set variant bits - RFC 4122
    bytes[8] = (bytes[8] & 0x3F) | 0x80;
    
    return Guid(bytes);
}

uint8_t Guid::HexCharToValue(char c) {
    if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
    if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
    throw std::invalid_argument("Invalid hex character");
}

std::string Guid::BytesToHexString(const std::array<uint8_t, 16>& bytes, bool uppercase) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    if (uppercase) {
        oss << std::uppercase;
    }
    
    for (const auto& byte : bytes) {
        oss << std::setw(2) << static_cast<int>(byte);
    }
    
    return oss.str();
}

Guid Guid::Parse(const std::string& input) {
    Guid result;
    if (!TryParse(input, result)) {
        throw std::invalid_argument("Invalid GUID format");
    }
    return result;
}

bool Guid::TryParse(const std::string& input, Guid& result) {
    if (input.empty()) {
        return false;
    }
    
    std::string cleanInput = input;
    
    // Remove braces or parentheses if present
    if ((cleanInput.front() == '{' && cleanInput.back() == '}') ||
        (cleanInput.front() == '(' && cleanInput.back() == ')')) {
        cleanInput = cleanInput.substr(1, cleanInput.length() - 2);
    }
    
    // Check for standard GUID format with hyphens
    std::regex guidRegex(R"(^([0-9a-fA-F]{8})-?([0-9a-fA-F]{4})-?([0-9a-fA-F]{4})-?([0-9a-fA-F]{4})-?([0-9a-fA-F]{12})$)");
    std::smatch matches;
    
    if (!std::regex_match(cleanInput, matches, guidRegex)) {
        return false;
    }
    
    try {
        std::array<uint8_t, 16> bytes;
        
        // Parse each group
        std::string part1 = matches[1].str(); // 8 chars
        std::string part2 = matches[2].str(); // 4 chars
        std::string part3 = matches[3].str(); // 4 chars
        std::string part4 = matches[4].str(); // 4 chars
        std::string part5 = matches[5].str(); // 12 chars
        
        // Convert to bytes (little-endian for first 3 parts to match .NET)
        // Part 1 (4 bytes, little-endian)
        for (int i = 0; i < 4; ++i) {
            int idx = (3 - i) * 2;
            bytes[i] = (HexCharToValue(part1[idx]) << 4) | HexCharToValue(part1[idx + 1]);
        }
        
        // Part 2 (2 bytes, little-endian)
        for (int i = 0; i < 2; ++i) {
            int idx = (1 - i) * 2;
            bytes[4 + i] = (HexCharToValue(part2[idx]) << 4) | HexCharToValue(part2[idx + 1]);
        }
        
        // Part 3 (2 bytes, little-endian)
        for (int i = 0; i < 2; ++i) {
            int idx = (1 - i) * 2;
            bytes[6 + i] = (HexCharToValue(part3[idx]) << 4) | HexCharToValue(part3[idx + 1]);
        }
        
        // Part 4 (2 bytes, big-endian)
        for (int i = 0; i < 2; ++i) {
            int idx = i * 2;
            bytes[8 + i] = (HexCharToValue(part4[idx]) << 4) | HexCharToValue(part4[idx + 1]);
        }
        
        // Part 5 (6 bytes, big-endian)
        for (int i = 0; i < 6; ++i) {
            int idx = i * 2;
            bytes[10 + i] = (HexCharToValue(part5[idx]) << 4) | HexCharToValue(part5[idx + 1]);
        }
        
        result = Guid(bytes);
        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}

int Guid::CompareTo(const Guid& other) const {
    // Compare byte by byte
    for (size_t i = 0; i < 16; ++i) {
        if (m_data[i] < other.m_data[i]) return -1;
        if (m_data[i] > other.m_data[i]) return 1;
    }
    return 0;
}

std::string Guid::ToString(const std::string& format) const {
    if (format.empty() || format == "D") {
        // Default format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
        std::ostringstream oss;
        oss << std::hex << std::setfill('0') << std::nouppercase;
        
        // First part (4 bytes, little-endian display)
        for (int i = 3; i >= 0; --i) {
            oss << std::setw(2) << static_cast<int>(m_data[i]);
        }
        oss << "-";
        
        // Second part (2 bytes, little-endian display)
        for (int i = 5; i >= 4; --i) {
            oss << std::setw(2) << static_cast<int>(m_data[i]);
        }
        oss << "-";
        
        // Third part (2 bytes, little-endian display)
        for (int i = 7; i >= 6; --i) {
            oss << std::setw(2) << static_cast<int>(m_data[i]);
        }
        oss << "-";
        
        // Fourth part (2 bytes, big-endian display)
        for (int i = 8; i <= 9; ++i) {
            oss << std::setw(2) << static_cast<int>(m_data[i]);
        }
        oss << "-";
        
        // Fifth part (6 bytes, big-endian display)
        for (int i = 10; i <= 15; ++i) {
            oss << std::setw(2) << static_cast<int>(m_data[i]);
        }
        
        return oss.str();
    }
    else if (format == "N") {
        // No hyphens: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        std::string result = ToString("D");
        result.erase(std::remove(result.begin(), result.end(), '-'), result.end());
        return result;
    }
    else if (format == "B") {
        // Braces: {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
        return "{" + ToString("D") + "}";
    }
    else if (format == "P") {
        // Parentheses: (xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx)
        return "(" + ToString("D") + ")";
    }
    else {
        throw std::invalid_argument("Invalid format specifier");
    }
}

} // namespace System
