
#pragma once
/*
 * BinaryReader.h
 * C++ implementation of .NET's System.IO.BinaryReader class
 * 
 * Reads primitive data types as binary values in a specific encoding.
 */
#include "../Object.h"
#include "../String.h"
#include "../Text/Encoding.h"
#include "Stream.h"
#include <memory>

namespace System::IO {

class BinaryReader : public Object {
private:
    Stream* m_stream;
    std::vector<uint8_t> m_buffer;
    std::shared_ptr<Text::Encoding> m_encoding;
    std::vector<char> m_charBuffer;
    bool m_leaveOpen;
    
    static constexpr int MaxCharBytesSize = 128;

    void FillBuffer(int numBytes);
    int Read7BitEncodedInt();
    int InternalReadOneChar();

public:
    // Constructors
    explicit BinaryReader(Stream* input);
    BinaryReader(Stream* input, std::shared_ptr<Text::Encoding> encoding);
    BinaryReader(Stream* input, std::shared_ptr<Text::Encoding> encoding, bool leaveOpen);
    
    // Destructor
    virtual ~BinaryReader();
    
    // Non-copyable but movable
    BinaryReader(const BinaryReader&) = delete;
    BinaryReader& operator=(const BinaryReader&) = delete;
    BinaryReader(BinaryReader&& other) noexcept;
    BinaryReader& operator=(BinaryReader&& other) noexcept;
    
    // Properties
    [[nodiscard]] Stream* BaseStream() const { return m_stream; }
    
    // Methods
    void Close();
    int PeekChar();
    int Read();
    int Read(std::vector<char>& buffer, int index, int count);
    int Read(std::vector<uint8_t>& buffer, int index, int count);
    
    // Read primitive types
    bool ReadBoolean();
    uint8_t ReadByte();
    int8_t ReadSByte();
    char ReadChar();
    int16_t ReadInt16();
    uint16_t ReadUInt16();
    int32_t ReadInt32();
    uint32_t ReadUInt32();
    int64_t ReadInt64();
    uint64_t ReadUInt64();
    float ReadSingle();
    double ReadDouble();
    String ReadString();
    
    // Read arrays
    std::vector<char> ReadChars(int count);
    std::vector<uint8_t> ReadBytes(int count);

private:
    void EnsureNotClosed() const;
    void Initialize(Stream* input, std::shared_ptr<Text::Encoding> encoding, bool leaveOpen);
};

} // namespace System::IO
