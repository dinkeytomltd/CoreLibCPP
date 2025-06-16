
#pragma once
/*
 * Path.h
 * C++ implementation of .NET's System.IO.Path class
 * 
 * Performs operations on String instances that contain file or directory path information.
 */
#include "../String.h"
#include <vector>

namespace System::IO {

class Path {
public:
    // Path cannot be instantiated
    Path() = delete;
    
    // Path separators and invalid characters
    static const char DirectorySeparatorChar;
    static const char AltDirectorySeparatorChar;
    static const char VolumeSeparatorChar;
    static const char PathSeparator;
    
    static std::vector<char> GetInvalidFileNameChars();
    static std::vector<char> GetInvalidPathChars();
    
    // Path manipulation methods
    static String ChangeExtension(const String& path, const String& extension);
    static String Combine(const String& path1, const String& path2);
    static String Combine(const String& path1, const String& path2, const String& path3);
    static String Combine(const String& path1, const String& path2, const String& path3, const String& path4);
    static String Combine(const std::vector<String>& paths);
    
    // Path component extraction
    static String GetDirectoryName(const String& path);
    static String GetExtension(const String& path);
    static String GetFileName(const String& path);
    static String GetFileNameWithoutExtension(const String& path);
    static String GetFullPath(const String& path);
    static String GetPathRoot(const String& path);
    
    // Path validation
    static bool HasExtension(const String& path);
    static bool IsPathRooted(const String& path);
    
    // Temporary file operations
    static String GetTempFileName();
    static String GetTempPath();
    
    // Random file name generation
    static String GetRandomFileName();

private:
    static void CheckInvalidPathChars(const String& path);
    static bool IsDirectorySeparator(char c);
    static bool IsValidDriveChar(char value);
    static String InternalCombine(const String& path1, const String& path2);
    static String NormalizePath(const String& path);
    static String RemoveRelativeSegments(const String& path);
    static int GetRootLength(const String& path);
    
#ifdef _WIN32
    static String GetFullPathWindows(const String& path);
    static bool IsUncPath(const String& path);
    static bool IsDevice(const String& path);
#else
    static String GetFullPathUnix(const String& path);
#endif
};

} // namespace System::IO
