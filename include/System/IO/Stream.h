
#pragma once
/*
 * Stream.h
 * C++ implementation of .NET's System.IO.Stream abstract class
 * 
 * Provides a generic view of a sequence of bytes. This is an abstract class.
 */
#include "../Object.h"
#include "../String.h"
#include "SeekOrigin.h"
#include <memory>
#include <vector>

namespace System::IO {

class Stream : public Object {
public:
    // Virtual destructor
    virtual ~Stream() = default;
    
    // Abstract properties (pure virtual)
    [[nodiscard]] virtual bool CanRead() const = 0;
    [[nodiscard]] virtual bool CanSeek() const = 0;
    [[nodiscard]] virtual bool CanWrite() const = 0;
    [[nodiscard]] virtual int64_t Length() const = 0;
    [[nodiscard]] virtual int64_t Position() const = 0;
    virtual void SetPosition(int64_t value) = 0;
    
    // Abstract methods (pure virtual)
    virtual void Flush() = 0;
    virtual int Read(std::vector<uint8_t>& buffer, int offset, int count) = 0;
    virtual int64_t Seek(int64_t offset, SeekOrigin origin) = 0;
    virtual void SetLength(int64_t value) = 0;
    virtual void Write(const std::vector<uint8_t>& buffer, int offset, int count) = 0;
    
    // Virtual methods with default implementations
    virtual void Close();
    virtual void CopyTo(Stream& destination);
    virtual void CopyTo(Stream& destination, int bufferSize);
    virtual int ReadByte();
    virtual void WriteByte(uint8_t value);
    
    // Convenience methods
    std::vector<uint8_t> ReadAllBytes();
    void WriteAllBytes(const std::vector<uint8_t>& bytes);
    
protected:
    // Protected constructor (abstract class)
    Stream() = default;
    
    // Copy constructor and assignment (protected)
    Stream(const Stream& other) = default;
    Stream& operator=(const Stream& other) = default;
    
private:
    static constexpr int DefaultCopyBufferSize = 81920; // 80KB
};

} // namespace System::IO
