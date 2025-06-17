
#include "SkiaBackend.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include "System/Drawing/Font.h"
#include "System/Drawing/Image.h"
#include "System/Drawing/Drawing2D/Matrix.h"

// Skia includes - will be available when Skia is built
#ifdef SK_GANESH
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPaint.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkColor.h"
#include "include/core/SkRect.h"
#else
// Stub definitions for when Skia is not available
class SkCanvas {};
class SkSurface {};
class SkPaint {};
class SkMatrix {};
typedef uint32_t SkColor;
struct SkRect { static SkRect MakeXYWH(float x, float y, float w, float h) { return {}; } };
#define SkColorSetARGB(a,r,g,b) 0
#endif

namespace System {
namespace Drawing {

SkiaBackend::SkiaBackend() 
    : m_canvas(nullptr)
    , m_surface(nullptr)
    , m_ownsCanvas(false)
    , m_currentTransform(std::make_unique<SkMatrix>())
    , m_smoothingMode(SmoothingMode::Default)
    , m_textRenderingHint(TextRenderingHint::SystemDefault) {
#ifdef SK_GANESH
    if (m_currentTransform) {
        m_currentTransform->reset();
    }
#endif
}

SkiaBackend::~SkiaBackend() {
    if (m_ownsCanvas && m_canvas) {
        m_canvas.reset();
    }
}

bool SkiaBackend::Initialize(int width, int height) {
    // TODO: Create Skia surface with specified dimensions
    return false;
}

bool SkiaBackend::Initialize(void* pixels, int width, int height, size_t rowBytes) {
    // TODO: Create Skia surface from pixel buffer
    return false;
}

bool SkiaBackend::Initialize(SkCanvas* canvas, bool ownsCanvas) {
    m_canvas.reset(canvas);
    m_ownsCanvas = ownsCanvas;
    return canvas != nullptr;
}

bool SkiaBackend::Initialize(std::shared_ptr<SkSurface> surface) {
    m_surface = surface;
    if (surface) {
        m_canvas.reset(surface->getCanvas());
        m_ownsCanvas = false;  // Surface owns the canvas
        return true;
    }
    return false;
}

void SkiaBackend::DrawLine(const Pen& pen, const Point& pt1, const Point& pt2) {
    DrawLine(pen, PointF(static_cast<float>(pt1.X), static_cast<float>(pt1.Y)), 
             PointF(static_cast<float>(pt2.X), static_cast<float>(pt2.Y)));
}

void SkiaBackend::DrawLine(const Pen& pen, const PointF& pt1, const PointF& pt2) {
    if (!m_canvas) return;
    
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    m_canvas->drawLine(pt1.X, pt1.Y, pt2.X, pt2.Y, paint);
}

void SkiaBackend::DrawRectangle(const Pen& pen, const Rectangle& rect) {
    DrawRectangle(pen, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                  static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void SkiaBackend::DrawRectangle(const Pen& pen, const RectangleF& rect) {
    if (!m_canvas) return;
    
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    paint.setStyle(SkPaint::kStroke_Style);
    
    SkRect skRect = SkRect::MakeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    m_canvas->drawRect(skRect, paint);
}

void SkiaBackend::FillRectangle(const Brush& brush, const Rectangle& rect) {
    FillRectangle(brush, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                    static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void SkiaBackend::FillRectangle(const Brush& brush, const RectangleF& rect) {
    if (!m_canvas) return;
    
    SkPaint paint;
    ApplyBrushToSkPaint(brush, paint);
    paint.setStyle(SkPaint::kFill_Style);
    
    SkRect skRect = SkRect::MakeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    m_canvas->drawRect(skRect, paint);
}

void SkiaBackend::DrawEllipse(const Pen& pen, const Rectangle& rect) {
    DrawEllipse(pen, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void SkiaBackend::DrawEllipse(const Pen& pen, const RectangleF& rect) {
    if (!m_canvas) return;
    
    SkPaint paint;
    ApplyPenToSkPaint(pen, paint);
    paint.setStyle(SkPaint::kStroke_Style);
    
    SkRect skRect = SkRect::MakeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    m_canvas->drawOval(skRect, paint);
}

void SkiaBackend::FillEllipse(const Brush& brush, const Rectangle& rect) {
    FillEllipse(brush, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                  static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void SkiaBackend::FillEllipse(const Brush& brush, const RectangleF& rect) {
    if (!m_canvas) return;
    
    SkPaint paint;
    ApplyBrushToSkPaint(brush, paint);
    paint.setStyle(SkPaint::kFill_Style);
    
    SkRect skRect = SkRect::MakeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    m_canvas->drawOval(skRect, paint);
}

void SkiaBackend::DrawString(const std::string& text, const Font& font, const Brush& brush, const PointF& point) {
    // TODO: Implement text drawing with Skia
}

void SkiaBackend::DrawImage(const Image& image, const Point& point) {
    DrawImage(image, PointF(static_cast<float>(point.X), static_cast<float>(point.Y)));
}

void SkiaBackend::DrawImage(const Image& image, const PointF& point) {
    // TODO: Implement image drawing with Skia
}

void SkiaBackend::DrawImage(const Image& image, const Rectangle& destRect, const Rectangle& srcRect) {
    // TODO: Implement image drawing with rectangles
}

void SkiaBackend::DrawImage(const Image& image, const RectangleF& destRect, const RectangleF& srcRect) {
    // TODO: Implement image drawing with rectangles
}

void SkiaBackend::SetClip(const Rectangle& rect) {
    SetClip(RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                       static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void SkiaBackend::SetClip(const RectangleF& rect) {
    if (!m_canvas) return;
    
    SkRect skRect = SkRect::MakeXYWH(rect.X, rect.Y, rect.Width, rect.Height);
    m_canvas->clipRect(skRect);
}

void SkiaBackend::ResetClip() {
    // TODO: Implement clip reset
}

void SkiaBackend::SetTransform(const Matrix& matrix) {
    // TODO: Convert Matrix to SkMatrix and apply
}

void SkiaBackend::TranslateTransform(float dx, float dy) {
    if (!m_canvas) return;
    m_canvas->translate(dx, dy);
}

void SkiaBackend::RotateTransform(float angle) {
    if (!m_canvas) return;
    m_canvas->rotate(angle);
}

void SkiaBackend::ScaleTransform(float sx, float sy) {
    if (!m_canvas) return;
    m_canvas->scale(sx, sy);
}

void SkiaBackend::ResetTransform() {
    if (!m_canvas) return;
    m_canvas->resetMatrix();
}

SmoothingMode SkiaBackend::GetSmoothingMode() const {
    return m_smoothingMode;
}

void SkiaBackend::SetSmoothingMode(SmoothingMode mode) {
    m_smoothingMode = mode;
    UpdateSkiaSettings();
}

TextRenderingHint SkiaBackend::GetTextRenderingHint() const {
    return m_textRenderingHint;
}

void SkiaBackend::SetTextRenderingHint(TextRenderingHint hint) {
    m_textRenderingHint = hint;
    UpdateSkiaSettings();
}

void SkiaBackend::BeginDraw() {
    // No-op for Skia
}

void SkiaBackend::EndDraw() {
    // No-op for Skia
}

void SkiaBackend::Clear(const Color& color) {
    if (!m_canvas) return;
    
    SkColor skColor = SkColorSetARGB(color.A, color.R, color.G, color.B);
    m_canvas->clear(skColor);
}

void SkiaBackend::Flush() {
    if (m_surface) {
        m_surface->flushAndSubmit();
    }
}

void SkiaBackend::Save() {
    if (!m_canvas) return;
    m_canvas->save();
}

void SkiaBackend::Restore() {
    if (!m_canvas) return;
    m_canvas->restore();
}

SizeF SkiaBackend::MeasureString(const std::string& text, const Font& font) {
    // TODO: Implement text measurement
    return SizeF(0, 0);
}

void SkiaBackend::ApplyPenToSkPaint(const Pen& pen, SkPaint& paint) const {
    // TODO: Convert Pen properties to SkPaint
}

void SkiaBackend::ApplyBrushToSkPaint(const Brush& brush, SkPaint& paint) const {
    // TODO: Convert Brush properties to SkPaint
}

void SkiaBackend::ApplyFontToSkPaint(const Font& font, SkPaint& paint) const {
    // TODO: Convert Font properties to SkPaint
}

void SkiaBackend::UpdateSkiaSettings() {
    // TODO: Apply smoothing mode and text rendering hint to Skia
}

}
}
