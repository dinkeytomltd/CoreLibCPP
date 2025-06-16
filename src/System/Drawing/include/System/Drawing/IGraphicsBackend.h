
#pragma once

#include "Color.h"
#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include <memory>
#include <string>

namespace System {
namespace Drawing {

// Forward declarations
class Pen;
class Brush;
class Font;
class Image;
class Matrix;

// Enumerations (moved from Graphics.h for backend interface)
enum class SmoothingMode {
    Invalid = -1,
    Default = 0,
    HighSpeed = 1,
    HighQuality = 2,
    None = 3,
    AntiAlias = 4
};

enum class TextRenderingHint {
    SystemDefault = 0,
    SingleBitPerPixelGridFit = 1,
    SingleBitPerPixel = 2,
    AntiAliasGridFit = 3,
    AntiAlias = 4,
    ClearTypeGridFit = 5
};

// Core backend interface
class IGraphicsBackend {
public:
    virtual ~IGraphicsBackend() = default;
    
    // Drawing operations
    virtual void DrawLine(const Pen& pen, const Point& pt1, const Point& pt2) = 0;
    virtual void DrawLine(const Pen& pen, const PointF& pt1, const PointF& pt2) = 0;
    virtual void DrawRectangle(const Pen& pen, const Rectangle& rect) = 0;
    virtual void DrawRectangle(const Pen& pen, const RectangleF& rect) = 0;
    virtual void FillRectangle(const Brush& brush, const Rectangle& rect) = 0;
    virtual void FillRectangle(const Brush& brush, const RectangleF& rect) = 0;
    virtual void DrawEllipse(const Pen& pen, const Rectangle& rect) = 0;
    virtual void DrawEllipse(const Pen& pen, const RectangleF& rect) = 0;
    virtual void FillEllipse(const Brush& brush, const Rectangle& rect) = 0;
    virtual void FillEllipse(const Brush& brush, const RectangleF& rect) = 0;
    virtual void DrawString(const std::string& text, const Font& font, const Brush& brush, const PointF& point) = 0;
    virtual void DrawImage(const Image& image, const Point& point) = 0;
    virtual void DrawImage(const Image& image, const PointF& point) = 0;
    virtual void DrawImage(const Image& image, const Rectangle& destRect, const Rectangle& srcRect) = 0;
    virtual void DrawImage(const Image& image, const RectangleF& destRect, const RectangleF& srcRect) = 0;
    
    // Graphics state
    virtual void SetClip(const Rectangle& rect) = 0;
    virtual void SetClip(const RectangleF& rect) = 0;
    virtual void ResetClip() = 0;
    virtual void SetTransform(const Matrix& matrix) = 0;
    virtual void TranslateTransform(float dx, float dy) = 0;
    virtual void RotateTransform(float angle) = 0;
    virtual void ScaleTransform(float sx, float sy) = 0;
    virtual void ResetTransform() = 0;
    
    // Properties
    virtual SmoothingMode GetSmoothingMode() const = 0;
    virtual void SetSmoothingMode(SmoothingMode mode) = 0;
    virtual TextRenderingHint GetTextRenderingHint() const = 0;
    virtual void SetTextRenderingHint(TextRenderingHint hint) = 0;
    
    // Backend-specific operations
    virtual void BeginDraw() = 0;
    virtual void EndDraw() = 0;
    virtual void Clear(const Color& color) = 0;
    virtual void Flush() = 0;
    virtual void Save() = 0;
    virtual void Restore() = 0;
    
    // Text measurement
    virtual SizeF MeasureString(const std::string& text, const Font& font) = 0;
};

}
}
