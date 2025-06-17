
#ifdef _WIN32

#include "GdiPlusBackend.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include "System/Drawing/Font.h"
#include "System/Drawing/Image.h"
#include "System/Drawing/Drawing2D/Matrix.h"
#include <stdexcept>

namespace System {
namespace Drawing {

// Static member definitions
ULONG_PTR GdiPlusBackend::s_gdiplusToken = 0;
int GdiPlusBackend::s_refCount = 0;
std::mutex GdiPlusBackend::s_initMutex;

GdiPlusBackend::GdiPlusBackend()
    : m_graphics(nullptr)
    , m_hdc(nullptr)
    , m_hwnd(nullptr)
    , m_ownsGraphics(false)
{
    InitializeGdiPlus();
}

GdiPlusBackend::~GdiPlusBackend() {
    if (m_ownsGraphics && m_graphics) {
        delete m_graphics;
    }
    ShutdownGdiPlus();
}

bool GdiPlusBackend::Initialize(HDC hdc) {
    m_hdc = hdc;
    m_graphics = new Gdiplus::Graphics(hdc);
    m_ownsGraphics = true;
    return m_graphics != nullptr;
}

bool GdiPlusBackend::Initialize(HWND hwnd) {
    m_hwnd = hwnd;
    m_graphics = new Gdiplus::Graphics(hwnd);
    m_ownsGraphics = true;
    return m_graphics != nullptr;
}

bool GdiPlusBackend::Initialize(int width, int height) {
    m_bitmap = std::make_unique<Gdiplus::Bitmap>(width, height, PixelFormat32bppARGB);
    m_graphics = new Gdiplus::Graphics(m_bitmap.get());
    m_ownsGraphics = true;
    return m_graphics != nullptr;
}

bool GdiPlusBackend::Initialize(Gdiplus::Graphics* graphics) {
    m_graphics = graphics;
    m_ownsGraphics = false;
    return m_graphics != nullptr;
}

// Drawing operations - NYI stubs for now
void GdiPlusBackend::DrawLine(const Pen& pen, const Point& pt1, const Point& pt2) {
    throw std::runtime_error("GdiPlusBackend::DrawLine(Point) - NYI");
}

void GdiPlusBackend::DrawLine(const Pen& pen, const PointF& pt1, const PointF& pt2) {
    throw std::runtime_error("GdiPlusBackend::DrawLine(PointF) - NYI");
}

void GdiPlusBackend::DrawRectangle(const Pen& pen, const Rectangle& rect) {
    throw std::runtime_error("GdiPlusBackend::DrawRectangle(Rectangle) - NYI");
}

void GdiPlusBackend::DrawRectangle(const Pen& pen, const RectangleF& rect) {
    throw std::runtime_error("GdiPlusBackend::DrawRectangle(RectangleF) - NYI");
}

void GdiPlusBackend::FillRectangle(const Brush& brush, const Rectangle& rect) {
    throw std::runtime_error("GdiPlusBackend::FillRectangle(Rectangle) - NYI");
}

void GdiPlusBackend::FillRectangle(const Brush& brush, const RectangleF& rect) {
    throw std::runtime_error("GdiPlusBackend::FillRectangle(RectangleF) - NYI");
}

void GdiPlusBackend::DrawEllipse(const Pen& pen, const Rectangle& rect) {
    throw std::runtime_error("GdiPlusBackend::DrawEllipse(Rectangle) - NYI");
}

void GdiPlusBackend::DrawEllipse(const Pen& pen, const RectangleF& rect) {
    throw std::runtime_error("GdiPlusBackend::DrawEllipse(RectangleF) - NYI");
}

void GdiPlusBackend::FillEllipse(const Brush& brush, const Rectangle& rect) {
    throw std::runtime_error("GdiPlusBackend::FillEllipse(Rectangle) - NYI");
}

void GdiPlusBackend::FillEllipse(const Brush& brush, const RectangleF& rect) {
    throw std::runtime_error("GdiPlusBackend::FillEllipse(RectangleF) - NYI");
}

void GdiPlusBackend::DrawString(const std::string& text, const Font& font, const Brush& brush, const PointF& point) {
    throw std::runtime_error("GdiPlusBackend::DrawString - NYI");
}

void GdiPlusBackend::DrawImage(const Image& image, const Point& point) {
    throw std::runtime_error("GdiPlusBackend::DrawImage(Point) - NYI");
}

void GdiPlusBackend::DrawImage(const Image& image, const PointF& point) {
    throw std::runtime_error("GdiPlusBackend::DrawImage(PointF) - NYI");
}

void GdiPlusBackend::DrawImage(const Image& image, const Rectangle& destRect, const Rectangle& srcRect) {
    throw std::runtime_error("GdiPlusBackend::DrawImage(Rectangle) - NYI");
}

void GdiPlusBackend::DrawImage(const Image& image, const RectangleF& destRect, const RectangleF& srcRect) {
    throw std::runtime_error("GdiPlusBackend::DrawImage(RectangleF) - NYI");
}

// Graphics state management - NYI stubs
void GdiPlusBackend::SetClip(const Rectangle& rect) {
    throw std::runtime_error("GdiPlusBackend::SetClip(Rectangle) - NYI");
}

void GdiPlusBackend::SetClip(const RectangleF& rect) {
    throw std::runtime_error("GdiPlusBackend::SetClip(RectangleF) - NYI");
}

void GdiPlusBackend::ResetClip() {
    throw std::runtime_error("GdiPlusBackend::ResetClip - NYI");
}

void GdiPlusBackend::SetTransform(const Matrix& matrix) {
    throw std::runtime_error("GdiPlusBackend::SetTransform - NYI");
}

void GdiPlusBackend::TranslateTransform(float dx, float dy) {
    throw std::runtime_error("GdiPlusBackend::TranslateTransform - NYI");
}

void GdiPlusBackend::RotateTransform(float angle) {
    throw std::runtime_error("GdiPlusBackend::RotateTransform - NYI");
}

void GdiPlusBackend::ScaleTransform(float sx, float sy) {
    throw std::runtime_error("GdiPlusBackend::ScaleTransform - NYI");
}

void GdiPlusBackend::ResetTransform() {
    throw std::runtime_error("GdiPlusBackend::ResetTransform - NYI");
}

// Properties - NYI stubs
SmoothingMode GdiPlusBackend::GetSmoothingMode() const {
    return SmoothingMode::Default;
}

void GdiPlusBackend::SetSmoothingMode(SmoothingMode mode) {
    // NYI stub
}

TextRenderingHint GdiPlusBackend::GetTextRenderingHint() const {
    return TextRenderingHint::SystemDefault;
}

void GdiPlusBackend::SetTextRenderingHint(TextRenderingHint hint) {
    // NYI stub
}

// Backend operations - NYI stubs
void GdiPlusBackend::BeginDraw() {
    // NYI stub
}

void GdiPlusBackend::EndDraw() {
    // NYI stub
}

void GdiPlusBackend::Clear(const Color& color) {
    throw std::runtime_error("GdiPlusBackend::Clear - NYI");
}

void GdiPlusBackend::Flush() {
    if (m_graphics) {
        m_graphics->Flush();
    }
}

void GdiPlusBackend::Save() {
    throw std::runtime_error("GdiPlusBackend::Save - NYI");
}

void GdiPlusBackend::Restore() {
    throw std::runtime_error("GdiPlusBackend::Restore - NYI");
}

SizeF GdiPlusBackend::MeasureString(const std::string& text, const Font& font) {
    throw std::runtime_error("GdiPlusBackend::MeasureString - NYI");
}

// Helper methods - NYI stubs
Gdiplus::Color GdiPlusBackend::ColorToGdiPlus(const Color& color) {
    return Gdiplus::Color(255, 0, 0, 0); // NYI stub
}

std::unique_ptr<Gdiplus::Pen> GdiPlusBackend::CreateGdiplusPen(const Pen& pen) {
    return std::make_unique<Gdiplus::Pen>(Gdiplus::Color::Black); // NYI stub
}

std::unique_ptr<Gdiplus::Brush> GdiPlusBackend::CreateGdiplusBrush(const Brush& brush) {
    return std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color::Black); // NYI stub
}

std::unique_ptr<Gdiplus::Font> GdiPlusBackend::CreateGdiplusFont(const Font& font) {
    return std::make_unique<Gdiplus::Font>(L"Arial", 12.0f); // NYI stub
}

Gdiplus::Matrix GdiPlusBackend::CreateGdiplusMatrix(const Matrix& matrix) {
    return Gdiplus::Matrix(); // NYI stub
}

// GDI+ startup/shutdown
bool GdiPlusBackend::InitializeGdiPlus() {
    std::lock_guard<std::mutex> lock(s_initMutex);
    
    if (s_refCount == 0) {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::Status status = Gdiplus::GdiplusStartup(&s_gdiplusToken, &gdiplusStartupInput, nullptr);
        if (status != Gdiplus::Ok) {
            return false;
        }
    }
    
    s_refCount++;
    return true;
}

void GdiPlusBackend::ShutdownGdiPlus() {
    std::lock_guard<std::mutex> lock(s_initMutex);
    
    s_refCount--;
    if (s_refCount == 0 && s_gdiplusToken != 0) {
        Gdiplus::GdiplusShutdown(s_gdiplusToken);
        s_gdiplusToken = 0;
    }
}

}
}

#endif // _WIN32
