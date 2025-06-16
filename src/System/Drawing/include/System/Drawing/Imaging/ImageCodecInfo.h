
#pragma once

#include "ImageFormat.h"
#include <string>
#include <vector>

namespace System {
namespace Drawing {
namespace Imaging {

struct ImageCodecInfo {
    std::string Clsid;
    std::string FormatID;
    std::string CodecName;
    std::string DllName;
    std::string FormatDescription;
    std::string FilenameExtension;
    std::string MimeType;
    ImageFormat Format;
    uint32_t Flags;
    uint32_t Version;
    std::vector<std::string> SignaturePatterns;
    std::vector<std::string> SignatureMasks;
    
    ImageCodecInfo();
    ImageCodecInfo(const std::string& codecName, const std::string& formatDescription,
                   const std::string& filenameExtension, const std::string& mimeType,
                   ImageFormat format);
    
    // String representation
    std::string ToString() const;
};

class ImageCodecInfoHelper {
public:
    // Get available codecs
    static std::vector<ImageCodecInfo> GetImageDecoders();
    static std::vector<ImageCodecInfo> GetImageEncoders();
    
    // Find specific codecs
    static ImageCodecInfo* GetImageDecoderByFormat(ImageFormat format);
    static ImageCodecInfo* GetImageEncoderByFormat(ImageFormat format);
    static ImageCodecInfo* GetImageDecoderByMimeType(const std::string& mimeType);
    static ImageCodecInfo* GetImageEncoderByMimeType(const std::string& mimeType);
    static ImageCodecInfo* GetImageDecoderByExtension(const std::string& extension);
    static ImageCodecInfo* GetImageEncoderByExtension(const std::string& extension);
    
    // Codec capabilities
    static bool CanDecode(ImageFormat format);
    static bool CanEncode(ImageFormat format);
    static bool SupportsMultipleFrames(ImageFormat format);
    static bool SupportsAnimation(ImageFormat format);
    
private:
    static void InitializeCodecs();
    static std::vector<ImageCodecInfo> _decoders;
    static std::vector<ImageCodecInfo> _encoders;
    static bool _initialized;
};

}
}
}
