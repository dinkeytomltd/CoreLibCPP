
#pragma once
/*
 * StreamReader.h
 * C++ implementation of .NET's System.IO.StreamReader class
 * 
 * Implements a TextReader that reads characters from a byte stream in a particular encoding.
 */
#include "../Object.h"
#include "../String.h"
#include "../Text/Encoding.h"
#include "Stream.h"
#include <memory>

namespace System::IO {

class StreamReader : public Object {
private:
    std::unique_ptr<Stream> m_stream;
    bool m_ownStream;
    std::shared_ptr<Text::Encoding> m_encoding;
    std::vector<uint8_t> m_byteBuffer;
    std::vector<char> m_charBuffer;
    int m_byteLen;
    int m_bytePos;
    int m_charLen;
    int m_charPos;
    bool m_detectEncoding;
    bool m_checkPreamble;
    bool m_isBlocked;
    bool m_closable;
    
    static constexpr int DefaultBufferSize = 1024;
    static constexpr int DefaultFileStreamBufferSize = 4096;
    static constexpr int MinBufferSize = 128;

    void Initialize(Stream* stream, std::shared_ptr<Text::Encoding> encoding, bool detectEncodingFromByteOrderMarks, int bufferSize, bool leaveOpen);
    int ReadBuffer();
    bool IsPreamble();
    void CompressBuffer(int n);

public:
    // Constructors
    explicit StreamReader(Stream* stream);
    StreamReader(Stream* stream, bool detectEncodingFromByteOrderMarks);
    StreamReader(Stream* stream, std::shared_ptr<Text::Encoding> encoding);
    StreamReader(Stream* stream, std::shared_ptr<Text::Encoding> encoding, bool detectEncodingFromByteOrderMarks);
    StreamReader(Stream* stream, std::shared_ptr<Text::Encoding> encoding, bool detectEncodingFromByteOrderMarks, int bufferSize);
    StreamReader(Stream* stream, std::shared_ptr<Text::Encoding> encoding, bool detectEncodingFromByteOrderMarks, int bufferSize, bool leaveOpen);
    
    explicit StreamReader(const String& path);
    StreamReader(const String& path, bool detectEncodingFromByteOrderMarks);
    StreamReader(const String& path, std::shared_ptr<Text::Encoding> encoding);
    StreamReader(const String& path, std::shared_ptr<Text::Encoding> encoding, bool detectEncodingFromByteOrderMarks);
    StreamReader(const String& path, std::shared_ptr<Text::Encoding> encoding, bool detectEncodingFromByteOrderMarks, int bufferSize);
    
    // Destructor
    virtual ~StreamReader();
    
    // Non-copyable but movable
    StreamReader(const StreamReader&) = delete;
    StreamReader& operator=(const StreamReader&) = delete;
    StreamReader(StreamReader&& other) noexcept;
    StreamReader& operator=(StreamReader&& other) noexcept;
    
    // Properties
    [[nodiscard]] Stream* BaseStream() const { return m_stream.get(); }
    [[nodiscard]] std::shared_ptr<Text::Encoding> CurrentEncoding() const { return m_encoding; }
    [[nodiscard]] bool EndOfStream() const;
    
    // Methods
    void Close();
    void DiscardBufferedData();
    int Peek();
    int Read();
    int Read(std::vector<char>& buffer, int index, int count);
    String ReadLine();
    String ReadToEnd();

private:
    void EnsureNotClosed() const;
};

} // namespace System::IO
