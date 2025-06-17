
#pragma once

#include "System/Drawing/IGraphicsBackend.h"
#include <memory>

// Forward declarations for Skia
class SkCanvas;
class SkSurface;
class SkPaint;
class SkMatrix;

namespace System {
namespace Drawing {

class SkiaBackend : public IGraphicsBackend {
private:
    std::unique_ptr<SkCanvas> m_canvas;
    std::shared_ptr<SkSurface> m_surface;
    bool m_ownsCanvas;
    std::unique_ptr<SkMatrix> m_currentTransform;  // Use pointer to avoid incomplete type
    SmoothingMode m_smoothingMode;
    TextRenderingHint m_textRenderingHint;
    
public:
    SkiaBackend();
    ~SkiaBackend() override;
    
    // Initialize with various targets
    bool Initialize(int width, int height);
    bool Initialize(void* pixels, int width, int height, size_t rowBytes);
    bool Initialize(SkCanvas* canvas, bool ownsCanvas = false);
    bool Initialize(std::shared_ptr<SkSurface> surface);
    
    // IGraphicsBackend implementation
    void DrawLine(const Pen& pen, const Point& pt1, const Point& pt2) override;
    void DrawLine(const Pen& pen, const PointF& pt1, const PointF& pt2) override;
    void DrawRectangle(const Pen& pen, const Rectangle& rect) override;
    void DrawRectangle(const Pen& pen, const RectangleF& rect) override;
    void FillRectangle(const Brush& brush, const Rectangle& rect) override;
    void FillRectangle(const Brush& brush, const RectangleF& rect) override;
    void DrawEllipse(const Pen& pen, const Rectangle& rect) override;
    void DrawEllipse(const Pen& pen, const RectangleF& rect) override;
    void FillEllipse(const Brush& brush, const Rectangle& rect) override;
    void FillEllipse(const Brush& brush, const RectangleF& rect) override;
    void DrawString(const std::string& text, const Font& font, const Brush& brush, const PointF& point) override;
    void DrawImage(const Image& image, const Point& point) override;
    void DrawImage(const Image& image, const PointF& point) override;
    void DrawImage(const Image& image, const Rectangle& destRect, const Rectangle& srcRect) override;
    void DrawImage(const Image& image, const RectangleF& destRect, const RectangleF& srcRect) override;
    
    void SetClip(const Rectangle& rect) override;
    void SetClip(const RectangleF& rect) override;
    void ResetClip() override;
    void SetTransform(const Matrix& matrix) override;
    void TranslateTransform(float dx, float dy) override;
    void RotateTransform(float angle) override;
    void ScaleTransform(float sx, float sy) override;
    void ResetTransform() override;
    
    SmoothingMode GetSmoothingMode() const override;
    void SetSmoothingMode(SmoothingMode mode) override;
    TextRenderingHint GetTextRenderingHint() const override;
    void SetTextRenderingHint(TextRenderingHint hint) override;
    
    void BeginDraw() override;
    void EndDraw() override;
    void Clear(const Color& color) override;
    void Flush() override;
    void Save() override;
    void Restore() override;
    
    SizeF MeasureString(const std::string& text, const Font& font) override;
    
    // Skia-specific access
    SkCanvas* GetSkCanvas() const { return m_canvas.get(); }
    
private:
    // Helper methods
    void ApplyPenToSkPaint(const Pen& pen, SkPaint& paint) const;
    void ApplyBrushToSkPaint(const Brush& brush, SkPaint& paint) const;
    void ApplyFontToSkPaint(const Font& font, SkPaint& paint) const;
    void UpdateSkiaSettings();
};

}
}
