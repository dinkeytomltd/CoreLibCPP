
#pragma once
/*
 * SearchOption.h
 * C++ implementation of .NET's System.IO.SearchOption enumeration
 * 
 * Specifies whether to search the current directory, or the current directory and all subdirectories.
 */

namespace System::IO {

enum class SearchOption {
    TopDirectoryOnly = 0,   // Includes only the current directory in a search operation.
    AllDirectories = 1      // Includes the current directory and all its subdirectories in a search operation.
};

} // namespace System::IO
