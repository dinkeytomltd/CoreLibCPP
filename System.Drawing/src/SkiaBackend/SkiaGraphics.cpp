
#include "System/Drawing/Graphics.h"
#include "System/Drawing/Color.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkImageInfo.h"

namespace System {
namespace Drawing {

// Factory methods for creating Graphics from Skia objects
std::unique_ptr<Graphics> CreateGraphicsFromSkSurface(int width, int height) {
    SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
    sk_sp<SkSurface> surface = SkSurface::MakeRaster(info);
    
    if (!surface) {
        throw std::runtime_error("Failed to create Skia surface");
    }
    
    return std::make_unique<Graphics>(surface);
}

std::unique_ptr<Graphics> CreateGraphicsFromSkCanvas(SkCanvas* canvas) {
    if (!canvas) {
        throw std::invalid_argument("Canvas cannot be null");
    }
    
    return std::make_unique<Graphics>(canvas, false);
}

}
}
