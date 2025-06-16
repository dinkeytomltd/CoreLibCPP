
#pragma once
/*
 * File.h
 * C++ implementation of .NET's System.IO.File class
 * 
 * Provides static methods for the creation, copying, deletion, moving, and opening of a single file.
 */
#include "../String.h"
#include "../DateTime.h"
#include "../Text/Encoding.h"
#include "FileStream.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include <vector>
#include <memory>

namespace System::IO {

class File {
public:
    // File cannot be instantiated
    File() = delete;
    
    // File existence and properties
    static bool Exists(const String& path);
    
    // File creation and opening
    static std::unique_ptr<FileStream> Create(const String& path);
    static std::unique_ptr<FileStream> Create(const String& path, int bufferSize);
    static std::unique_ptr<FileStream> Open(const String& path, FileMode mode);
    static std::unique_ptr<FileStream> Open(const String& path, FileMode mode, FileAccess access);
    static std::unique_ptr<FileStream> Open(const String& path, FileMode mode, FileAccess access, FileShare share);
    static std::unique_ptr<FileStream> OpenRead(const String& path);
    static std::unique_ptr<FileStream> OpenWrite(const String& path);
    
    // Text file operations
    static std::unique_ptr<StreamReader> OpenText(const String& path);
    static std::unique_ptr<StreamWriter> CreateText(const String& path);
    static std::unique_ptr<StreamWriter> AppendText(const String& path);
    
    // Read operations
    static std::vector<uint8_t> ReadAllBytes(const String& path);
    static std::vector<String> ReadAllLines(const String& path);
    static std::vector<String> ReadAllLines(const String& path, std::shared_ptr<Text::Encoding> encoding);
    static String ReadAllText(const String& path);
    static String ReadAllText(const String& path, std::shared_ptr<Text::Encoding> encoding);
    
    // Write operations
    static void WriteAllBytes(const String& path, const std::vector<uint8_t>& bytes);
    static void WriteAllLines(const String& path, const std::vector<String>& contents);
    static void WriteAllLines(const String& path, const std::vector<String>& contents, std::shared_ptr<Text::Encoding> encoding);
    static void WriteAllText(const String& path, const String& contents);
    static void WriteAllText(const String& path, const String& contents, std::shared_ptr<Text::Encoding> encoding);
    
    // Append operations
    static void AppendAllLines(const String& path, const std::vector<String>& contents);
    static void AppendAllLines(const String& path, const std::vector<String>& contents, std::shared_ptr<Text::Encoding> encoding);
    static void AppendAllText(const String& path, const String& contents);
    static void AppendAllText(const String& path, const String& contents, std::shared_ptr<Text::Encoding> encoding);
    
    // File operations
    static void Copy(const String& sourceFileName, const String& destFileName);
    static void Copy(const String& sourceFileName, const String& destFileName, bool overwrite);
    static void Delete(const String& path);
    static void Move(const String& sourceFileName, const String& destFileName);
    
    // File attributes and times
    static DateTime GetCreationTime(const String& path);
    static DateTime GetCreationTimeUtc(const String& path);
    static DateTime GetLastAccessTime(const String& path);
    static DateTime GetLastAccessTimeUtc(const String& path);
    static DateTime GetLastWriteTime(const String& path);
    static DateTime GetLastWriteTimeUtc(const String& path);
    
    static void SetCreationTime(const String& path, const DateTime& creationTime);
    static void SetCreationTimeUtc(const String& path, const DateTime& creationTimeUtc);
    static void SetLastAccessTime(const String& path, const DateTime& lastAccessTime);
    static void SetLastAccessTimeUtc(const String& path, const DateTime& lastAccessTimeUtc);
    static void SetLastWriteTime(const String& path, const DateTime& lastWriteTime);
    static void SetLastWriteTimeUtc(const String& path, const DateTime& lastWriteTimeUtc);

private:
    static void ValidatePath(const String& path);
    static void EnsureFileExists(const String& path);
};

} // namespace System::IO
