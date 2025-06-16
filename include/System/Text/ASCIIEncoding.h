
#pragma once
/*
 * ASCIIEncoding.h
 * C++ implementation of .NET's System.Text.ASCIIEncoding class
 */
#include "Encoding.h"

namespace System {
namespace Text {

class ASCIIEncoding : public Encoding {
public:
    ASCIIEncoding() = default;
    ~ASCIIEncoding() override = default;

    // Implement abstract methods from Encoding
    std::vector<uint8_t> GetBytes(const String& s) const override;
    String GetString(const std::vector<uint8_t>& bytes) const override;
};

} // namespace Text
} // namespace System
