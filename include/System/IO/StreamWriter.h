
#pragma once
/*
 * StreamWriter.h
 * C++ implementation of .NET's System.IO.StreamWriter class
 * 
 * Implements a TextWriter for writing characters to a stream in a particular encoding.
 */
#include "../Object.h"
#include "../String.h"
#include "../Text/Encoding.h"
#include "Stream.h"
#include <memory>

namespace System::IO {

class StreamWriter : public Object {
private:
    std::unique_ptr<Stream> m_stream;
    bool m_ownStream;
    std::shared_ptr<Text::Encoding> m_encoding;
    std::vector<uint8_t> m_byteBuffer;
    std::vector<char> m_charBuffer;
    int m_charPos;
    int m_charLen;
    bool m_autoFlush;
    bool m_haveWrittenPreamble;
    bool m_closable;
    
    static constexpr int DefaultBufferSize = 1024;
    static constexpr int DefaultFileStreamBufferSize = 4096;
    static constexpr int MinBufferSize = 128;

    void Initialize(Stream* stream, std::shared_ptr<Text::Encoding> encoding, int bufferSize, bool leaveOpen);
    void Flush(bool flushStream, bool flushEncoder);

public:
    // Constructors
    explicit StreamWriter(Stream* stream);
    StreamWriter(Stream* stream, std::shared_ptr<Text::Encoding> encoding);
    StreamWriter(Stream* stream, std::shared_ptr<Text::Encoding> encoding, int bufferSize);
    StreamWriter(Stream* stream, std::shared_ptr<Text::Encoding> encoding, int bufferSize, bool leaveOpen);
    
    explicit StreamWriter(const String& path);
    StreamWriter(const String& path, bool append);
    StreamWriter(const String& path, bool append, std::shared_ptr<Text::Encoding> encoding);
    StreamWriter(const String& path, bool append, std::shared_ptr<Text::Encoding> encoding, int bufferSize);
    
    // Destructor
    virtual ~StreamWriter();
    
    // Non-copyable but movable
    StreamWriter(const StreamWriter&) = delete;
    StreamWriter& operator=(const StreamWriter&) = delete;
    StreamWriter(StreamWriter&& other) noexcept;
    StreamWriter& operator=(StreamWriter&& other) noexcept;
    
    // Properties
    [[nodiscard]] bool AutoFlush() const { return m_autoFlush; }
    void SetAutoFlush(bool value) { m_autoFlush = value; if (value) Flush(); }
    [[nodiscard]] Stream* BaseStream() const { return m_stream.get(); }
    [[nodiscard]] std::shared_ptr<Text::Encoding> Encoding() const { return m_encoding; }
    
    // Methods
    void Close();
    void Flush();
    void Write(char value);
    void Write(const String& value);
    void Write(const std::vector<char>& buffer);
    void Write(const std::vector<char>& buffer, int index, int count);
    void WriteLine();
    void WriteLine(char value);
    void WriteLine(const String& value);

private:
    void EnsureNotClosed() const;
    void WriteCore(const std::vector<char>& buffer, int index, int count, bool appendNewLine);
};

} // namespace System::IO
