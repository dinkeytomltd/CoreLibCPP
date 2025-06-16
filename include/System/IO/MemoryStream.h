
#pragma once
/*
 * MemoryStream.h
 * C++ implementation of .NET's System.IO.MemoryStream class
 * 
 * Creates a stream whose backing store is memory.
 */
#include "Stream.h"

namespace System::IO {

class MemoryStream : public Stream {
private:
    std::vector<uint8_t> m_buffer;
    int64_t m_position;
    int64_t m_length;
    int64_t m_capacity;
    bool m_expandable;
    bool m_writable;
    bool m_exposable;
    bool m_isOpen;

    void EnsureCapacity(int64_t value);
    void SetCapacityInternal(int64_t value);

public:
    // Constructors
    MemoryStream();
    explicit MemoryStream(int capacity);
    explicit MemoryStream(const std::vector<uint8_t>& buffer);
    MemoryStream(const std::vector<uint8_t>& buffer, bool writable);
    MemoryStream(const std::vector<uint8_t>& buffer, int index, int count);
    MemoryStream(const std::vector<uint8_t>& buffer, int index, int count, bool writable);
    MemoryStream(const std::vector<uint8_t>& buffer, int index, int count, bool writable, bool publiclyVisible);
    
    // Destructor
    virtual ~MemoryStream() = default;
    
    // Copy constructor and assignment
    MemoryStream(const MemoryStream& other);
    MemoryStream& operator=(const MemoryStream& other);
    MemoryStream(MemoryStream&& other) noexcept;
    MemoryStream& operator=(MemoryStream&& other) noexcept;
    
    // Properties (override from Stream)
    [[nodiscard]] bool CanRead() const override { return m_isOpen; }
    [[nodiscard]] bool CanSeek() const override { return m_isOpen; }
    [[nodiscard]] bool CanWrite() const override { return m_writable && m_isOpen; }
    [[nodiscard]] int64_t Length() const override;
    [[nodiscard]] int64_t Position() const override;
    void SetPosition(int64_t value) override;
    
    // MemoryStream-specific properties
    [[nodiscard]] int64_t Capacity() const { return m_capacity; }
    void SetCapacity(int64_t value);
    
    // Methods (override from Stream)
    void Flush() override;
    int Read(std::vector<uint8_t>& buffer, int offset, int count) override;
    int ReadByte() override;
    int64_t Seek(int64_t offset, SeekOrigin origin) override;
    void SetLength(int64_t value) override;
    void Write(const std::vector<uint8_t>& buffer, int offset, int count) override;
    void WriteByte(uint8_t value) override;
    void Close() override;
    
    // MemoryStream-specific methods
    std::vector<uint8_t> ToArray() const;
    std::vector<uint8_t>& GetBuffer();
    const std::vector<uint8_t>& GetBuffer() const;
    void WriteTo(Stream& stream);

private:
    void Initialize(const std::vector<uint8_t>& buffer, int index, int count, bool writable, bool publiclyVisible);
    void EnsureNotClosed() const;
    void EnsureWriteable() const;
};

} // namespace System::IO
