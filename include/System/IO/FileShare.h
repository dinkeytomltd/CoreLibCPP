
#pragma once
/*
 * FileShare.h
 * C++ implementation of .NET's System.IO.FileShare enumeration
 * 
 * Contains constants for controlling the kind of access other operations can have to the same file.
 */

namespace System::IO {

enum class FileShare {
    None = 0,           // Declines sharing of the current file.
    Read = 1,           // Allows subsequent opening of the file for reading.
    Write = 2,          // Allows subsequent opening of the file for writing.
    ReadWrite = 3,      // Allows subsequent opening of the file for reading or writing.
    Delete = 4,         // Allows subsequent deleting of a file.
    Inheritable = 16    // Makes the file handle inheritable by child processes.
};

} // namespace System::IO
