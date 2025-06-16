
#include "System/Drawing/Imaging/ImageCodecInfo.h"
#include <sstream>

namespace System {
namespace Drawing {
namespace Imaging {

// Static members
std::vector<ImageCodecInfo> ImageCodecInfoHelper::_decoders;
std::vector<ImageCodecInfo> ImageCodecInfoHelper::_encoders;
bool ImageCodecInfoHelper::_initialized = false;

ImageCodecInfo::ImageCodecInfo() 
    : Format(ImageFormat::Unknown), Flags(0), Version(1) {
}

ImageCodecInfo::ImageCodecInfo(const std::string& codecName, const std::string& formatDescription,
                               const std::string& filenameExtension, const std::string& mimeType,
                               ImageFormat format)
    : CodecName(codecName), FormatDescription(formatDescription), 
      FilenameExtension(filenameExtension), MimeType(mimeType), 
      Format(format), Flags(0), Version(1) {
}

std::string ImageCodecInfo::ToString() const {
    std::ostringstream oss;
    oss << "ImageCodecInfo: " << CodecName << " (" << FormatDescription << ")";
    return oss.str();
}

void ImageCodecInfoHelper::InitializeCodecs() {
    if (_initialized) return;
    
    // Initialize decoders
    _decoders.clear();
    _decoders.emplace_back("Built-in BMP Decoder", "BMP", "*.BMP;*.DIB;*.RLE", "image/bmp", ImageFormat::Bmp);
    _decoders.emplace_back("Built-in GIF Decoder", "GIF", "*.GIF", "image/gif", ImageFormat::Gif);
    _decoders.emplace_back("Built-in JPEG Decoder", "JPEG", "*.JPG;*.JPEG;*.JPE;*.JFIF", "image/jpeg", ImageFormat::Jpeg);
    _decoders.emplace_back("Built-in PNG Decoder", "PNG", "*.PNG", "image/png", ImageFormat::Png);
    _decoders.emplace_back("Built-in TIFF Decoder", "TIFF", "*.TIF;*.TIFF", "image/tiff", ImageFormat::Tiff);
    _decoders.emplace_back("Built-in WebP Decoder", "WebP", "*.WEBP", "image/webp", ImageFormat::WebP);
    
    // Initialize encoders
    _encoders.clear();
    _encoders.emplace_back("Built-in BMP Encoder", "BMP", "*.BMP;*.DIB;*.RLE", "image/bmp", ImageFormat::Bmp);
    _encoders.emplace_back("Built-in JPEG Encoder", "JPEG", "*.JPG;*.JPEG;*.JPE;*.JFIF", "image/jpeg", ImageFormat::Jpeg);
    _encoders.emplace_back("Built-in PNG Encoder", "PNG", "*.PNG", "image/png", ImageFormat::Png);
    _encoders.emplace_back("Built-in TIFF Encoder", "TIFF", "*.TIF;*.TIFF", "image/tiff", ImageFormat::Tiff);
    _encoders.emplace_back("Built-in WebP Encoder", "WebP", "*.WEBP", "image/webp", ImageFormat::WebP);
    
    _initialized = true;
}

std::vector<ImageCodecInfo> ImageCodecInfoHelper::GetImageDecoders() {
    InitializeCodecs();
    return _decoders;
}

std::vector<ImageCodecInfo> ImageCodecInfoHelper::GetImageEncoders() {
    InitializeCodecs();
    return _encoders;
}

ImageCodecInfo* ImageCodecInfoHelper::GetImageDecoderByFormat(ImageFormat format) {
    InitializeCodecs();
    for (auto& codec : _decoders) {
        if (codec.Format == format) {
            return &codec;
        }
    }
    return nullptr;
}

ImageCodecInfo* ImageCodecInfoHelper::GetImageEncoderByFormat(ImageFormat format) {
    InitializeCodecs();
    for (auto& codec : _encoders) {
        if (codec.Format == format) {
            return &codec;
        }
    }
    return nullptr;
}

ImageCodecInfo* ImageCodecInfoHelper::GetImageDecoderByMimeType(const std::string& mimeType) {
    InitializeCodecs();
    for (auto& codec : _decoders) {
        if (codec.MimeType == mimeType) {
            return &codec;
        }
    }
    return nullptr;
}

ImageCodecInfo* ImageCodecInfoHelper::GetImageEncoderByMimeType(const std::string& mimeType) {
    InitializeCodecs();
    for (auto& codec : _encoders) {
        if (codec.MimeType == mimeType) {
            return &codec;
        }
    }
    return nullptr;
}

ImageCodecInfo* ImageCodecInfoHelper::GetImageDecoderByExtension(const std::string& extension) {
    InitializeCodecs();
    std::string upperExt = extension;
    std::transform(upperExt.begin(), upperExt.end(), upperExt.begin(), ::toupper);
    if (upperExt[0] != '.') {
        upperExt = "." + upperExt;
    }
    
    for (auto& codec : _decoders) {
        if (codec.FilenameExtension.find(upperExt) != std::string::npos) {
            return &codec;
        }
    }
    return nullptr;
}

ImageCodecInfo* ImageCodecInfoHelper::GetImageEncoderByExtension(const std::string& extension) {
    InitializeCodecs();
    std::string upperExt = extension;
    std::transform(upperExt.begin(), upperExt.end(), upperExt.begin(), ::toupper);
    if (upperExt[0] != '.') {
        upperExt = "." + upperExt;
    }
    
    for (auto& codec : _encoders) {
        if (codec.FilenameExtension.find(upperExt) != std::string::npos) {
            return &codec;
        }
    }
    return nullptr;
}

bool ImageCodecInfoHelper::CanDecode(ImageFormat format) {
    return GetImageDecoderByFormat(format) != nullptr;
}

bool ImageCodecInfoHelper::CanEncode(ImageFormat format) {
    return GetImageEncoderByFormat(format) != nullptr;
}

bool ImageCodecInfoHelper::SupportsMultipleFrames(ImageFormat format) {
    switch (format) {
        case ImageFormat::Gif:
        case ImageFormat::Tiff:
            return true;
        default:
            return false;
    }
}

bool ImageCodecInfoHelper::SupportsAnimation(ImageFormat format) {
    return format == ImageFormat::Gif;
}

}
}
}
