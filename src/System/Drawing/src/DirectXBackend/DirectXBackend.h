
#pragma once

#ifdef _WIN32

#include "System/Drawing/IGraphicsBackend.h"
#include <memory>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

namespace System {
namespace Drawing {

class DirectXBackend : public IGraphicsBackend {
private:
    Microsoft::WRL::ComPtr<ID2D1Factory> m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_renderTarget;
    Microsoft::WRL::ComPtr<IDWriteFactory> m_writeFactory;
    Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicFactory;
    
    // Current graphics state
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_currentBrush;
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_currentStrokeStyle;
    D2D1::Matrix3x2F m_currentTransform;
    SmoothingMode m_smoothingMode;
    TextRenderingHint m_textRenderingHint;
    
public:
    DirectXBackend();
    ~DirectXBackend() override;
    
    // Initialize with render target
    bool Initialize(ID2D1RenderTarget* renderTarget);
    bool Initialize(HWND hwnd, const Size& size);
    bool Initialize(HDC hdc, const Rectangle& bounds);
    
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
    
    // DirectX-specific methods
    bool IsHardwareAccelerated() const;
    ID2D1RenderTarget* GetRenderTarget() const { return m_renderTarget.Get(); }
    
private:
    // Helper methods
    D2D1_COLOR_F ColorToD2D(const Color& color);
    Microsoft::WRL::ComPtr<ID2D1Brush> CreateBrush(const Brush& brush);
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> CreateStrokeStyle(const Pen& pen);
    Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(const Font& font);
    Microsoft::WRL::ComPtr<ID2D1Bitmap> LoadImageAsBitmap(const Image& image);
    
    // Initialization helpers
    bool InitializeFactories();
    void UpdateDirectXSettings();
};

}
}

#endif // _WIN32
