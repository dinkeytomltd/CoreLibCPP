
#pragma once

#include "Size.h"
#include "Rectangle.h"
#include "Color.h"
#include <memory>
#include <string>
#include <vector>

// Forward declarations for Skia
class SkImage;
class SkData;

namespace System {
namespace Drawing {

// Forward declarations
class Graphics;
namespace Imaging {
    enum class ImageFormat;
    enum class PixelFormat;
}

class Image {
protected:
    sk_sp<SkImage> _skImage;
    mutable std::shared_ptr<Graphics> _graphics;
    
public:
    // Constructors
    Image();
    explicit Image(sk_sp<SkImage> skImage);
    virtual ~Image() = default;
    
    // Non-copyable but movable
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;
    
    // Static factory methods
    static std::shared_ptr<Image> FromFile(const std::string& filename);
    static std::shared_ptr<Image> FromStream(const std::vector<uint8_t>& data);
    static std::shared_ptr<Image> FromHbitmap(void* hbitmap);
    static std::shared_ptr<Image> FromHicon(void* hicon);
    
    // Properties
    int Width() const;
    int Height() const;
    Size GetSize() const;
    RectangleF GetBounds() const;
    Imaging::PixelFormat GetPixelFormat() const;
    float HorizontalResolution() const;
    float VerticalResolution() const;
    
    // Methods
    virtual void Save(const std::string& filename) const;
    virtual void Save(const std::string& filename, Imaging::ImageFormat format) const;
    virtual void Save(std::vector<uint8_t>& data, Imaging::ImageFormat format) const;
    
    std::shared_ptr<Image> GetThumbnailImage(int thumbWidth, int thumbHeight) const;
    void RotateFlip(int rotateFlipType);
    
    // Skia integration
    sk_sp<SkImage> GetSkImage() const { return _skImage; }
    void SetSkImage(sk_sp<SkImage> skImage) { _skImage = skImage; }
    
protected:
    void EnsureSkImage() const;
};

}
}
