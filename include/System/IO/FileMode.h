
#pragma once
/*
 * FileMode.h
 * C++ implementation of .NET's System.IO.FileMode enumeration
 * 
 * Specifies how the operating system should open a file.
 */

namespace System::IO {

enum class FileMode {
    CreateNew = 1,      // Creates a new file. The function fails if a specified file exists.
    Create = 2,         // Creates a new file, always. If a file exists, it is overwritten.
    Open = 3,           // Opens an existing file. The function fails if the file does not exist.
    OpenOrCreate = 4,   // Opens a file if it exists; otherwise, creates a new file.
    Truncate = 5,       // Opens an existing file and truncates it so that its size is 0 bytes.
    Append = 6          // Opens the file if it exists and seeks to the end of the file, or creates a new file.
};

} // namespace System::IO
