
#pragma once
/*
 * FileAccess.h
 * C++ implementation of .NET's System.IO.FileAccess enumeration
 * 
 * Defines constants for read, write, or read/write access to a file.
 */

namespace System::IO {

enum class FileAccess {
    Read = 1,       // Read access to the file. Data can be read from the file.
    Write = 2,      // Write access to the file. Data can be written to the file.
    ReadWrite = 3   // Read and write access to the file. Data can be written to and read from the file.
};

} // namespace System::IO
