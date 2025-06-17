
#include "System/Drawing/Bitmap.h"
#include "System/Drawing/Graphics.h"
#include "System/Drawing/Imaging/PixelFormat.h"
#include "System/Drawing/Imaging/ImageFormat.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkBitmap.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkSurface.h"
#include "include/core/SkCanvas.h"

namespace System {
namespace Drawing {

Bitmap::Bitmap(int width, int height) 
    : Image(), _bitmapDirty(false) {
    CreateEmptyBitmap(width, height, Imaging::PixelFormat::Format32bppArgb);
}

Bitmap::Bitmap(int width, int height, Imaging::PixelFormat format)
    : Image(), _bitmapDirty(false) {
    CreateEmptyBitmap(width, height, format);
}

Bitmap::Bitmap(int width, int height, int stride, Imaging::PixelFormat format, void* scan0)
    : Image(), _bitmapDirty(false) {
    // TODO: Implement bitmap creation from external data
    throw std::runtime_error("Bitmap constructor with external data not yet implemented");
}

Bitmap::Bitmap(const std::string& filename)
    : Image(), _bitmapDirty(false) {
    auto image = Image::FromFile(filename);
    _skImage = image->GetSkImage();
}

Bitmap::Bitmap(const std::vector<uint8_t>& data)
    : Image(), _bitmapDirty(false) {
    auto image = Image::FromStream(data);
    _skImage = image->GetSkImage();
}

Bitmap::Bitmap(const Image& original)
    : Image(), _bitmapDirty(false) {
    _skImage = original.GetSkImage();
}

Bitmap::Bitmap(const Image& original, const Size& newSize)
    : Image(), _bitmapDirty(false) {
    // TODO: Implement bitmap scaling
    throw std::runtime_error("Bitmap scaling constructor not yet implemented");
}

Bitmap::Bitmap(const Image& original, int width, int height)
    : Bitmap(original, Size(width, height)) {
}

Color Bitmap::GetPixel(int x, int y) const {
    EnsureSkBitmap();
    if (!_skBitmap) {
        throw std::runtime_error("No bitmap data");
    }
    
    if (x < 0 || x >= _skBitmap->width() || y < 0 || y >= _skBitmap->height()) {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    
    SkColor skColor = _skBitmap->getColor(x, y);
    return Color::FromSkColor(skColor);
}

void Bitmap::SetPixel(int x, int y, const Color& color) {
    EnsureSkBitmap();
    if (!_skBitmap) {
        throw std::runtime_error("No bitmap data");
    }
    
    if (x < 0 || x >= _skBitmap->width() || y < 0 || y >= _skBitmap->height()) {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    
    // Make bitmap mutable if it isn't already
    if (_skBitmap->isImmutable()) {
        SkBitmap mutableBitmap;
        if (!_skBitmap->copyTo(&mutableBitmap)) {
            throw std::runtime_error("Failed to make bitmap mutable");
        }
        *_skBitmap = mutableBitmap;
    }
    
    *_skBitmap->getAddr32(x, y) = color.ToSkColor();
    _bitmapDirty = true;
}

std::shared_ptr<Graphics> Bitmap::GetGraphics() {
    EnsureSkBitmap();
    if (!_skBitmap) {
        throw std::runtime_error("No bitmap data");
    }
    
    // Create a surface from the bitmap
    sk_sp<SkSurface> surface = SkSurface::MakeRasterDirect(
        _skBitmap->info(), _skBitmap->getPixels(), _skBitmap->rowBytes());
    
    if (!surface) {
        throw std::runtime_error("Failed to create surface from bitmap");
    }
    
    return std::make_unique<Graphics>(surface);
}

std::shared_ptr<Bitmap> Bitmap::Clone() const {
    EnsureSkBitmap();
    if (!_skBitmap) {
        throw std::runtime_error("No bitmap data to clone");
    }
    
    auto clonedBitmap = std::make_shared<Bitmap>(Width(), Height());
    if (!_skBitmap->copyTo(clonedBitmap->GetSkBitmap())) {
        throw std::runtime_error("Failed to clone bitmap");
    }
    
    return clonedBitmap;
}

std::shared_ptr<Bitmap> Bitmap::Clone(const Rectangle& rect, Imaging::PixelFormat format) const {
    EnsureSkBitmap();
    if (!_skBitmap) {
        throw std::runtime_error("No bitmap data to clone");
    }
    
    SkIRect skRect = SkIRect::MakeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    SkBitmap subset;
    if (!_skBitmap->extractSubset(&subset, skRect)) {
        throw std::runtime_error("Failed to extract bitmap subset");
    }
    
    auto clonedBitmap = std::make_shared<Bitmap>(rect.Width, rect.Height, format);
    clonedBitmap->SetSkBitmap(subset);
    
    return clonedBitmap;
}

std::shared_ptr<Bitmap> Bitmap::Clone(const RectangleF& rect, Imaging::PixelFormat format) const {
    Rectangle intRect = Rectangle::Round(rect);
    return Clone(intRect, format);
}

void Bitmap::SetResolution(float xDpi, float yDpi) {
    // TODO: Implement DPI setting
    // Skia doesn't directly support DPI metadata, would need to store separately
}

Imaging::BitmapData* Bitmap::LockBits(const Rectangle& rect, int flags, Imaging::PixelFormat format) {
    // TODO: Implement bitmap locking for direct pixel access
    throw std::runtime_error("LockBits not yet implemented");
}

void Bitmap::UnlockBits(Imaging::BitmapData* bitmapData) {
    // TODO: Implement bitmap unlocking
    throw std::runtime_error("UnlockBits not yet implemented");
}

void Bitmap::SetPalette(const std::vector<Color>& palette) {
    // TODO: Implement palette setting for indexed color formats
    throw std::runtime_error("SetPalette not yet implemented");
}

std::vector<Color> Bitmap::GetPalette() const {
    // TODO: Implement palette retrieval for indexed color formats
    throw std::runtime_error("GetPalette not yet implemented");
}

void Bitmap::MakeTransparent() {
    MakeTransparent(Color::White);
}

void Bitmap::MakeTransparent(const Color& transparentColor) {
    EnsureSkBitmap();
    if (!_skBitmap) {
        throw std::runtime_error("No bitmap data");
    }
    
    // Make bitmap mutable if it isn't already
    if (_skBitmap->isImmutable()) {
        SkBitmap mutableBitmap;
        if (!_skBitmap->copyTo(&mutableBitmap)) {
            throw std::runtime_error("Failed to make bitmap mutable");
        }
        *_skBitmap = mutableBitmap;
    }
    
    SkColor transparentSkColor = transparentColor.ToSkColor();
    
    // Iterate through all pixels and make matching colors transparent
    for (int y = 0; y < _skBitmap->height(); ++y) {
        for (int x = 0; x < _skBitmap->width(); ++x) {
            SkColor currentColor = _skBitmap->getColor(x, y);
            if ((currentColor & 0x00FFFFFF) == (transparentSkColor & 0x00FFFFFF)) {
                *_skBitmap->getAddr32(x, y) = SkColorSetA(currentColor, 0);
            }
        }
    }
    
    _bitmapDirty = true;
}

SkBitmap* Bitmap::GetSkBitmap() const {
    EnsureSkBitmap();
    return _skBitmap.get();
}

void Bitmap::SetSkBitmap(const SkBitmap& skBitmap) {
    if (!_skBitmap) {
        _skBitmap = std::make_unique<SkBitmap>();
    }
    *_skBitmap = skBitmap;
    _bitmapDirty = true;
    SyncToSkImage();
}

void Bitmap::EnsureSkBitmap() const {
    if (!_skBitmap && _skImage) {
        SyncFromSkImage();
    }
}

void Bitmap::SyncFromSkImage() const {
    if (!_skImage) return;
    
    if (!_skBitmap) {
        _skBitmap = std::make_unique<SkBitmap>();
    }
    
    if (!_skImage->asLegacyBitmap(_skBitmap.get())) {
        throw std::runtime_error("Failed to convert SkImage to SkBitmap");
    }
}

void Bitmap::SyncToSkImage() {
    if (!_skBitmap || !_bitmapDirty) return;
    
    _skImage = SkImage::MakeFromBitmap(*_skBitmap);
    _bitmapDirty = false;
}

void Bitmap::CreateEmptyBitmap(int width, int height, Imaging::PixelFormat format) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Bitmap dimensions must be positive");
    }
    
    SkColorType colorType;
    SkAlphaType alphaType = kPremul_SkAlphaType;
    
    switch (format) {
        case Imaging::PixelFormat::Format24bppRgb:
            colorType = kRGB_888x_SkColorType;
            alphaType = kOpaque_SkAlphaType;
            break;
        case Imaging::PixelFormat::Format32bppRgb:
            colorType = kRGBA_8888_SkColorType;
            alphaType = kOpaque_SkAlphaType;
            break;
        case Imaging::PixelFormat::Format32bppArgb:
            colorType = kRGBA_8888_SkColorType;
            alphaType = kPremul_SkAlphaType;
            break;
        case Imaging::PixelFormat::Format16bppRgb565:
            colorType = kRGB_565_SkColorType;
            alphaType = kOpaque_SkAlphaType;
            break;
        default:
            colorType = kRGBA_8888_SkColorType;
            alphaType = kPremul_SkAlphaType;
            break;
    }
    
    SkImageInfo info = SkImageInfo::Make(width, height, colorType, alphaType);
    
    if (!_skBitmap) {
        _skBitmap = std::make_unique<SkBitmap>();
    }
    
    if (!_skBitmap->allocPixels(info)) {
        throw std::runtime_error("Failed to allocate bitmap pixels");
    }
    
    _skBitmap->eraseColor(SK_ColorTRANSPARENT);
    SyncToSkImage();
}

}
}
