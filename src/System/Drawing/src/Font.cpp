
#include "System/Drawing/Font.h"
#include "System/Drawing/Graphics.h"
#include <stdexcept>
#include <sstream>

// Skia includes
#include "include/core/SkFont.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkFontMgr.h"

namespace System {
namespace Drawing {

Font::Font(const FontFamily& family, float emSize)
    : Font(family, emSize, FontStyle::Regular, GraphicsUnit::Point, 1, false) {
}

Font::Font(const FontFamily& family, float emSize, FontStyle style)
    : Font(family, emSize, style, GraphicsUnit::Point, 1, false) {
}

Font::Font(const FontFamily& family, float emSize, FontStyle style, GraphicsUnit unit)
    : Font(family, emSize, style, unit, 1, false) {
}

Font::Font(const FontFamily& family, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet)
    : Font(family, emSize, style, unit, gdiCharSet, false) {
}

Font::Font(const FontFamily& family, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet, bool gdiVerticalFont)
    : _fontFamily(family)
    , _size(emSize)
    , _style(style)
    , _unit(unit)
    , _gdiCharSet(gdiCharSet)
    , _gdiVerticalFont(gdiVerticalFont) {
    
    if (emSize <= 0) {
        throw std::invalid_argument("Font size must be positive");
    }
}

Font::Font(const std::string& familyName, float emSize)
    : Font(FontFamily(familyName), emSize) {
}

Font::Font(const std::string& familyName, float emSize, FontStyle style)
    : Font(FontFamily(familyName), emSize, style) {
}

Font::Font(const std::string& familyName, float emSize, FontStyle style, GraphicsUnit unit)
    : Font(FontFamily(familyName), emSize, style, unit) {
}

Font::Font(const std::string& familyName, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet)
    : Font(FontFamily(familyName), emSize, style, unit, gdiCharSet) {
}

Font::Font(const std::string& familyName, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet, bool gdiVerticalFont)
    : Font(FontFamily(familyName), emSize, style, unit, gdiCharSet, gdiVerticalFont) {
}

Font::Font(const Font& other) {
    CopyFrom(other);
}

Font& Font::operator=(const Font& other) {
    if (this != &other) {
        CopyFrom(other);
    }
    return *this;
}

Font::Font(Font&& other) noexcept
    : _fontFamily(std::move(other._fontFamily))
    , _size(other._size)
    , _style(other._style)
    , _unit(other._unit)
    , _gdiCharSet(other._gdiCharSet)
    , _gdiVerticalFont(other._gdiVerticalFont)
    , _skTypeface(std::move(other._skTypeface))
    , _skFont(std::move(other._skFont)) {
}

Font& Font::operator=(Font&& other) noexcept {
    if (this != &other) {
        _fontFamily = std::move(other._fontFamily);
        _size = other._size;
        _style = other._style;
        _unit = other._unit;
        _gdiCharSet = other._gdiCharSet;
        _gdiVerticalFont = other._gdiVerticalFont;
        _skTypeface = std::move(other._skTypeface);
        _skFont = std::move(other._skFont);
    }
    return *this;
}

std::string Font::GetName() const {
    return _fontFamily.GetName();
}

float Font::GetHeight() const {
    return GetHeight(96.0f); // Default DPI
}

float Font::GetHeight(const Graphics& graphics) const {
    return GetHeight(graphics.DpiY());
}

float Font::GetHeight(float dpi) const {
    InitializeSkObjects();
    if (_skFont) {
        SkFontMetrics metrics;
        _skFont->getMetrics(&metrics);
        return metrics.fDescent - metrics.fAscent;
    }
    
    // Fallback calculation
    float pixelSize = ConvertSizeToPixels(_size, _unit);
    return pixelSize * 1.2f; // Approximate line height
}

int Font::ToLogicalUnit(int value, GraphicsUnit unit) const {
    // TODO: Implement unit conversion
    return value;
}

float Font::SizeInPoints() const {
    switch (_unit) {
        case GraphicsUnit::Point:
            return _size;
        case GraphicsUnit::Pixel:
            return _size * 72.0f / 96.0f; // Convert pixels to points
        case GraphicsUnit::Inch:
            return _size * 72.0f;
        case GraphicsUnit::Millimeter:
            return _size * 72.0f / 25.4f;
        default:
            return _size;
    }
}

bool Font::operator==(const Font& other) const {
    return _fontFamily.GetName() == other._fontFamily.GetName() &&
           std::abs(_size - other._size) < 1e-6f &&
           _style == other._style &&
           _unit == other._unit;
}

bool Font::operator!=(const Font& other) const {
    return !(*this == other);
}

std::string Font::ToString() const {
    std::ostringstream oss;
    oss << "[Font: Name=" << GetName() << ", Size=" << _size;
    
    if (Bold()) oss << ", Bold";
    if (Italic()) oss << ", Italic";
    if (Underline()) oss << ", Underline";
    if (Strikeout()) oss << ", Strikeout";
    
    oss << "]";
    return oss.str();
}

SkFont Font::GetSkFont() const {
    InitializeSkObjects();
    if (_skFont) {
        return *_skFont;
    }
    
    // Fallback
    SkFont font;
    font.setSize(ConvertSizeToPixels(_size, _unit));
    return font;
}

std::shared_ptr<SkTypeface> Font::GetSkTypeface() const {
    InitializeSkObjects();
    return _skTypeface;
}

void Font::InitializeSkObjects() const {
    if (_skTypeface && _skFont) {
        return; // Already initialized
    }
    
    // Create typeface
    _skTypeface = _fontFamily.CreateTypeface(_style);
    if (!_skTypeface) {
        // Fallback to default typeface
        _skTypeface = SkTypeface::MakeDefault();
    }
    
    // Create font
    _skFont = std::make_unique<SkFont>(_skTypeface, ConvertSizeToPixels(_size, _unit));
    
    // Apply style properties
    if (Bold()) {
        _skFont->setEmbolden(true);
    }
    
    if (Italic()) {
        _skFont->setSkewX(-0.25f); // Approximate italic skew
    }
}

float Font::ConvertSizeToPixels(float size, GraphicsUnit unit) const {
    switch (unit) {
        case GraphicsUnit::Pixel:
            return size;
        case GraphicsUnit::Point:
            return size * 96.0f / 72.0f; // Convert points to pixels
        case GraphicsUnit::Inch:
            return size * 96.0f;
        case GraphicsUnit::Millimeter:
            return size * 96.0f / 25.4f;
        case GraphicsUnit::Document:
            return size * 96.0f / 300.0f; // 300 DPI document units
        default:
            return size;
    }
}

void Font::CopyFrom(const Font& other) {
    _fontFamily = other._fontFamily;
    _size = other._size;
    _style = other._style;
    _unit = other._unit;
    _gdiCharSet = other._gdiCharSet;
    _gdiVerticalFont = other._gdiVerticalFont;
    
    // Don't copy Skia objects - they will be recreated as needed
    _skTypeface.reset();
    _skFont.reset();
}

}
}
