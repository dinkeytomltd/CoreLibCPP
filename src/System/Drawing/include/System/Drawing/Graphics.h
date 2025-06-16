
#pragma once

#include "Color.h"
#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include "IGraphicsBackend.h"
#include "GraphicsConfiguration.h"
#include <memory>
#include <vector>

// Forward declarations
class SkCanvas;
class SkSurface;
class SkPaint;

namespace System {
namespace Drawing {

// Forward declarations
class Pen;
class Brush;
class Font;
class Image;
class GraphicsPath;
class Matrix;

enum class CompositingMode {
    SourceOver = 0,
    SourceCopy = 1
};

enum class CompositingQuality {
    Invalid = -1,
    Default = 0,
    HighSpeed = 1,
    HighQuality = 2,
    GammaCorrected = 3,
    AssumeLinear = 4
};

enum class InterpolationMode {
    Invalid = -1,
    Default = 0,
    Low = 1,
    High = 2,
    Bilinear = 3,
    Bicubic = 4,
    NearestNeighbor = 5,
    HighQualityBilinear = 6,
    HighQualityBicubic = 7
};

enum class PixelOffsetMode {
    Invalid = -1,
    Default = 0,
    HighSpeed = 1,
    HighQuality = 2,
    None = 3,
    Half = 4
};

class Graphics {
private:
    std::unique_ptr<IGraphicsBackend> m_backend;
    GraphicsBackendType m_backendType;
    
    // Legacy Skia support for backward compatibility
    std::unique_ptr<SkCanvas> _canvas;
    std::shared_ptr<SkSurface> _surface;
    bool _ownsCanvas;
    
    // Graphics state
    CompositingMode _compositingMode;
    CompositingQuality _compositingQuality;
    InterpolationMode _interpolationMode;
    PixelOffsetMode _pixelOffsetMode;
    
public:
    // Constructors with backend selection
    Graphics(GraphicsBackendType backendType = GraphicsBackendType::Auto);
    Graphics(std::unique_ptr<IGraphicsBackend> backend);
    
    // Legacy constructors for backward compatibility
    explicit Graphics(SkCanvas* canvas, bool ownsCanvas = false);
    explicit Graphics(std::shared_ptr<SkSurface> surface);
    ~Graphics();
    
    // Non-copyable but movable
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics(Graphics&&) = default;
    Graphics& operator=(Graphics&&) = default;
    
    // Static factory methods with backend selection
    static std::shared_ptr<Graphics> FromImage(std::shared_ptr<Image> image, 
                                             GraphicsBackendType backendType = GraphicsBackendType::Auto);
    static std::shared_ptr<Graphics> FromHwnd(void* hwnd, 
                                            GraphicsBackendType backendType = GraphicsBackendType::Auto);
    static std::shared_ptr<Graphics> FromHdc(void* hdc, 
                                           GraphicsBackendType backendType = GraphicsBackendType::Auto);
    
    // Legacy factory methods for backward compatibility
    static std::unique_ptr<Graphics> FromImage(const Image& image);
    static std::unique_ptr<Graphics> FromHwnd(void* hwnd);
    static std::unique_ptr<Graphics> FromHdc(void* hdc);
    
    // Backend information
    GraphicsBackendType GetBackendType() const { return m_backendType; }
    IGraphicsBackend* GetBackend() const { return m_backend.get(); }
    
    // Drawing methods - Lines (delegate to backend)
    void DrawLine(const Pen& pen, int x1, int y1, int x2, int y2) {
        if (m_backend) m_backend->DrawLine(pen, Point(x1, y1), Point(x2, y2));
    }
    void DrawLine(const Pen& pen, float x1, float y1, float x2, float y2) {
        if (m_backend) m_backend->DrawLine(pen, PointF(x1, y1), PointF(x2, y2));
    }
    void DrawLine(const Pen& pen, const Point& pt1, const Point& pt2) {
        if (m_backend) m_backend->DrawLine(pen, pt1, pt2);
    }
    void DrawLine(const Pen& pen, const PointF& pt1, const PointF& pt2) {
        if (m_backend) m_backend->DrawLine(pen, pt1, pt2);
    }
    void DrawLines(const Pen& pen, const std::vector<Point>& points);
    void DrawLines(const Pen& pen, const std::vector<PointF>& points);
    
