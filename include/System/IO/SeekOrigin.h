
#pragma once
/*
 * SeekOrigin.h
 * C++ implementation of .NET's System.IO.SeekOrigin enumeration
 * 
 * Specifies the position in a stream to use for seeking.
 */

namespace System::IO {

enum class SeekOrigin {
    Begin = 0,      // Specifies the beginning of a stream.
    Current = 1,    // Specifies the current position within a stream.
    End = 2         // Specifies the end of a stream.
};

} // namespace System::IO
