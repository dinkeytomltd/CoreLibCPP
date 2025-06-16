
#pragma once
/*
 * DriveType.h
 * C++ implementation of .NET's System.IO.DriveType enumeration
 * 
 * Defines constants for drive types.
 */

namespace System::IO {

enum class DriveType {
    Unknown = 0,        // The type of drive is unknown.
    Removable = 2,      // The drive is a removable storage device, such as a floppy disk drive or a USB flash drive.
    Fixed = 3,          // The drive is a fixed drive.
    Network = 4,        // The drive is a network drive.
    CDRom = 5,          // The drive is an optical disc device, such as a CD or DVD-ROM.
    Ram = 6             // The drive is a RAM drive.
};

} // namespace System::IO
