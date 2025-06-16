
#pragma once
/*
 * FileStream.h
 * C++ implementation of .NET's System.IO.FileStream class
 * 
 * Provides a Stream for a file, supporting both synchronous read and write operations.
 */
#include "Stream.h"
#include "FileMode.h"
#include "FileAccess.h"
#include "FileShare.h"
#ifdef _WIN32
#include "Win32Helpers.h"
#endif

namespace System::IO {

class FileStream : public Stream {
private:
#ifdef _WIN32
    Win32Helpers::SafeFileHandle m_handle;
#else
    int m_handle;
#endif
    String m_path;
    FileAccess m_access;
    bool m_canRead;
    bool m_canWrite;
    bool m_canSeek;
    int64_t m_position;
    int64_t m_length;
    std::vector<uint8_t> m_buffer;
    int m_bufferSize;
    int m_readPos;
    int m_readLen;
    int m_writePos;
    bool m_bufferDirty;

    void InitializeBuffer(int bufferSize);
    void FlushWriteBuffer();
    void FlushReadBuffer();
    int ReadFromFile(std::vector<uint8_t>& buffer, int offset, int count);
    void WriteToFile(const std::vector<uint8_t>& buffer, int offset, int count);
    void UpdateLength();

public:
    // Constructors
    FileStream(const String& path, FileMode mode);
    FileStream(const String& path, FileMode mode, FileAccess access);
    FileStream(const String& path, FileMode mode, FileAccess access, FileShare share);
    FileStream(const String& path, FileMode mode, FileAccess access, FileShare share, int bufferSize);
    
    // Destructor
    virtual ~FileStream();
    
    // Non-copyable but movable
    FileStream(const FileStream&) = delete;
    FileStream& operator=(const FileStream&) = delete;
    FileStream(FileStream&& other) noexcept;
    FileStream& operator=(FileStream&& other) noexcept;
    
    // Properties (override from Stream)
    [[nodiscard]] bool CanRead() const override { return m_canRead; }
    [[nodiscard]] bool CanSeek() const override { return m_canSeek; }
    [[nodiscard]] bool CanWrite() const override { return m_canWrite; }
    [[nodiscard]] int64_t Length() const override;
    [[nodiscard]] int64_t Position() const override { return m_position; }
    void SetPosition(int64_t value) override;
    
    // FileStream-specific properties
    [[nodiscard]] String Name() const { return m_path; }
    [[nodiscard]] bool IsAsync() const { return false; } // Simplified - no async support initially
    
    // Methods (override from Stream)
    void Flush() override;
    int Read(std::vector<uint8_t>& buffer, int offset, int count) override;
    int64_t Seek(int64_t offset, SeekOrigin origin) override;
    void SetLength(int64_t value) override;
    void Write(const std::vector<uint8_t>& buffer, int offset, int count) override;
    void Close() override;
    
    // FileStream-specific methods
    void Lock(int64_t position, int64_t length);
    void Unlock(int64_t position, int64_t length);

private:
    void Initialize(const String& path, FileMode mode, FileAccess access, FileShare share, int bufferSize);
    void ValidateParameters(const std::vector<uint8_t>& buffer, int offset, int count) const;
};

} // namespace System::IO
