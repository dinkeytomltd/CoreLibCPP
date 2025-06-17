
#pragma once

#ifdef _WIN32

#include "System/Drawing/IGraphicsBackend.h"
#include <windows.h>
#include <gdiplus.h>
#include <memory>
#include <mutex>

#pragma comment(lib, "gdiplus.lib")

namespace System {
namespace Drawing {

class GdiPlusBackend : public IGraphicsBackend {
private:
    Gdiplus::Graphics* m_graphics;
    HDC m_hdc;
    HWND m_hwnd;
    std::unique_ptr<Gdiplus::Bitmap> m_bitmap;
    bool m_ownsGraphics;
    
    // GDI+ startup/shutdown management
    static ULONG_PTR s_gdiplusToken;
    static int s_refCount;
    static std::mutex s_initMutex;
    
public:
    GdiPlusBackend();
    ~GdiPlusBackend() override;
    
    // Initialization methods
    bool Initialize(HDC hdc);
    bool Initialize(HWND hwnd);
    bool Initialize(int width, int height);  // Memory bitmap
    bool Initialize(Gdiplus::Graphics* graphics);  // External graphics
    
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
    
    // Graphics state management
    void SetClip(const Rectangle& rect) override;
    void SetClip(const RectangleF& rect) override;
    void ResetClip() override;
    void SetTransform(const Matrix& matrix) override;
    void TranslateTransform(float dx, float dy) override;
    void RotateTransform(float angle) override;
    void ScaleTransform(float sx, float sy) override;
    void ResetTransform() override;
    
    // Properties
    SmoothingMode GetSmoothingMode() const override;
    void SetSmoothingMode(SmoothingMode mode) override;
    TextRenderingHint GetTextRenderingHint() const override;
    void SetTextRenderingHint(TextRenderingHint hint) override;
    
    // Backend operations
    void BeginDraw() override;
    void EndDraw() override;
    void Clear(const Color& color) override;
    void Flush() override;
    void Save() override;
    void Restore() override;
    
    // Text measurement
    SizeF MeasureString(const std::string& text, const Font& font) override;
    
    // GDI+-specific methods
    Gdiplus::Graphics* GetGdiplusGraphics() const { return m_graphics; }
    HDC GetHDC() const { return m_hdc; }
    
private:
    // Helper methods
    Gdiplus::Color ColorToGdiPlus(const Color& color);
    std::unique_ptr<Gdiplus::Pen> CreateGdiplusPen(const Pen& pen);
    std::unique_ptr<Gdiplus::Brush> CreateGdiplusBrush(const Brush& brush);
    std::unique_ptr<Gdiplus::Font> CreateGdiplusFont(const Font& font);
    Gdiplus::Matrix CreateGdiplusMatrix(const Matrix& matrix);
    
    // GDI+ startup/shutdown
    static bool InitializeGdiPlus();
    static void ShutdownGdiPlus();
};

}
}

#endif // _WIN32
