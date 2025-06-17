
#include "System/Drawing/Imaging/ImageFormat.h"
#include <algorithm>
#include <unordered_map>

namespace System {
namespace Drawing {
namespace Imaging {

std::string ImageFormatHelper::ToString(ImageFormat format) {
    switch (format) {
        case ImageFormat::Bmp: return "Bmp";
        case ImageFormat::Emf: return "Emf";
        case ImageFormat::Exif: return "Exif";
        case ImageFormat::Gif: return "Gif";
        case ImageFormat::Icon: return "Icon";
        case ImageFormat::Jpeg: return "Jpeg";
        case ImageFormat::MemoryBmp: return "MemoryBmp";
        case ImageFormat::Png: return "Png";
        case ImageFormat::Tiff: return "Tiff";
        case ImageFormat::Wmf: return "Wmf";
        case ImageFormat::WebP: return "WebP";
        default: return "Unknown";
    }
}

ImageFormat ImageFormatHelper::FromString(const std::string& formatString) {
    std::string lower = formatString;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "bmp") return ImageFormat::Bmp;
    if (lower == "emf") return ImageFormat::Emf;
    if (lower == "exif") return ImageFormat::Exif;
    if (lower == "gif") return ImageFormat::Gif;
    if (lower == "icon" || lower == "ico") return ImageFormat::Icon;
    if (lower == "jpeg" || lower == "jpg") return ImageFormat::Jpeg;
    if (lower == "memorybmp") return ImageFormat::MemoryBmp;
    if (lower == "png") return ImageFormat::Png;
    if (lower == "tiff" || lower == "tif") return ImageFormat::Tiff;
    if (lower == "wmf") return ImageFormat::Wmf;
    if (lower == "webp") return ImageFormat::WebP;
    
    return ImageFormat::Unknown;
}

ImageFormat ImageFormatHelper::FromFileExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return ImageFormat::Unknown;
    }
    
    std::string extension = filename.substr(dotPos + 1);
    return FromString(extension);
}

std::string ImageFormatHelper::GetDefaultExtension(ImageFormat format) {
    switch (format) {
        case ImageFormat::Bmp: return ".bmp";
        case ImageFormat::Emf: return ".emf";
        case ImageFormat::Exif: return ".exif";
        case ImageFormat::Gif: return ".gif";
        case ImageFormat::Icon: return ".ico";
        case ImageFormat::Jpeg: return ".jpg";
        case ImageFormat::MemoryBmp: return ".bmp";
        case ImageFormat::Png: return ".png";
        case ImageFormat::Tiff: return ".tiff";
        case ImageFormat::Wmf: return ".wmf";
        case ImageFormat::WebP: return ".webp";
        default: return "";
    }
}

std::string ImageFormatHelper::GetMimeType(ImageFormat format) {
    switch (format) {
        case ImageFormat::Bmp: return "image/bmp";
        case ImageFormat::Emf: return "image/x-emf";
        case ImageFormat::Exif: return "image/jpeg";
        case ImageFormat::Gif: return "image/gif";
        case ImageFormat::Icon: return "image/x-icon";
        case ImageFormat::Jpeg: return "image/jpeg";
        case ImageFormat::MemoryBmp: return "image/bmp";
        case ImageFormat::Png: return "image/png";
        case ImageFormat::Tiff: return "image/tiff";
        case ImageFormat::Wmf: return "image/x-wmf";
        case ImageFormat::WebP: return "image/webp";
        default: return "application/octet-stream";
    }
}

bool ImageFormatHelper::IsSupported(ImageFormat format) {
    switch (format) {
        case ImageFormat::Bmp:
        case ImageFormat::Gif:
        case ImageFormat::Jpeg:
        case ImageFormat::Png:
        case ImageFormat::Tiff:
        case ImageFormat::WebP:
            return true;
        case ImageFormat::Emf:
        case ImageFormat::Exif:
        case ImageFormat::Icon:
        case ImageFormat::MemoryBmp:
        case ImageFormat::Wmf:
            return false; // Not yet implemented
        default:
            return false;
    }
}

}
}
}
