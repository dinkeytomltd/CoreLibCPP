
#include "System/Drawing/Image.h"
#include "System/Drawing/Bitmap.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkImage.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkPixmap.h"

namespace System {
namespace Drawing {

// Helper functions for Skia image operations
sk_sp<SkImage> CreateSkImageFromBitmap(const SkBitmap& bitmap) {
    return SkImage::MakeFromBitmap(bitmap);
}

sk_sp<SkImage> CreateSkImageFromPixels(int width, int height, const void* pixels, size_t rowBytes) {
    SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
    return SkImage::MakeRasterCopy(SkPixmap(info, pixels, rowBytes));
}

bool ExtractBitmapFromSkImage(const sk_sp<SkImage>& image, SkBitmap& bitmap) {
    if (!image) {
        return false;
    }
    
    return image->asLegacyBitmap(&bitmap);
}

}
}
