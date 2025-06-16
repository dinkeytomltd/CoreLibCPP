
#pragma once

// CoreLib.Base - Essential System types
// This header provides a convenient way to include all Base module functionality

// Core System types
#include "System/Object.h"
#include "System/String.h"
#include "System/Exception.h"

// Primitive types
#include "System/Boolean.h"
#include "System/Byte.h"
#include "System/SByte.h"
#include "System/Int16.h"
#include "System/Int32.h"
#include "System/Int64.h"
#include "System/UInt16.h"
#include "System/UInt32.h"
#include "System/UInt64.h"
#include "System/Single.h"
#include "System/Double.h"
#include "System/Decimal.h"
#include "System/Char.h"
#include "System/IntPtr.h"
#include "System/UIntPtr.h"

// Date and Time
#include "System/DateTime.h"
#include "System/DateTimeOffset.h"
#include "System/TimeSpan.h"

// Text processing
#include "System/Text/StringBuilder.h"
#include "System/Text/Encoding.h"
#include "System/Text/UTF8Encoding.h"
#include "System/Text/ASCIIEncoding.h"
#include "System/Text/RegularExpressions/Regex.h"

// Enumerations
#include "System/Enum.h"
#include "System/DayOfWeek.h"
#include "System/FileAttributes.h"

// Exceptions
#include "System/SystemException.h"
#include "System/ApplicationException.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/InvalidOperationException.h"
#include "System/NotSupportedException.h"
#include "System/NotImplementedException.h"
#include "System/IndexOutOfRangeException.h"
#include "System/NullReferenceException.h"
#include "System/FormatException.h"
#include "System/OverflowException.h"
#include "System/DivideByZeroException.h"

// LINQ and Coroutines
#include "System/Linq/Linq.hpp"
#include "System/Coroutines/Generator.hpp"
#include "System/Coroutines/GeneratorExtensions.hpp"

// GUID
#include "System/Guid.h"

// Delegates
#include "System/Delegate.h"
#ifdef CORELIB_REFLECTION_AVAILABLE
#include "System/DelegateReflection.h"
#endif

// Runtime services
#include "System/Runtime/CompilerServices/SynchronizationContext.h"
