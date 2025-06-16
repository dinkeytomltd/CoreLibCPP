
#pragma once

#include "FontFamily.h"
#include <string>
#include <memory>

// Forward declarations for Skia
class SkFont;
class SkTypeface;

namespace System {
namespace Drawing {

// Forward declarations
class Graphics;

// Enumerations
enum class FontStyle {
    Regular = 0,
    Bold = 1,
    Italic = 2,
    Underline = 4,
    Strikeout = 8
};

// Enable bitwise operations for FontStyle
inline FontStyle operator|(FontStyle a, FontStyle b) {
    return static_cast<FontStyle>(static_cast<int>(a) | static_cast<int>(b));
}

inline FontStyle operator&(FontStyle a, FontStyle b) {
    return static_cast<FontStyle>(static_cast<int>(a) & static_cast<int>(b));
}

inline FontStyle operator^(FontStyle a, FontStyle b) {
    return static_cast<FontStyle>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline FontStyle operator~(FontStyle a) {
    return static_cast<FontStyle>(~static_cast<int>(a));
}

enum class GraphicsUnit {
    World = 0,
    Display = 1,
    Pixel = 2,
    Point = 3,
    Inch = 4,
    Document = 5,
    Millimeter = 6
};

class Font {
private:
    FontFamily _fontFamily;
    float _size;
    FontStyle _style;
    GraphicsUnit _unit;
    uint8_t _gdiCharSet;
    bool _gdiVerticalFont;
    mutable std::shared_ptr<SkTypeface> _skTypeface;
    mutable std::unique_ptr<SkFont> _skFont;
    
public:
    // Constructors
    Font(const FontFamily& family, float emSize);
    Font(const FontFamily& family, float emSize, FontStyle style);
    Font(const FontFamily& family, float emSize, FontStyle style, GraphicsUnit unit);
    Font(const FontFamily& family, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet);
    Font(const FontFamily& family, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet, bool gdiVerticalFont);
    
    Font(const std::string& familyName, float emSize);
    Font(const std::string& familyName, float emSize, FontStyle style);
    Font(const std::string& familyName, float emSize, FontStyle style, GraphicsUnit unit);
    Font(const std::string& familyName, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet);
    Font(const std::string& familyName, float emSize, FontStyle style, GraphicsUnit unit, uint8_t gdiCharSet, bool gdiVerticalFont);
    
    // Copy constructor and assignment
    Font(const Font& other);
    Font& operator=(const Font& other);
    
    // Move constructor and assignment
    Font(Font&& other) noexcept;
    Font& operator=(Font&& other) noexcept;
    
    // Destructor
    ~Font() = default;
    
    // Properties
    FontFamily GetFontFamily() const { return _fontFamily; }
    float GetSize() const { return _size; }
    FontStyle GetStyle() const { return _style; }
    GraphicsUnit GetUnit() const { return _unit; }
    uint8_t GetGdiCharSet() const { return _gdiCharSet; }
    bool GetGdiVerticalFont() const { return _gdiVerticalFont; }
    std::string GetName() const;
    
    // Style queries
    bool Bold() const { return (static_cast<int>(_style) & static_cast<int>(FontStyle::Bold)) != 0; }
    bool Italic() const { return (static_cast<int>(_style) & static_cast<int>(FontStyle::Italic)) != 0; }
    bool Underline() const { return (static_cast<int>(_style) & static_cast<int>(FontStyle::Underline)) != 0; }
    bool Strikeout() const { return (static_cast<int>(_style) & static_cast<int>(FontStyle::Strikeout)) != 0; }
    
    // Metrics
    float GetHeight() const;
    float GetHeight(const Graphics& graphics) const;
    float GetHeight(float dpi) const;
    int ToLogicalUnit(int value, GraphicsUnit unit) const;
    
    // Size conversion
    float SizeInPoints() const;
    
    // Equality
    bool operator==(const Font& other) const;
    bool operator!=(const Font& other) const;
    
    // String representation
    std::string ToString() const;
    
    // Skia integration
    SkFont GetSkFont() const;
    std::shared_ptr<SkTypeface> GetSkTypeface() const;
    
private:
    void InitializeSkObjects() const;
    float ConvertSizeToPixels(float size, GraphicsUnit unit) const;
    void CopyFrom(const Font& other);
};

}
}
