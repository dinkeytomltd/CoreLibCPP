
#include "System/Drawing/Imaging/PixelFormat.h"
#include <algorithm>

namespace System {
namespace Drawing {
namespace Imaging {

std::string PixelFormatHelper::ToString(PixelFormat format) {
    switch (format) {
        case PixelFormat::Undefined: return "Undefined";
        case PixelFormat::DontCare: return "DontCare";
        case PixelFormat::Format1bppIndexed: return "Format1bppIndexed";
        case PixelFormat::Format4bppIndexed: return "Format4bppIndexed";
        case PixelFormat::Format8bppIndexed: return "Format8bppIndexed";
        case PixelFormat::Format16bppGrayScale: return "Format16bppGrayScale";
        case PixelFormat::Format16bppRgb555: return "Format16bppRgb555";
        case PixelFormat::Format16bppRgb565: return "Format16bppRgb565";
        case PixelFormat::Format16bppArgb1555: return "Format16bppArgb1555";
        case PixelFormat::Format24bppRgb: return "Format24bppRgb";
        case PixelFormat::Format32bppRgb: return "Format32bppRgb";
        case PixelFormat::Format32bppArgb: return "Format32bppArgb";
        case PixelFormat::Format32bppPArgb: return "Format32bppPArgb";
        case PixelFormat::Format48bppRgb: return "Format48bppRgb";
        case PixelFormat::Format64bppArgb: return "Format64bppArgb";
        case PixelFormat::Format64bppPArgb: return "Format64bppPArgb";
        default: return "Unknown";
    }
}

PixelFormat PixelFormatHelper::FromString(const std::string& formatString) {
    std::string lower = formatString;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "format1bppindexed") return PixelFormat::Format1bppIndexed;
    if (lower == "format4bppindexed") return PixelFormat::Format4bppIndexed;
    if (lower == "format8bppindexed") return PixelFormat::Format8bppIndexed;
    if (lower == "format16bppgrayscale") return PixelFormat::Format16bppGrayScale;
    if (lower == "format16bpprgb555") return PixelFormat::Format16bppRgb555;
    if (lower == "format16bpprgb565") return PixelFormat::Format16bppRgb565;
    if (lower == "format16bppargb1555") return PixelFormat::Format16bppArgb1555;
    if (lower == "format24bpprgb") return PixelFormat::Format24bppRgb;
    if (lower == "format32bpprgb") return PixelFormat::Format32bppRgb;
    if (lower == "format32bppargb") return PixelFormat::Format32bppArgb;
    if (lower == "format32bpppargb") return PixelFormat::Format32bppPArgb;
    if (lower == "format48bpprgb") return PixelFormat::Format48bppRgb;
    if (lower == "format64bppargb") return PixelFormat::Format64bppArgb;
    if (lower == "format64bpppargb") return PixelFormat::Format64bppPArgb;
    
    return PixelFormat::Undefined;
}

int PixelFormatHelper::GetPixelFormatSize(PixelFormat pixfmt) {
    switch (pixfmt) {
        case PixelFormat::Format1bppIndexed: return 1;
        case PixelFormat::Format4bppIndexed: return 4;
        case PixelFormat::Format8bppIndexed: return 8;
        case PixelFormat::Format16bppGrayScale:
        case PixelFormat::Format16bppRgb555:
        case PixelFormat::Format16bppRgb565:
        case PixelFormat::Format16bppArgb1555: return 16;
        case PixelFormat::Format24bppRgb: return 24;
        case PixelFormat::Format32bppRgb:
        case PixelFormat::Format32bppArgb:
        case PixelFormat::Format32bppPArgb: return 32;
        case PixelFormat::Format48bppRgb: return 48;
        case PixelFormat::Format64bppArgb:
        case PixelFormat::Format64bppPArgb: return 64;
        default: return 0;
    }
}

bool PixelFormatHelper::IsAlphaPixelFormat(PixelFormat pixfmt) {
    return (static_cast<uint32_t>(pixfmt) & static_cast<uint32_t>(PixelFormat::Alpha)) != 0;
}

bool PixelFormatHelper::IsCanonicalPixelFormat(PixelFormat pixfmt) {
    return (static_cast<uint32_t>(pixfmt) & static_cast<uint32_t>(PixelFormat::Canonical)) != 0;
}

bool PixelFormatHelper::IsExtendedPixelFormat(PixelFormat pixfmt) {
    return (static_cast<uint32_t>(pixfmt) & static_cast<uint32_t>(PixelFormat::Extended)) != 0;
}

bool PixelFormatHelper::IsIndexedPixelFormat(PixelFormat pixfmt) {
    return (static_cast<uint32_t>(pixfmt) & static_cast<uint32_t>(PixelFormat::Indexed)) != 0;
}

int PixelFormatHelper::GetBitsPerPixel(PixelFormat format) {
    return GetPixelFormatSize(format);
}

int PixelFormatHelper::GetBytesPerPixel(PixelFormat format) {
    int bits = GetBitsPerPixel(format);
    return (bits + 7) / 8; // Round up to nearest byte
}

bool PixelFormatHelper::HasAlpha(PixelFormat format) {
    switch (format) {
        case PixelFormat::Format16bppArgb1555:
        case PixelFormat::Format32bppArgb:
        case PixelFormat::Format32bppPArgb:
        case PixelFormat::Format64bppArgb:
        case PixelFormat::Format64bppPArgb:
            return true;
        default:
            return false;
    }
}

bool PixelFormatHelper::IsPremultiplied(PixelFormat format) {
    return (static_cast<uint32_t>(format) & static_cast<uint32_t>(PixelFormat::PAlpha)) != 0;
}

bool PixelFormatHelper::IsGrayscale(PixelFormat format) {
    return format == PixelFormat::Format16bppGrayScale;
}

}
}
}
