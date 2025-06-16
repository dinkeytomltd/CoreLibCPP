
#pragma once

#include <string>

namespace System {
namespace Drawing {
namespace Imaging {

enum class ImageFormat {
    Unknown = 0,
    Bmp = 1,
    Emf = 2,
    Exif = 3,
    Gif = 4,
    Icon = 5,
    Jpeg = 6,
    MemoryBmp = 7,
    Png = 8,
    Tiff = 9,
    Wmf = 10,
    WebP = 11
};

class ImageFormatHelper {
public:
    static std::string ToString(ImageFormat format);
    static ImageFormat FromString(const std::string& formatString);
    static ImageFormat FromFileExtension(const std::string& filename);
    static std::string GetDefaultExtension(ImageFormat format);
    static std::string GetMimeType(ImageFormat format);
    static bool IsSupported(ImageFormat format);
    
    // Standard format instances
    static ImageFormat Bmp() { return ImageFormat::Bmp; }
    static ImageFormat Emf() { return ImageFormat::Emf; }
    static ImageFormat Exif() { return ImageFormat::Exif; }
    static ImageFormat Gif() { return ImageFormat::Gif; }
    static ImageFormat Icon() { return ImageFormat::Icon; }
    static ImageFormat Jpeg() { return ImageFormat::Jpeg; }
    static ImageFormat MemoryBmp() { return ImageFormat::MemoryBmp; }
    static ImageFormat Png() { return ImageFormat::Png; }
    static ImageFormat Tiff() { return ImageFormat::Tiff; }
    static ImageFormat Wmf() { return ImageFormat::Wmf; }
    static ImageFormat WebP() { return ImageFormat::WebP; }
};

}
}
}