    // Drawing methods - Rectangles (delegate to backend)
    void DrawRectangle(const Pen& pen, int x, int y, int width, int height) {
        if (m_backend) m_backend->DrawRectangle(pen, Rectangle(x, y, width, height));
    }
    void DrawRectangle(const Pen& pen, float x, float y, float width, float height) {
        if (m_backend) m_backend->DrawRectangle(pen, RectangleF(x, y, width, height));
    }
    void DrawRectangle(const Pen& pen, const Rectangle& rect) {
        if (m_backend) m_backend->DrawRectangle(pen, rect);
    }
    void DrawRectangle(const Pen& pen, const RectangleF& rect) {
        if (m_backend) m_backend->DrawRectangle(pen, rect);
    }
    void DrawRectangles(const Pen& pen, const std::vector<Rectangle>& rects);
    void DrawRectangles(const Pen& pen, const std::vector<RectangleF>& rects);
    
    void FillRectangle(const Brush& brush, int x, int y, int width, int height) {
        if (m_backend) m_backend->FillRectangle(brush, Rectangle(x, y, width, height));
    }
    void FillRectangle(const Brush& brush, float x, float y, float width, float height) {
        if (m_backend) m_backend->FillRectangle(brush, RectangleF(x, y, width, height));
    }
    void FillRectangle(const Brush& brush, const Rectangle& rect) {
        if (m_backend) m_backend->FillRectangle(brush, rect);
    }
    void FillRectangle(const Brush& brush, const RectangleF& rect) {
        if (m_backend) m_backend->FillRectangle(brush, rect);
    }
    void FillRectangles(const Brush& brush, const std::vector<Rectangle>& rects);
    void FillRectangles(const Brush& brush, const std::vector<RectangleF>& rects);
    
    // Drawing methods - Ellipses (delegate to backend)
    void DrawEllipse(const Pen& pen, int x, int y, int width, int height) {
        if (m_backend) m_backend->DrawEllipse(pen, Rectangle(x, y, width, height));
    }
    void DrawEllipse(const Pen& pen, float x, float y, float width, float height) {
        if (m_backend) m_backend->DrawEllipse(pen, RectangleF(x, y, width, height));
    }
    void DrawEllipse(const Pen& pen, const Rectangle& rect) {
        if (m_backend) m_backend->DrawEllipse(pen, rect);
    }
    void DrawEllipse(const Pen& pen, const RectangleF& rect) {
        if (m_backend) m_backend->DrawEllipse(pen, rect);
    }
    
    void FillEllipse(const Brush& brush, int x, int y, int width, int height) {
        if (m_backend) m_backend->FillEllipse(brush, Rectangle(x, y, width, height));
    }
    void FillEllipse(const Brush& brush, float x, float y, float width, float height) {
        if (m_backend) m_backend->FillEllipse(brush, RectangleF(x, y, width, height));
    }
    void FillEllipse(const Brush& brush, const Rectangle& rect) {
        if (m_backend) m_backend->FillEllipse(brush, rect);
    }
    void FillEllipse(const Brush& brush, const RectangleF& rect) {
        if (m_backend) m_backend->FillEllipse(brush, rect);
    }
    
    // Drawing methods - Arcs
    void DrawArc(const Pen& pen, int x, int y, int width, int height, int startAngle, int sweepAngle);
    void DrawArc(const Pen& pen, float x, float y, float width, float height, float startAngle, float sweepAngle);
    void DrawArc(const Pen& pen, const Rectangle& rect, float startAngle, float sweepAngle);
    void DrawArc(const Pen& pen, const RectangleF& rect, float startAngle, float sweepAngle);
    
    void FillPie(const Brush& brush, int x, int y, int width, int height, int startAngle, int sweepAngle);
    void FillPie(const Brush& brush, float x, float y, float width, float height, float startAngle, float sweepAngle);
    void FillPie(const Brush& brush, const Rectangle& rect, float startAngle, float sweepAngle);
    void FillPie(const Brush& brush, const RectangleF& rect, float startAngle, float sweepAngle);
    
    // Drawing methods - Polygons
    void DrawPolygon(const Pen& pen, const std::vector<Point>& points);
    void DrawPolygon(const Pen& pen, const std::vector<PointF>& points);
    void FillPolygon(const Brush& brush, const std::vector<Point>& points);
    void FillPolygon(const Brush& brush, const std::vector<PointF>& points);
    
