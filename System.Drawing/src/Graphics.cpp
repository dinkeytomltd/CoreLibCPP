
#include "System/Drawing/Graphics.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include "System/Drawing/Font.h"
#include "System/Drawing/Image.h"
#include "System/Drawing/Drawing2D/GraphicsPath.h"
#include "System/Drawing/Drawing2D/Matrix.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkFont.h"
#include "include/core/SkTextBlob.h"
#include "include/effects/SkGradientShader.h"

namespace System {
namespace Drawing {

Graphics::Graphics(SkCanvas* canvas, bool ownsCanvas)
    : _canvas(canvas ? std::unique_ptr<SkCanvas>(canvas) : nullptr)
    , _ownsCanvas(ownsCanvas)
    , _smoothingMode(SmoothingMode::Default)
    , _textRenderingHint(TextRenderingHint::SystemDefault)
    , _compositingMode(CompositingMode::SourceOver)
    , _compositingQuality(CompositingQuality::Default)
    , _interpolationMode(InterpolationMode::Default)
    , _pixelOffsetMode(PixelOffsetMode::Default) {
    
    if (!_canvas) {
        throw std::invalid_argument("Canvas cannot be null");
    }
}

Graphics::Graphics(std::shared_ptr<SkSurface> surface)
    : _surface(surface)
    , _canvas(surface ? std::unique_ptr<SkCanvas>(surface->getCanvas()) : nullptr)
    , _ownsCanvas(false)
    , _smoothingMode(SmoothingMode::Default)
    , _textRenderingHint(TextRenderingHint::SystemDefault)
    , _compositingMode(CompositingMode::SourceOver)
    , _compositingQuality(CompositingQuality::Default)
    , _interpolationMode(InterpolationMode::Default)
    , _pixelOffsetMode(PixelOffsetMode::Default) {
    
    if (!_canvas) {
        throw std::invalid_argument("Surface cannot be null");
    }
}

Graphics::~Graphics() {
    if (_ownsCanvas) {
        _canvas.release();
    }
}

// Static factory methods
std::unique_ptr<Graphics> Graphics::FromImage(const Image& image) {
    // Implementation would create a surface from the image
    throw std::runtime_error("Graphics::FromImage not yet implemented");
}

std::unique_ptr<Graphics> Graphics::FromHwnd(void* hwnd) {
    // Platform-specific implementation
    throw std::runtime_error("Graphics::FromHwnd not yet implemented");
}

std::unique_ptr<Graphics> Graphics::FromHdc(void* hdc) {
    // Platform-specific implementation
    throw std::runtime_error("Graphics::FromHdc not yet implemented");
}

// Drawing methods - Lines
void Graphics::DrawLine(const Pen& pen, int x1, int y1, int x2, int y2) {
    DrawLine(pen, static_cast<float>(x1), static_cast<float>(y1), 
             static_cast<float>(x2), static_cast<float>(y2));
}

void Graphics::DrawLine(const Pen& pen, float x1, float y1, float x2, float y2) {
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    _canvas->drawLine(x1, y1, x2, y2, paint);
}

void Graphics::DrawLine(const Pen& pen, const Point& pt1, const Point& pt2) {
    DrawLine(pen, pt1.X, pt1.Y, pt2.X, pt2.Y);
}

void Graphics::DrawLine(const Pen& pen, const PointF& pt1, const PointF& pt2) {
    DrawLine(pen, pt1.X, pt1.Y, pt2.X, pt2.Y);
}

void Graphics::DrawLines(const Pen& pen, const std::vector<Point>& points) {
    if (points.size() < 2) return;
    
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    
    SkPath path;
    path.moveTo(points[0].X, points[0].Y);
    for (size_t i = 1; i < points.size(); ++i) {
        path.lineTo(points[i].X, points[i].Y);
    }
    _canvas->drawPath(path, paint);
}

void Graphics::DrawLines(const Pen& pen, const std::vector<PointF>& points) {
    if (points.size() < 2) return;
    
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    
    SkPath path;
    path.moveTo(points[0].X, points[0].Y);
    for (size_t i = 1; i < points.size(); ++i) {
        path.lineTo(points[i].X, points[i].Y);
    }
    _canvas->drawPath(path, paint);
}

// Drawing methods - Rectangles
void Graphics::DrawRectangle(const Pen& pen, int x, int y, int width, int height) {
    DrawRectangle(pen, static_cast<float>(x), static_cast<float>(y), 
                  static_cast<float>(width), static_cast<float>(height));
}

void Graphics::DrawRectangle(const Pen& pen, float x, float y, float width, float height) {
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    SkRect rect = SkRect::MakeXYWH(x, y, width, height);
    _canvas->drawRect(rect, paint);
}

void Graphics::DrawRectangle(const Pen& pen, const Rectangle& rect) {
    DrawRectangle(pen, rect.X, rect.Y, rect.Width, rect.Height);
}

void Graphics::DrawRectangle(const Pen& pen, const RectangleF& rect) {
    DrawRectangle(pen, rect.X, rect.Y, rect.Width, rect.Height);
}

void Graphics::FillRectangle(const Brush& brush, int x, int y, int width, int height) {
    FillRectangle(brush, static_cast<float>(x), static_cast<float>(y), 
                  static_cast<float>(width), static_cast<float>(height));
}

void Graphics::FillRectangle(const Brush& brush, float x, float y, float width, float height) {
    SkPaint paint;
    ApplyBrushToSkPaint(brush, paint);
    SkRect rect = SkRect::MakeXYWH(x, y, width, height);
    _canvas->drawRect(rect, paint);
}

void Graphics::FillRectangle(const Brush& brush, const Rectangle& rect) {
    FillRectangle(brush, rect.X, rect.Y, rect.Width, rect.Height);
}

void Graphics::FillRectangle(const Brush& brush, const RectangleF& rect) {
    FillRectangle(brush, rect.X, rect.Y, rect.Width, rect.Height);
}

// Drawing methods - Ellipses
void Graphics::DrawEllipse(const Pen& pen, int x, int y, int width, int height) {
    DrawEllipse(pen, static_cast<float>(x), static_cast<float>(y), 
                static_cast<float>(width), static_cast<float>(height));
}

void Graphics::DrawEllipse(const Pen& pen, float x, float y, float width, float height) {
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    SkRect rect = SkRect::MakeXYWH(x, y, width, height);
    _canvas->drawOval(rect, paint);
}

void Graphics::DrawEllipse(const Pen& pen, const Rectangle& rect) {
    DrawEllipse(pen, rect.X, rect.Y, rect.Width, rect.Height);
}

void Graphics::DrawEllipse(const Pen& pen, const RectangleF& rect) {
    DrawEllipse(pen, rect.X, rect.Y, rect.Width, rect.Height);
}

void Graphics::FillEllipse(const Brush& brush, int x, int y, int width, int height) {
    FillEllipse(brush, static_cast<float>(x), static_cast<float>(y), 
                static_cast<float>(width), static_cast<float>(height));
}

void Graphics::FillEllipse(const Brush& brush, float x, float y, float width, float height) {
    SkPaint paint;
    ApplyBrushToSkPaint(brush, paint);
    SkRect rect = SkRect::MakeXYWH(x, y, width, height);
    _canvas->drawOval(rect, paint);
}

void Graphics::FillEllipse(const Brush& brush, const Rectangle& rect) {
    FillEllipse(brush, rect.X, rect.Y, rect.Width, rect.Height);
}

void Graphics::FillEllipse(const Brush& brush, const RectangleF& rect) {
    FillEllipse(brush, rect.X, rect.Y, rect.Width, rect.Height);
}

// Text rendering
void Graphics::DrawString(const std::string& text, const Font& font, const Brush& brush, float x, float y) {
    SkPaint paint;
    ApplyBrushToSkPaint(brush, paint);
    ApplyFontToSkPaint(font, paint);
    
    SkFont skFont = font.GetSkFont();
    _canvas->drawString(text.c_str(), x, y, skFont, paint);
}

void Graphics::DrawString(const std::string& text, const Font& font, const Brush& brush, const PointF& point) {
    DrawString(text, font, brush, point.X, point.Y);
}

void Graphics::DrawString(const std::string& text, const Font& font, const Brush& brush, const RectangleF& layoutRectangle) {
    // For now, just draw at the top-left of the rectangle
    // TODO: Implement proper text layout within rectangle
    DrawString(text, font, brush, layoutRectangle.X, layoutRectangle.Y);
}

SizeF Graphics::MeasureString(const std::string& text, const Font& font) {
    SkFont skFont = font.GetSkFont();
    SkRect bounds;
    skFont.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
    return SizeF(bounds.width(), bounds.height());
}

SizeF Graphics::MeasureString(const std::string& text, const Font& font, const SizeF& layoutArea) {
    // For now, just return basic measurement
    // TODO: Implement proper text layout within area
    return MeasureString(text, font);
}

SizeF Graphics::MeasureString(const std::string& text, const Font& font, int width) {
    return MeasureString(text, font, SizeF(static_cast<float>(width), 0));
}

// State management
void Graphics::Save() {
    _canvas->save();
}

void Graphics::Restore() {
    _canvas->restore();
}

void Graphics::Clear(const Color& color) {
    _canvas->clear(color.ToSkColor());
}

void Graphics::Flush() {
    if (_surface) {
        _surface->flushAndSubmit();
    }
}

// Properties
void Graphics::SetSmoothingMode(SmoothingMode value) {
    _smoothingMode = value;
    UpdateSkiaSettings();
}

void Graphics::SetTextRenderingHint(TextRenderingHint value) {
    _textRenderingHint = value;
    UpdateSkiaSettings();
}

void Graphics::SetCompositingMode(CompositingMode value) {
    _compositingMode = value;
    UpdateSkiaSettings();
}

void Graphics::SetCompositingQuality(CompositingQuality value) {
    _compositingQuality = value;
    UpdateSkiaSettings();
}

void Graphics::SetInterpolationMode(InterpolationMode value) {
    _interpolationMode = value;
    UpdateSkiaSettings();
}

void Graphics::SetPixelOffsetMode(PixelOffsetMode value) {
    _pixelOffsetMode = value;
    UpdateSkiaSettings();
}

float Graphics::DpiX() const {
    return 96.0f; // Default DPI
}

float Graphics::DpiY() const {
    return 96.0f; // Default DPI
}

// Private helper methods
void Graphics::ApplyPenToSkPaint(const Pen& pen, SkPaint& paint) const {
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(pen.GetWidth());
    pen.ApplyToSkPaint(paint);
}

void Graphics::ApplyBrushToSkPaint(const Brush& brush, SkPaint& paint) const {
    paint.setStyle(SkPaint::kFill_Style);
    brush.ApplyToSkPaint(paint);
}

void Graphics::ApplyFontToSkPaint(const Font& font, SkPaint& paint) const {
    // Font properties are applied through SkFont, not SkPaint
    // This method can be used for text-specific paint properties
}

void Graphics::UpdateSkiaSettings() {
    // Apply current graphics settings to Skia canvas
    // This would update anti-aliasing, filtering, etc.
}

// Placeholder implementations for methods not yet implemented
void Graphics::DrawRectangles(const Pen& pen, const std::vector<Rectangle>& rects) {
    for (const auto& rect : rects) {
        DrawRectangle(pen, rect);
    }
}

void Graphics::DrawRectangles(const Pen& pen, const std::vector<RectangleF>& rects) {
    for (const auto& rect : rects) {
        DrawRectangle(pen, rect);
    }
}

void Graphics::FillRectangles(const Brush& brush, const std::vector<Rectangle>& rects) {
    for (const auto& rect : rects) {
        FillRectangle(brush, rect);
    }
}

void Graphics::FillRectangles(const Brush& brush, const std::vector<RectangleF>& rects) {
    for (const auto& rect : rects) {
        FillRectangle(brush, rect);
    }
}

void Graphics::DrawArc(const Pen& pen, int x, int y, int width, int height, int startAngle, int sweepAngle) {
    // TODO: Implement arc drawing
    throw std::runtime_error("DrawArc not yet implemented");
}

void Graphics::DrawArc(const Pen& pen, float x, float y, float width, float height, float startAngle, float sweepAngle) {
    // TODO: Implement arc drawing
    throw std::runtime_error("DrawArc not yet implemented");
}

void Graphics::DrawArc(const Pen& pen, const Rectangle& rect, float startAngle, float sweepAngle) {
    DrawArc(pen, rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
}

void Graphics::DrawArc(const Pen& pen, const RectangleF& rect, float startAngle, float sweepAngle) {
    DrawArc(pen, rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
}

void Graphics::FillPie(const Brush& brush, int x, int y, int width, int height, int startAngle, int sweepAngle) {
    // TODO: Implement pie filling
    throw std::runtime_error("FillPie not yet implemented");
}

void Graphics::FillPie(const Brush& brush, float x, float y, float width, float height, float startAngle, float sweepAngle) {
    // TODO: Implement pie filling
    throw std::runtime_error("FillPie not yet implemented");
}

void Graphics::FillPie(const Brush& brush, const Rectangle& rect, float startAngle, float sweepAngle) {
    FillPie(brush, rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
}

void Graphics::FillPie(const Brush& brush, const RectangleF& rect, float startAngle, float sweepAngle) {
    FillPie(brush, rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
}

void Graphics::DrawPolygon(const Pen& pen, const std::vector<Point>& points) {
    // TODO: Implement polygon drawing
    throw std::runtime_error("DrawPolygon not yet implemented");
}

void Graphics::DrawPolygon(const Pen& pen, const std::vector<PointF>& points) {
    // TODO: Implement polygon drawing
    throw std::runtime_error("DrawPolygon not yet implemented");
}

void Graphics::FillPolygon(const Brush& brush, const std::vector<Point>& points) {
    // TODO: Implement polygon filling
    throw std::runtime_error("FillPolygon not yet implemented");
}

void Graphics::FillPolygon(const Brush& brush, const std::vector<PointF>& points) {
    // TODO: Implement polygon filling
    throw std::runtime_error("FillPolygon not yet implemented");
}

void Graphics::DrawPath(const Pen& pen, const GraphicsPath& path) {
    // TODO: Implement path drawing
    throw std::runtime_error("DrawPath not yet implemented");
}

void Graphics::FillPath(const Brush& brush, const GraphicsPath& path) {
    // TODO: Implement path filling
    throw std::runtime_error("FillPath not yet implemented");
}

void Graphics::DrawImage(const Image& image, int x, int y) {
    // TODO: Implement image drawing
    throw std::runtime_error("DrawImage not yet implemented");
}

void Graphics::DrawImage(const Image& image, float x, float y) {
    // TODO: Implement image drawing
    throw std::runtime_error("DrawImage not yet implemented");
}

void Graphics::DrawImage(const Image& image, const Point& point) {
    DrawImage(image, point.X, point.Y);
}

void Graphics::DrawImage(const Image& image, const PointF& point) {
    DrawImage(image, point.X, point.Y);
}

void Graphics::DrawImage(const Image& image, const Rectangle& rect) {
    // TODO: Implement image drawing
    throw std::runtime_error("DrawImage not yet implemented");
}

void Graphics::DrawImage(const Image& image, const RectangleF& rect) {
    // TODO: Implement image drawing
    throw std::runtime_error("DrawImage not yet implemented");
}

void Graphics::DrawImage(const Image& image, const Rectangle& destRect, const Rectangle& srcRect) {
    // TODO: Implement image drawing
    throw std::runtime_error("DrawImage not yet implemented");
}

void Graphics::DrawImage(const Image& image, const RectangleF& destRect, const RectangleF& srcRect) {
    // TODO: Implement image drawing
    throw std::runtime_error("DrawImage not yet implemented");
}

void Graphics::SetTransform(const Matrix& matrix) {
    // TODO: Implement transformation
    throw std::runtime_error("SetTransform not yet implemented");
}

Matrix Graphics::GetTransform() const {
    // TODO: Implement transformation
    throw std::runtime_error("GetTransform not yet implemented");
}

void Graphics::ResetTransform() {
    // TODO: Implement transformation
    throw std::runtime_error("ResetTransform not yet implemented");
}

void Graphics::MultiplyTransform(const Matrix& matrix) {
    // TODO: Implement transformation
    throw std::runtime_error("MultiplyTransform not yet implemented");
}

void Graphics::TranslateTransform(float dx, float dy) {
    _canvas->translate(dx, dy);
}

void Graphics::RotateTransform(float angle) {
    _canvas->rotate(angle);
}

void Graphics::ScaleTransform(float sx, float sy) {
    _canvas->scale(sx, sy);
}

void Graphics::SetClip(const Rectangle& rect) {
    SkRect skRect = rect.ToSkIRect().makeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    _canvas->clipRect(skRect);
}

void Graphics::SetClip(const RectangleF& rect) {
    SkRect skRect = rect.ToSkRect();
    _canvas->clipRect(skRect);
}

void Graphics::SetClip(const GraphicsPath& path) {
    // TODO: Implement path clipping
    throw std::runtime_error("SetClip with path not yet implemented");
}

void Graphics::IntersectClip(const Rectangle& rect) {
    SkRect skRect = rect.ToSkIRect().makeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    _canvas->clipRect(skRect, SkClipOp::kIntersect);
}

void Graphics::IntersectClip(const RectangleF& rect) {
    SkRect skRect = rect.ToSkRect();
    _canvas->clipRect(skRect, SkClipOp::kIntersect);
}

void Graphics::IntersectClip(const GraphicsPath& path) {
    // TODO: Implement path clipping
    throw std::runtime_error("IntersectClip with path not yet implemented");
}

void Graphics::ExcludeClip(const Rectangle& rect) {
    SkRect skRect = rect.ToSkIRect().makeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    _canvas->clipRect(skRect, SkClipOp::kDifference);
}

void Graphics::ExcludeClip(const RectangleF& rect) {
    SkRect skRect = rect.ToSkRect();
    _canvas->clipRect(skRect, SkClipOp::kDifference);
}

void Graphics::ResetClip() {
    // TODO: Implement clip reset
    throw std::runtime_error("ResetClip not yet implemented");
}

RectangleF Graphics::GetClipBounds() const {
    SkRect bounds = _canvas->getLocalClipBounds();
    return RectangleF::FromSkRect(bounds);
}

bool Graphics::IsClipEmpty() const {
    return _canvas->isClipEmpty();
}

}
}
