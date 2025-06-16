
#pragma once

// CoreLib.IO - System.IO namespace
// This header provides a convenient way to include all IO module functionality

#include "CoreLib/Base.h"

// IO Enumerations
#include "System/IO/FileMode.h"
#include "System/IO/FileAccess.h"
#include "System/IO/FileShare.h"
#include "System/IO/SearchOption.h"
#include "System/IO/DriveType.h"
#include "System/IO/SeekOrigin.h"

// IO Exceptions
#include "System/IO/IOException.h"
#include "System/IO/FileNotFoundException.h"
#include "System/IO/DirectoryNotFoundException.h"
#include "System/IO/EndOfStreamException.h"

// Streams
#include "System/IO/Stream.h"
#include "System/IO/FileStream.h"
#include "System/IO/MemoryStream.h"
#include "System/IO/StreamReader.h"
#include "System/IO/StreamWriter.h"
#include "System/IO/BinaryReader.h"
#include "System/IO/BinaryWriter.h"

// File system
#include "System/IO/File.h"
#include "System/IO/Directory.h"
#include "System/IO/FileInfo.h"
#include "System/IO/DirectoryInfo.h"
#include "System/IO/Path.h"
#include "System/IO/DriveInfo.h"

// Platform helpers
#include "System/IO/Win32Helpers.h"
