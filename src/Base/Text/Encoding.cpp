
/*
 * Encoding.cpp
 * Implementation file for System.Text.Encoding
 */
#include "../../../include/System/Text/Encoding.h"
#include "../../../include/System/Text/UTF8Encoding.h"
#include "../../../include/System/Text/ASCIIEncoding.h"

namespace System {
namespace Text {

// Static instances for common encodings
static UTF8Encoding utf8Instance;
static ASCIIEncoding asciiInstance;

const Encoding& Encoding::UTF8() {
    return utf8Instance;
}

const Encoding& Encoding::ASCII() {
    return asciiInstance;
}

} // namespace Text
} // namespace System
