
#pragma once

// Main System.Drawing header - includes all public APIs
#include "Color.h"
#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include "Graphics.h"
#include "Image.h"
#include "Bitmap.h"
#include "Brush.h"
#include "Pen.h"
#include "Font.h"
#include "FontFamily.h"

// Drawing2D namespace
#include "Drawing2D/Matrix.h"
#include "Drawing2D/GraphicsPath.h"
#include "Drawing2D/LinearGradientBrush.h"

// Imaging namespace
#include "Imaging/ImageFormat.h"
#include "Imaging/PixelFormat.h"
#include "Imaging/ImageCodecInfo.h"

namespace System {
namespace Drawing {
    // Version information
    constexpr int MajorVersion = 1;
    constexpr int MinorVersion = 0;
    constexpr int BuildVersion = 0;
    
    // Initialize the drawing system
    void Initialize();
    void Shutdown();
}
}
