
#pragma once
/*
 * Directory.h
 * C++ implementation of .NET's System.IO.Directory class
 * 
 * Exposes static methods for creating, moving, and enumerating through directories and subdirectories.
 */
#include "../String.h"
#include "../DateTime.h"
#include "SearchOption.h"
#include <vector>

namespace System::IO {

class Directory {
public:
    // Directory cannot be instantiated
    Directory() = delete;
    
    // Directory existence and creation
    static bool Exists(const String& path);
    static void CreateDirectory(const String& path);
    static void Delete(const String& path);
    static void Delete(const String& path, bool recursive);
    static void Move(const String& sourceDirName, const String& destDirName);
    
    // Directory enumeration
    static std::vector<String> GetDirectories(const String& path);
    static std::vector<String> GetDirectories(const String& path, const String& searchPattern);
    static std::vector<String> GetDirectories(const String& path, const String& searchPattern, SearchOption searchOption);
    
    static std::vector<String> GetFiles(const String& path);
    static std::vector<String> GetFiles(const String& path, const String& searchPattern);
    static std::vector<String> GetFiles(const String& path, const String& searchPattern, SearchOption searchOption);
    
    static std::vector<String> GetFileSystemEntries(const String& path);
    static std::vector<String> GetFileSystemEntries(const String& path, const String& searchPattern);
    static std::vector<String> GetFileSystemEntries(const String& path, const String& searchPattern, SearchOption searchOption);
    
    // Directory properties
    static String GetCurrentDirectory();
    static void SetCurrentDirectory(const String& path);
    static String GetDirectoryRoot(const String& path);
    static std::vector<String> GetLogicalDrives();
    
    // Directory attributes and times
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
    static void EnsureDirectoryExists(const String& path);
    static std::vector<String> InternalGetFileSystemEntries(const String& path, const String& searchPattern, SearchOption searchOption, bool includeFiles, bool includeDirs);
    static bool MatchesPattern(const String& name, const String& pattern);
};

} // namespace System::IO
