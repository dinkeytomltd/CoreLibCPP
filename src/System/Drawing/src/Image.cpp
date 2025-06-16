
#include "System/Drawing/Image.h"
#include "System/Drawing/Graphics.h"
#include "System/Drawing/Imaging/ImageFormat.h"
#include "System/Drawing/Imaging/PixelFormat.h"
#include <fstream>
#include <stdexcept>

// Skia includes
#include "include/core/SkImage.h"
#include "include/core/SkData.h"
#include "include/core/SkStream.h"
#include "include/codec/SkCodec.h"
#include "include/encode/SkPngEncoder.h"
#include "include/encode/SkJpegEncoder.h"

namespace System {
namespace Drawing {

Image::Image() : _skImage(nullptr) {
}

Image::Image(sk_sp<SkImage> skImage) : _skImage(skImage) {
}

std::shared_ptr<Image> Image::FromFile(const std::string& filename) {
    sk_sp<SkData> data = SkData::MakeFromFileName(filename.c_str());
    if (!data) {
        throw std::runtime_error("Failed to load image file: " + filename);
    }
    
    sk_sp<SkImage> skImage = SkImage::MakeFromEncoded(data);
    if (!skImage) {
        throw std::runtime_error("Failed to decode image file: " + filename);
    }
    
    return std::make_shared<Image>(skImage);
}

std::shared_ptr<Image> Image::FromStream(const std::vector<uint8_t>& data) {
    sk_sp<SkData> skData = SkData::MakeWithCopy(data.data(), data.size());
    if (!skData) {
        throw std::runtime_error("Failed to create data from stream");
    }
    
    sk_sp<SkImage> skImage = SkImage::MakeFromEncoded(skData);
    if (!skImage) {
        throw std::runtime_error("Failed to decode image from stream");
    }
    
    return std::make_shared<Image>(skImage);
}

std::shared_ptr<Image> Image::FromHbitmap(void* hbitmap) {
    // Platform-specific implementation
    throw std::runtime_error("Image::FromHbitmap not yet implemented");
}

std::shared_ptr<Image> Image::FromHicon(void* hicon) {
    // Platform-specific implementation
    throw std::runtime_error("Image::FromHicon not yet implemented");
}

int Image::Width() const {
    EnsureSkImage();
    return _skImage ? _skImage->width() : 0;
}

int Image::Height() const {
    EnsureSkImage();
    return _skImage ? _skImage->height() : 0;
}

Size Image::GetSize() const {
    return Size(Width(), Height());
}

RectangleF Image::GetBounds() const {
    return RectangleF(0, 0, static_cast<float>(Width()), static_cast<float>(Height()));
}

Imaging::PixelFormat Image::GetPixelFormat() const {
    EnsureSkImage();
    if (!_skImage) {
        return Imaging::PixelFormat::Undefined;
    }
    
    // Map Skia color types to System.Drawing pixel formats
    switch (_skImage->colorType()) {
        case kAlpha_8_SkColorType:
            return Imaging::PixelFormat::Format8bppIndexed;
        case kRGB_565_SkColorType:
            return Imaging::PixelFormat::Format16bppRgb565;
        case kARGB_4444_SkColorType:
            return Imaging::PixelFormat::Format16bppArgb1555;
        case kRGBA_8888_SkColorType:
            return Imaging::PixelFormat::Format32bppArgb;
        case kBGRA_8888_SkColorType:
            return Imaging::PixelFormat::Format32bppArgb;
        case kRGB_888x_SkColorType:
            return Imaging::PixelFormat::Format24bppRgb;
        case kRGBA_F16_SkColorType:
            return Imaging::PixelFormat::Format64bppArgb;
        default:
            return Imaging::PixelFormat::Format32bppArgb;
    }
}

float Image::HorizontalResolution() const {
    return 96.0f; // Default DPI
}

float Image::VerticalResolution() const {
    return 96.0f; // Default DPI
}

void Image::Save(const std::string& filename) const {
    Imaging::ImageFormat format = Imaging::ImageFormatHelper::FromFileExtension(filename);
    Save(filename, format);
}

void Image::Save(const std::string& filename, Imaging::ImageFormat format) const {
    EnsureSkImage();
    if (!_skImage) {
        throw std::runtime_error("No image data to save");
    }
    
    sk_sp<SkData> encodedData;
    
    switch (format) {
        case Imaging::ImageFormat::Png: {
            SkPngEncoder::Options options;
            encodedData = SkPngEncoder::Encode(nullptr, _skImage.get(), options);
            break;
        }
        case Imaging::ImageFormat::Jpeg: {
            SkJpegEncoder::Options options;
            options.fQuality = 90;
            encodedData = SkJpegEncoder::Encode(nullptr, _skImage.get(), options);
            break;
        }
        default:
            throw std::runtime_error("Unsupported image format for saving");
    }
    
    if (!encodedData) {
        throw std::runtime_error("Failed to encode image");
    }
    
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }
    
    file.write(reinterpret_cast<const char*>(encodedData->data()), encodedData->size());
    if (!file) {
        throw std::runtime_error("Failed to write image data to file: " + filename);
    }
}

void Image::Save(std::vector<uint8_t>& data, Imaging::ImageFormat format) const {
    EnsureSkImage();
    if (!_skImage) {
        throw std::runtime_error("No image data to save");
    }
    
    sk_sp<SkData> encodedData;
    
    switch (format) {
        case Imaging::ImageFormat::Png: {
            SkPngEncoder::Options options;
            encodedData = SkPngEncoder::Encode(nullptr, _skImage.get(), options);
            break;
        }
        case Imaging::ImageFormat::Jpeg: {
            SkJpegEncoder::Options options;
            options.fQuality = 90;
            encodedData = SkJpegEncoder::Encode(nullptr, _skImage.get(), options);
            break;
        }
        default:
            throw std::runtime_error("Unsupported image format for saving");
    }
    
    if (!encodedData) {
        throw std::runtime_error("Failed to encode image");
    }
    
    data.resize(encodedData->size());
    std::memcpy(data.data(), encodedData->data(), encodedData->size());
}

std::shared_ptr<Image> Image::GetThumbnailImage(int thumbWidth, int thumbHeight) const {
    EnsureSkImage();
    if (!_skImage) {
        throw std::runtime_error("No image data for thumbnail");
    }
    
    // Create a scaled version of the image
    sk_sp<SkImage> scaledImage = _skImage->makeSubset(nullptr, 
        SkIRect::MakeWH(thumbWidth, thumbHeight));
    
    if (!scaledImage) {
        throw std::runtime_error("Failed to create thumbnail");
    }
    
    return std::make_shared<Image>(scaledImage);
}

void Image::RotateFlip(int rotateFlipType) {
    // TODO: Implement rotation and flipping
    throw std::runtime_error("RotateFlip not yet implemented");
}

void Image::EnsureSkImage() const {
    // This method ensures that _skImage is valid
    // In derived classes, this might load the image data
}

}
}