    // Drawing methods - Paths
    void DrawPath(const Pen& pen, const GraphicsPath& path);
    void FillPath(const Brush& brush, const GraphicsPath& path);
    
    // Drawing methods - Text
    void DrawString(const std::string& text, const Font& font, const Brush& brush, float x, float y);
    void DrawString(const std::string& text, const Font& font, const Brush& brush, const PointF& point);
    void DrawString(const std::string& text, const Font& font, const Brush& brush, const RectangleF& layoutRectangle);
    
    // Text measurement
    SizeF MeasureString(const std::string& text, const Font& font);
    SizeF MeasureString(const std::string& text, const Font& font, const SizeF& layoutArea);
    SizeF MeasureString(const std::string& text, const Font& font, int width);
    
    // Drawing methods - Images
    void DrawImage(const Image& image, int x, int y);
    void DrawImage(const Image& image, float x, float y);
    void DrawImage(const Image& image, const Point& point);
    void DrawImage(const Image& image, const PointF& point);
    void DrawImage(const Image& image, const Rectangle& rect);
    void DrawImage(const Image& image, const RectangleF& rect);
    void DrawImage(const Image& image, const Rectangle& destRect, const Rectangle& srcRect);
    void DrawImage(const Image& image, const RectangleF& destRect, const RectangleF& srcRect);
    
    // Transformation methods
    void SetTransform(const Matrix& matrix);
    Matrix GetTransform() const;
    void ResetTransform();
    void MultiplyTransform(const Matrix& matrix);
    void TranslateTransform(float dx, float dy);
    void RotateTransform(float angle);
    void ScaleTransform(float sx, float sy);
    
    // Clipping methods
    void SetClip(const Rectangle& rect);
    void SetClip(const RectangleF& rect);
    void SetClip(const GraphicsPath& path);
    void IntersectClip(const Rectangle& rect);
    void IntersectClip(const RectangleF& rect);
    void IntersectClip(const GraphicsPath& path);
    void ExcludeClip(const Rectangle& rect);
    void ExcludeClip(const RectangleF& rect);
    void ResetClip();
    RectangleF GetClipBounds() const;
    bool IsClipEmpty() const;
    
    // State management (delegate to backend)
    void Save() {
        if (m_backend) m_backend->Save();
    }
    void Restore() {
        if (m_backend) m_backend->Restore();
    }
    void Clear(const Color& color) {
        if (m_backend) m_backend->Clear(color);
    }
    void Flush() {
        if (m_backend) m_backend->Flush();
    }
    
    // Properties (delegate to backend)
    SmoothingMode GetSmoothingMode() const { 
        return m_backend ? m_backend->GetSmoothingMode() : SmoothingMode::Default; 
    }
    void SetSmoothingMode(SmoothingMode value) {
        if (m_backend) m_backend->SetSmoothingMode(value);
    }
    
    TextRenderingHint GetTextRenderingHint() const { 
        return m_backend ? m_backend->GetTextRenderingHint() : TextRenderingHint::SystemDefault; 
    }
    void SetTextRenderingHint(TextRenderingHint value) {
        if (m_backend) m_backend->SetTextRenderingHint(value);
    }
    
    CompositingMode GetCompositingMode() const { return _compositingMode; }
    void SetCompositingMode(CompositingMode value);
    
    CompositingQuality GetCompositingQuality() const { return _compositingQuality; }
    void SetCompositingQuality(CompositingQuality value);
    
    InterpolationMode GetInterpolationMode() const { return _interpolationMode; }
    void SetInterpolationMode(InterpolationMode value);
    
    PixelOffsetMode GetPixelOffsetMode() const { return _pixelOffsetMode; }
    void SetPixelOffsetMode(PixelOffsetMode value);
    
    // DPI properties
    float DpiX() const;
    float DpiY() const;
    
    // Skia integration
    SkCanvas* GetSkCanvas() const { return _canvas.get(); }
    
private:
    void ApplyPenToSkPaint(const Pen& pen, SkPaint& paint) const;
    void ApplyBrushToSkPaint(const Brush& brush, SkPaint& paint) const;
    void ApplyFontToSkPaint(const Font& font, SkPaint& paint) const;
    void UpdateSkiaSettings();
};

}
}
