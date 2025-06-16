
#pragma once

#include "Image.h"
#include "Color.h"
#include "Rectangle.h"
#include <memory>

// Forward declarations for Skia
class SkBitmap;
class SkPixmap;

namespace System {
namespace Drawing {

// Forward declarations
class Graphics;
namespace Imaging {
    enum class PixelFormat;
    struct BitmapData;
}

class Bitmap : public Image {
private:
    mutable std::unique_ptr<SkBitmap> _skBitmap;
    mutable bool _bitmapDirty;
    
public:
    // Constructors
    Bitmap(int width, int height);
    Bitmap(int width, int height, Imaging::PixelFormat format);
    Bitmap(int width, int height, int stride, Imaging::PixelFormat format, void* scan0);
    explicit Bitmap(const std::string& filename);
    Bitmap(const std::vector<uint8_t>& data);
    Bitmap(const Image& original);
    Bitmap(const Image& original, const Size& newSize);
    Bitmap(const Image& original, int width, int height);
    
    // Destructor
    virtual ~Bitmap() = default;
    
    // Pixel access
    Color GetPixel(int x, int y) const;
    void SetPixel(int x, int y, const Color& color);
    
    // Graphics access
    std::shared_ptr<Graphics> GetGraphics();
    
    // Bitmap operations
    std::shared_ptr<Bitmap> Clone() const;
    std::shared_ptr<Bitmap> Clone(const Rectangle& rect, Imaging::PixelFormat format) const;
    std::shared_ptr<Bitmap> Clone(const RectangleF& rect, Imaging::PixelFormat format) const;
    
    // Resolution
    void SetResolution(float xDpi, float yDpi);
    
    // Locking for direct pixel access
    Imaging::BitmapData* LockBits(const Rectangle& rect, int flags, Imaging::PixelFormat format);
    void UnlockBits(Imaging::BitmapData* bitmapData);
    
    // Color palette
    void SetPalette(const std::vector<Color>& palette);
    std::vector<Color> GetPalette() const;
    
    // Transparency
    void MakeTransparent();
    void MakeTransparent(const Color& transparentColor);
    
    // Skia integration
    SkBitmap* GetSkBitmap() const;
    void SetSkBitmap(const SkBitmap& skBitmap);
    
private:
    void EnsureSkBitmap() const;
    void SyncFromSkImage() const;
    void SyncToSkImage();
    void CreateEmptyBitmap(int width, int height, Imaging::PixelFormat format);
};

}
}
