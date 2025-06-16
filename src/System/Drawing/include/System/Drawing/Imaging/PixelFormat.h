
#pragma once

#include <string>
#include <cstdint>

namespace System {
namespace Drawing {
namespace Imaging {

enum class PixelFormat : uint32_t {
    Undefined = 0,
    DontCare = 0,
    Max = 15,
    Indexed = 0x00010000,
    Gdi = 0x00020000,
    Format16bppRgb555 = 0x00021005,
    Format16bppRgb565 = 0x00021006,
    Format24bppRgb = 0x00021808,
    Format32bppRgb = 0x00022009,
    Format1bppIndexed = 0x00030101,
    Format4bppIndexed = 0x00030402,
    Format8bppIndexed = 0x00030803,
    Alpha = 0x00040000,
    Format16bppArgb1555 = 0x00061007,
    PAlpha = 0x00080000,
    Format16bppGrayScale = 0x00101004,
    Format48bppRgb = 0x00101018,
    Format64bppArgb = 0x00182020,
    Canonical = 0x00200000,
    Format32bppArgb = 0x00262008,
    Format64bppPArgb = 0x001C2020,
    Extended = 0x00100000,
    Format32bppPArgb = 0x000E200B
};

class PixelFormatHelper {
public:
    static std::string ToString(PixelFormat format);
    static PixelFormat FromString(const std::string& formatString);
    
    // Format properties
    static int GetPixelFormatSize(PixelFormat pixfmt);
    static bool IsAlphaPixelFormat(PixelFormat pixfmt);
    static bool IsCanonicalPixelFormat(PixelFormat pixfmt);
    static bool IsExtendedPixelFormat(PixelFormat pixfmt);
    static bool IsIndexedPixelFormat(PixelFormat pixfmt);
    
    // Conversion utilities
    static int GetBitsPerPixel(PixelFormat format);
    static int GetBytesPerPixel(PixelFormat format);
    static bool HasAlpha(PixelFormat format);
    static bool IsPremultiplied(PixelFormat format);
    static bool IsGrayscale(PixelFormat format);
    
    // Standard formats
    static PixelFormat Format1bppIndexed() { return PixelFormat::Format1bppIndexed; }
    static PixelFormat Format4bppIndexed() { return PixelFormat::Format4bppIndexed; }
    static PixelFormat Format8bppIndexed() { return PixelFormat::Format8bppIndexed; }
    static PixelFormat Format16bppGrayScale() { return PixelFormat::Format16bppGrayScale; }
    static PixelFormat Format16bppRgb555() { return PixelFormat::Format16bppRgb555; }
    static PixelFormat Format16bppRgb565() { return PixelFormat::Format16bppRgb565; }
    static PixelFormat Format16bppArgb1555() { return PixelFormat::Format16bppArgb1555; }
    static PixelFormat Format24bppRgb() { return PixelFormat::Format24bppRgb; }
    static PixelFormat Format32bppRgb() { return PixelFormat::Format32bppRgb; }
    static PixelFormat Format32bppArgb() { return PixelFormat::Format32bppArgb; }
    static PixelFormat Format32bppPArgb() { return PixelFormat::Format32bppPArgb; }
    static PixelFormat Format48bppRgb() { return PixelFormat::Format48bppRgb; }
    static PixelFormat Format64bppArgb() { return PixelFormat::Format64bppArgb; }
    static PixelFormat Format64bppPArgb() { return PixelFormat::Format64bppPArgb; }
};

// Bitmap data structure for direct pixel access
struct BitmapData {
    int Width;
    int Height;
    int Stride;
    PixelFormat Format;
    void* Scan0;
    int Reserved;
    
    BitmapData() : Width(0), Height(0), Stride(0), Format(PixelFormat::Undefined), Scan0(nullptr), Reserved(0) {}
};

// Image lock modes
enum class ImageLockMode {
    ReadOnly = 0x0001,
    WriteOnly = 0x0002,
    ReadWrite = ReadOnly | WriteOnly,
    UserInputBuffer = 0x0004
};

}
}
}
