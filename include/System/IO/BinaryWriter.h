
#pragma once
/*
 * BinaryWriter.h
 * C++ implementation of .NET's System.IO.BinaryWriter class
 * 
 * Writes primitive types in binary to a stream and supports writing strings in a specific encoding.
 */
#include "../Object.h"
#include "../String.h"
#include "../Text/Encoding.h"
#include "Stream.h"
#include <memory>

namespace System::IO {

class BinaryWriter : public Object {
private:
    Stream* m_stream;
    std::vector<uint8_t> m_buffer;
    std::shared_ptr<Text::Encoding> m_encoding;
    bool m_leaveOpen;
    
    static constexpr int LargeByteBufferSize = 256;

    void Write7BitEncodedInt(int value);

public:
    // Constructors
    explicit BinaryWriter(Stream* output);
    BinaryWriter(Stream* output, std::shared_ptr<Text::Encoding> encoding);
    BinaryWriter(Stream* output, std::shared_ptr<Text::Encoding> encoding, bool leaveOpen);
    
    // Destructor
    virtual ~BinaryWriter();
    
    // Non-copyable but movable
    BinaryWriter(const BinaryWriter&) = delete;
    BinaryWriter& operator=(const BinaryWriter&) = delete;
    BinaryWriter(BinaryWriter&& other) noexcept;
    BinaryWriter& operator=(BinaryWriter&& other) noexcept;
    
    // Properties
    [[nodiscard]] Stream* BaseStream() const { return m_stream; }
    
    // Methods
    void Close();
    void Flush();
    int64_t Seek(int offset, SeekOrigin origin);
    
    // Write primitive types
    void Write(bool value);
    void Write(uint8_t value);
    void Write(int8_t value);
    void Write(char value);
    void Write(int16_t value);
    void Write(uint16_t value);
    void Write(int32_t value);
    void Write(uint32_t value);
    void Write(int64_t value);
    void Write(uint64_t value);
    void Write(float value);
    void Write(double value);
    void Write(const String& value);
    
    // Write arrays
    void Write(const std::vector<char>& chars);
    void Write(const std::vector<char>& chars, int index, int count);
    void Write(const std::vector<uint8_t>& buffer);
    void Write(const std::vector<uint8_t>& buffer, int index, int count);

private:
    void EnsureNotClosed() const;
    void Initialize(Stream* output, std::shared_ptr<Text::Encoding> encoding, bool leaveOpen);
};

} // namespace System::IO
