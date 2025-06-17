
#ifdef _WIN32

#include "DirectXBackend.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include "System/Drawing/Font.h"
#include "System/Drawing/Image.h"
#include "System/Drawing/Drawing2D/Matrix.h"
#include "System/Drawing/GraphicsConfiguration.h"

namespace System {
namespace Drawing {

DirectXBackend::DirectXBackend() 
    : m_smoothingMode(SmoothingMode::Default)
    , m_textRenderingHint(TextRenderingHint::SystemDefault) {
    m_currentTransform = D2D1::Matrix3x2F::Identity();
    InitializeFactories();
}

DirectXBackend::~DirectXBackend() {
    // COM objects will be automatically released
}

bool DirectXBackend::InitializeFactories() {
    HRESULT hr = S_OK;
    
    // Create D2D1 Factory
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &m_d2dFactory
    );
    if (FAILED(hr)) return false;
    
    // Create DirectWrite Factory
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(m_writeFactory.GetAddressOf())
    );
    if (FAILED(hr)) return false;
    
    // Create WIC Factory
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_wicFactory)
    );
    if (FAILED(hr)) return false;
    
    return true;
}

bool DirectXBackend::Initialize(ID2D1RenderTarget* renderTarget) {
    m_renderTarget = renderTarget;
    return renderTarget != nullptr;
}

bool DirectXBackend::Initialize(HWND hwnd, const Size& size) {
    if (!m_d2dFactory) return false;
    
    D2D1_SIZE_U pixelSize = D2D1::SizeU(size.Width, size.Height);
    
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> hwndRenderTarget;
    HRESULT hr = m_d2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, pixelSize),
        &hwndRenderTarget
    );
    
    if (SUCCEEDED(hr)) {
        m_renderTarget = hwndRenderTarget;
        return true;
    }
    
    return false;
}

bool DirectXBackend::Initialize(HDC hdc, const Rectangle& bounds) {
    if (!m_d2dFactory) return false;
    
    D2D1_SIZE_U pixelSize = D2D1::SizeU(bounds.Width, bounds.Height);
    
    Microsoft::WRL::ComPtr<ID2D1DCRenderTarget> dcRenderTarget;
    HRESULT hr = m_d2dFactory->CreateDCRenderTarget(
        D2D1::RenderTargetProperties(),
        &dcRenderTarget
    );
    
    if (SUCCEEDED(hr)) {
        RECT rect = { bounds.X, bounds.Y, bounds.X + bounds.Width, bounds.Y + bounds.Height };
        hr = dcRenderTarget->BindDC(hdc, &rect);
        
        if (SUCCEEDED(hr)) {
            m_renderTarget = dcRenderTarget;
            return true;
        }
    }
    
    return false;
}

void DirectXBackend::DrawLine(const Pen& pen, const Point& pt1, const Point& pt2) {
    DrawLine(pen, PointF(static_cast<float>(pt1.X), static_cast<float>(pt1.Y)), 
             PointF(static_cast<float>(pt2.X), static_cast<float>(pt2.Y)));
}

void DirectXBackend::DrawLine(const Pen& pen, const PointF& pt1, const PointF& pt2) {
    if (!m_renderTarget) return;
    
    auto brush = CreateBrush(pen.GetBrush());
    auto strokeStyle = CreateStrokeStyle(pen);
    
    D2D1_POINT_2F d2dPt1 = D2D1::Point2F(pt1.X, pt1.Y);
    D2D1_POINT_2F d2dPt2 = D2D1::Point2F(pt2.X, pt2.Y);
    
    m_renderTarget->DrawLine(d2dPt1, d2dPt2, brush.Get(), pen.GetWidth(), strokeStyle.Get());
}

