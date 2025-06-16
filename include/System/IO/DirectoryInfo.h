
#pragma once
/*
 * DirectoryInfo.h
 * C++ implementation of .NET's System.IO.DirectoryInfo class
 * 
 * Exposes instance methods for creating, moving, and enumerating through directories and subdirectories.
 */
#include "../Object.h"
#include "../String.h"
#include "../DateTime.h"
#include "SearchOption.h"
#include <memory>
#include <vector>

namespace System::IO {

// Forward declarations
class FileInfo;

class DirectoryInfo : public Object {
private:
    String m_fullPath;
    String m_originalPath;
    mutable bool m_dataInitialized;
    mutable bool m_exists;
    mutable DateTime m_creationTime;
    mutable DateTime m_lastAccessTime;
    mutable DateTime m_lastWriteTime;

    void RefreshData() const;
    void EnsureDataInitialized() const;

public:
    // Constructors
    explicit DirectoryInfo(const String& path);
    
    // Copy constructor and assignment
    DirectoryInfo(const DirectoryInfo& other);
    DirectoryInfo& operator=(const DirectoryInfo& other);
    DirectoryInfo(DirectoryInfo&& other) noexcept;
    DirectoryInfo& operator=(DirectoryInfo&& other) noexcept;
    
    // Destructor
    virtual ~DirectoryInfo() = default;
    
    // Properties
    [[nodiscard]] bool Exists() const;
    [[nodiscard]] String Name() const;
    [[nodiscard]] String FullName() const { return m_fullPath; }
    [[nodiscard]] std::shared_ptr<DirectoryInfo> Parent() const;
    [[nodiscard]] std::shared_ptr<DirectoryInfo> Root() const;
    
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
    void Create();
    void Delete();
    void Delete(bool recursive);
    void MoveTo(const String& destDirName);
    
    // Enumeration methods
    std::vector<std::shared_ptr<DirectoryInfo>> GetDirectories();
    std::vector<std::shared_ptr<DirectoryInfo>> GetDirectories(const String& searchPattern);
    std::vector<std::shared_ptr<DirectoryInfo>> GetDirectories(const String& searchPattern, SearchOption searchOption);
    
    std::vector<std::shared_ptr<FileInfo>> GetFiles();
    std::vector<std::shared_ptr<FileInfo>> GetFiles(const String& searchPattern);
    std::vector<std::shared_ptr<FileInfo>> GetFiles(const String& searchPattern, SearchOption searchOption);
    
    // Subdirectory creation
    std::shared_ptr<DirectoryInfo> CreateSubdirectory(const String& path);
    
    // Object overrides
    [[nodiscard]] String ToString() const override;
    [[nodiscard]] bool Equals(const Object& obj) const override;
    [[nodiscard]] int GetHashCode() const override;

private:
    void Initialize(const String& path);
};

} // namespace System::IO
