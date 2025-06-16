
#pragma once
/*
 * FileInfo.h
 * C++ implementation of .NET's System.IO.FileInfo class
 * 
 * Provides properties and instance methods for the creation, copying, deletion, moving, and opening of files.
 */
#include "../Object.h"
#include "../String.h"
#include "../DateTime.h"
#include "FileStream.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include <memory>

namespace System::IO {

// Forward declaration
class DirectoryInfo;

class FileInfo : public Object {
private:
    String m_fullPath;
    String m_originalPath;
    mutable bool m_dataInitialized;
    mutable bool m_exists;
    mutable int64_t m_length;
    mutable DateTime m_creationTime;
    mutable DateTime m_lastAccessTime;
    mutable DateTime m_lastWriteTime;

    void RefreshData() const;
    void EnsureDataInitialized() const;

public:
    // Constructors
    explicit FileInfo(const String& fileName);
    
    // Copy constructor and assignment
    FileInfo(const FileInfo& other);
    FileInfo& operator=(const FileInfo& other);
    FileInfo(FileInfo&& other) noexcept;
    FileInfo& operator=(FileInfo&& other) noexcept;
    
    // Destructor
    virtual ~FileInfo() = default;
    
    // Properties
    [[nodiscard]] bool Exists() const;
    [[nodiscard]] String Name() const;
    [[nodiscard]] String FullName() const { return m_fullPath; }
    [[nodiscard]] String Extension() const;
    [[nodiscard]] int64_t Length() const;
    [[nodiscard]] String DirectoryName() const;
    [[nodiscard]] std::shared_ptr<DirectoryInfo> Directory() const;
    [[nodiscard]] bool IsReadOnly() const;
    void SetIsReadOnly(bool value);
    
    // Time properties
    [[nodiscard]] DateTime CreationTime() const;
    void SetCreationTime(const DateTime& value);
    [[nodiscard]] DateTime CreationTimeUtc() const;
    void SetCreationTimeUtc(const DateTime& value);
    [[nodiscard]] DateTime LastAccessTime() const;
    void SetLastAccessTime(const DateTime& value);
    [[nodiscard]] DateTime LastAccessTimeUtc() const;
    void SetLastAccessTimeUtc(const DateTime& value);
    [[nodiscard]] DateTime LastWriteTime() const;
    void SetLastWriteTime(const DateTime& value);
    [[nodiscard]] DateTime LastWriteTimeUtc() const;
    void SetLastWriteTimeUtc(const DateTime& value);
    
    // Methods
    void Refresh();
    void Delete();
    std::shared_ptr<FileInfo> CopyTo(const String& destFileName);
    std::shared_ptr<FileInfo> CopyTo(const String& destFileName, bool overwrite);
    void MoveTo(const String& destFileName);
    
    // File opening methods
    std::unique_ptr<FileStream> Create();
    std::unique_ptr<StreamWriter> CreateText();
    std::unique_ptr<StreamWriter> AppendText();
    std::unique_ptr<FileStream> Open(FileMode mode);
    std::unique_ptr<FileStream> Open(FileMode mode, FileAccess access);
    std::unique_ptr<FileStream> Open(FileMode mode, FileAccess access, FileShare share);
    std::unique_ptr<FileStream> OpenRead();
    std::unique_ptr<FileStream> OpenWrite();
    std::unique_ptr<StreamReader> OpenText();
    
    // Object overrides
    [[nodiscard]] String ToString() const override;
    [[nodiscard]] bool Equals(const Object& obj) const override;
    [[nodiscard]] int GetHashCode() const override;

private:
    void Initialize(const String& fileName);
};

} // namespace System::IO
