
#pragma once
/*
 * Win32Helpers.h
 * Windows API helper functions for System.IO implementation
 * 
 * Provides C++ wrappers around Windows file I/O APIs
 */
#ifdef _WIN32
#include <windows.h>
#include <string>
#include "../String.h"
#include "FileMode.h"
#include "FileAccess.h"
#include "FileShare.h"

namespace System::IO::Win32Helpers {

// Convert .NET enums to Windows API constants
DWORD GetDesiredAccess(FileAccess access);
DWORD GetShareMode(FileShare share);
DWORD GetCreationDisposition(FileMode mode);
DWORD GetFlagsAndAttributes();

// Convert String to wide string for Windows APIs
std::wstring StringToWString(const String& str);
String WStringToString(const std::wstring& wstr);

// Safe handle wrapper for Windows file handles
class SafeFileHandle {
private:
    HANDLE m_handle;
    bool m_ownsHandle;

public:
    explicit SafeFileHandle(HANDLE handle = INVALID_HANDLE_VALUE, bool ownsHandle = true);
    ~SafeFileHandle();
    
    // Non-copyable but movable
    SafeFileHandle(const SafeFileHandle&) = delete;
    SafeFileHandle& operator=(const SafeFileHandle&) = delete;
    SafeFileHandle(SafeFileHandle&& other) noexcept;
    SafeFileHandle& operator=(SafeFileHandle&& other) noexcept;
    
    [[nodiscard]] HANDLE Get() const { return m_handle; }
    [[nodiscard]] bool IsValid() const { return m_handle != INVALID_HANDLE_VALUE; }
    void Close();
    HANDLE Release();
};

// File operations
SafeFileHandle CreateFileHandle(const String& path, FileMode mode, FileAccess access, FileShare share);
bool ReadFileData(HANDLE handle, void* buffer, DWORD bytesToRead, DWORD& bytesRead);
bool WriteFileData(HANDLE handle, const void* buffer, DWORD bytesToWrite, DWORD& bytesWritten);
int64_t GetFileSize(HANDLE handle);
int64_t GetFilePosition(HANDLE handle);
int64_t SetFilePosition(HANDLE handle, int64_t offset, DWORD moveMethod);
bool FlushFileBuffers(HANDLE handle);
bool SetEndOfFile(HANDLE handle);

// Error handling
void ThrowIOExceptionForLastError(const String& path = String(""));
void ThrowIOExceptionForError(DWORD error, const String& path = String(""));

} // namespace System::IO::Win32Helpers

#endif // _WIN32