void DirectXBackend::DrawRectangle(const Pen& pen, const Rectangle& rect) {
    DrawRectangle(pen, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                  static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void DirectXBackend::DrawRectangle(const Pen& pen, const RectangleF& rect) {
    if (!m_renderTarget) return;
    
    auto brush = CreateBrush(pen.GetBrush());
    auto strokeStyle = CreateStrokeStyle(pen);
    
    D2D1_RECT_F d2dRect = D2D1::RectF(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
    
    m_renderTarget->DrawRectangle(d2dRect, brush.Get(), pen.GetWidth(), strokeStyle.Get());
}

void DirectXBackend::FillRectangle(const Brush& brush, const Rectangle& rect) {
    FillRectangle(brush, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                    static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void DirectXBackend::FillRectangle(const Brush& brush, const RectangleF& rect) {
    if (!m_renderTarget) return;
    
    auto d2dBrush = CreateBrush(brush);
    D2D1_RECT_F d2dRect = D2D1::RectF(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
    
    m_renderTarget->FillRectangle(d2dRect, d2dBrush.Get());
}

void DirectXBackend::DrawEllipse(const Pen& pen, const Rectangle& rect) {
    DrawEllipse(pen, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void DirectXBackend::DrawEllipse(const Pen& pen, const RectangleF& rect) {
    if (!m_renderTarget) return;
    
    auto brush = CreateBrush(pen.GetBrush());
    auto strokeStyle = CreateStrokeStyle(pen);
    
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(rect.X + rect.Width / 2, rect.Y + rect.Height / 2),
        rect.Width / 2,
        rect.Height / 2
    );
    
    m_renderTarget->DrawEllipse(ellipse, brush.Get(), pen.GetWidth(), strokeStyle.Get());
}

void DirectXBackend::FillEllipse(const Brush& brush, const Rectangle& rect) {
    FillEllipse(brush, RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                                  static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void DirectXBackend::FillEllipse(const Brush& brush, const RectangleF& rect) {
    if (!m_renderTarget) return;
    
    auto d2dBrush = CreateBrush(brush);
    
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(rect.X + rect.Width / 2, rect.Y + rect.Height / 2),
        rect.Width / 2,
        rect.Height / 2
    );
    
    m_renderTarget->FillEllipse(ellipse, d2dBrush.Get());
}

void DirectXBackend::DrawString(const std::string& text, const Font& font, const Brush& brush, const PointF& point) {
    // TODO: Implement text drawing with DirectWrite
}

void DirectXBackend::DrawImage(const Image& image, const Point& point) {
    DrawImage(image, PointF(static_cast<float>(point.X), static_cast<float>(point.Y)));
}

void DirectXBackend::DrawImage(const Image& image, const PointF& point) {
    // TODO: Implement image drawing with DirectX
}

void DirectXBackend::DrawImage(const Image& image, const Rectangle& destRect, const Rectangle& srcRect) {
    // TODO: Implement image drawing with rectangles
}

void DirectXBackend::DrawImage(const Image& image, const RectangleF& destRect, const RectangleF& srcRect) {
    // TODO: Implement image drawing with rectangles
}

void DirectXBackend::SetClip(const Rectangle& rect) {
    SetClip(RectangleF(static_cast<float>(rect.X), static_cast<float>(rect.Y),
                       static_cast<float>(rect.Width), static_cast<float>(rect.Height)));
}

void DirectXBackend::SetClip(const RectangleF& rect) {
    if (!m_renderTarget) return;
    
    D2D1_RECT_F d2dRect = D2D1::RectF(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
    m_renderTarget->PushAxisAlignedClip(d2dRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}

void DirectXBackend::ResetClip() {
    if (!m_renderTarget) return;
    m_renderTarget->PopAxisAlignedClip();
}

void DirectXBackend::SetTransform(const Matrix& matrix) {
    // TODO: Convert Matrix to D2D1::Matrix3x2F and apply
}

void DirectXBackend::TranslateTransform(float dx, float dy) {
    if (!m_renderTarget) return;
    
    m_currentTransform = m_currentTransform * D2D1::Matrix3x2F::Translation(dx, dy);
    m_renderTarget->SetTransform(m_currentTransform);
}

void DirectXBackend::RotateTransform(float angle) {
    if (!m_renderTarget) return;
    
    m_currentTransform = m_currentTransform * D2D1::Matrix3x2F::Rotation(angle);
    m_renderTarget->SetTransform(m_currentTransform);
}

void DirectXBackend::ScaleTransform(float sx, float sy) {
    if (!m_renderTarget) return;
    
    m_currentTransform = m_currentTransform * D2D1::Matrix3x2F::Scale(sx, sy);
    m_renderTarget->SetTransform(m_currentTransform);
}

void DirectXBackend::ResetTransform() {
    if (!m_renderTarget) return;
    
    m_currentTransform = D2D1::Matrix3x2F::Identity();
    m_renderTarget->SetTransform(m_currentTransform);
}

SmoothingMode DirectXBackend::GetSmoothingMode() const {
    return m_smoothingMode;
}

void DirectXBackend::SetSmoothingMode(SmoothingMode mode) {
    m_smoothingMode = mode;
    UpdateDirectXSettings();
}

TextRenderingHint DirectXBackend::GetTextRenderingHint() const {
    return m_textRenderingHint;
}

void DirectXBackend::SetTextRenderingHint(TextRenderingHint hint) {
    m_textRenderingHint = hint;
    UpdateDirectXSettings();
}

void DirectXBackend::BeginDraw() {
    if (!m_renderTarget) return;
    m_renderTarget->BeginDraw();
}

void DirectXBackend::EndDraw() {
    if (!m_renderTarget) return;
    m_renderTarget->EndDraw();
}

void DirectXBackend::Clear(const Color& color) {
    if (!m_renderTarget) return;
    
    D2D1_COLOR_F d2dColor = ColorToD2D(color);
    m_renderTarget->Clear(d2dColor);
}

void DirectXBackend::Flush() {
    if (!m_renderTarget) return;
    m_renderTarget->Flush();
}

void DirectXBackend::Save() {
    // TODO: Implement state saving
}

void DirectXBackend::Restore() {
    // TODO: Implement state restoration
}

SizeF DirectXBackend::MeasureString(const std::string& text, const Font& font) {
    // TODO: Implement text measurement with DirectWrite
    return SizeF(0, 0);
}

bool DirectXBackend::IsHardwareAccelerated() const {
    // TODO: Check if render target is hardware accelerated
    return true;
}

D2D1_COLOR_F DirectXBackend::ColorToD2D(const Color& color) {
    return D2D1::ColorF(
        color.R / 255.0f,
        color.G / 255.0f,
        color.B / 255.0f,
        color.A / 255.0f
    );
}

Microsoft::WRL::ComPtr<ID2D1Brush> DirectXBackend::CreateBrush(const Brush& brush) {
    // TODO: Convert Brush to ID2D1Brush
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidBrush;
    if (m_renderTarget) {
        m_renderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &solidBrush
        );
    }
    return solidBrush;
}

Microsoft::WRL::ComPtr<ID2D1StrokeStyle> DirectXBackend::CreateStrokeStyle(const Pen& pen) {
    // TODO: Convert Pen to ID2D1StrokeStyle
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> strokeStyle;
    if (m_d2dFactory) {
        m_d2dFactory->CreateStrokeStyle(
            D2D1::StrokeStyleProperties(),
            nullptr,
            0,
            &strokeStyle
        );
    }
    return strokeStyle;
}

Microsoft::WRL::ComPtr<IDWriteTextFormat> DirectXBackend::CreateTextFormat(const Font& font) {
    // TODO: Convert Font to IDWriteTextFormat
    Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
    return textFormat;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap> DirectXBackend::LoadImageAsBitmap(const Image& image) {
    // TODO: Convert Image to ID2D1Bitmap
    Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap;
    return bitmap;
}

void DirectXBackend::UpdateDirectXSettings() {
    if (!m_renderTarget) return;
    
    // Apply smoothing mode
    D2D1_ANTIALIAS_MODE antialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
    switch (m_smoothingMode) {
        case SmoothingMode::None:
        case SmoothingMode::HighSpeed:
            antialiasMode = D2D1_ANTIALIAS_MODE_ALIASED;
            break;
        case SmoothingMode::AntiAlias:
        case SmoothingMode::HighQuality:
        default:
            antialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
            break;
    }
    m_renderTarget->SetAntialiasMode(antialiasMode);
    
    // Apply text rendering hint
    D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
    switch (m_textRenderingHint) {
        case TextRenderingHint::SingleBitPerPixel:
        case TextRenderingHint::SingleBitPerPixelGridFit:
            textAntialiasMode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
            break;
        case TextRenderingHint::AntiAlias:
        case TextRenderingHint::AntiAliasGridFit:
            textAntialiasMode = D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE;
            break;
        case TextRenderingHint::ClearTypeGridFit:
            textAntialiasMode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
            break;
        default:
            textAntialiasMode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
            break;
    }
    m_renderTarget->SetTextAntialiasMode(textAntialiasMode);
}

}
}

#endif // _WIN32
