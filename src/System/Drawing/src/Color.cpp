
#include "System/Drawing/Color.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <cmath>

// Skia includes
#include "include/core/SkColor.h"

namespace System {
namespace Drawing {

// Named color definitions
const Color Color::Transparent = Color(0, 255, 255, 255);
const Color Color::AliceBlue = Color(240, 248, 255);
const Color Color::AntiqueWhite = Color(250, 235, 215);
const Color Color::Aqua = Color(0, 255, 255);
const Color Color::Aquamarine = Color(127, 255, 212);
const Color Color::Azure = Color(240, 255, 255);
const Color Color::Beige = Color(245, 245, 220);
const Color Color::Bisque = Color(255, 228, 196);
const Color Color::Black = Color(0, 0, 0);
const Color Color::BlanchedAlmond = Color(255, 235, 205);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::BlueViolet = Color(138, 43, 226);
const Color Color::Brown = Color(165, 42, 42);
const Color Color::BurlyWood = Color(222, 184, 135);
const Color Color::CadetBlue = Color(95, 158, 160);
const Color Color::Chartreuse = Color(127, 255, 0);
const Color Color::Chocolate = Color(210, 105, 30);
const Color Color::Coral = Color(255, 127, 80);
const Color Color::CornflowerBlue = Color(100, 149, 237);
const Color Color::Cornsilk = Color(255, 248, 220);
const Color Color::Crimson = Color(220, 20, 60);
const Color Color::Cyan = Color(0, 255, 255);
const Color Color::DarkBlue = Color(0, 0, 139);
const Color Color::DarkCyan = Color(0, 139, 139);
const Color Color::DarkGoldenrod = Color(184, 134, 11);
const Color Color::DarkGray = Color(169, 169, 169);
const Color Color::DarkGreen = Color(0, 100, 0);
const Color Color::DarkKhaki = Color(189, 183, 107);
const Color Color::DarkMagenta = Color(139, 0, 139);
const Color Color::DarkOliveGreen = Color(85, 107, 47);
const Color Color::DarkOrange = Color(255, 140, 0);
const Color Color::DarkOrchid = Color(153, 50, 204);
const Color Color::DarkRed = Color(139, 0, 0);
const Color Color::DarkSalmon = Color(233, 150, 122);
const Color Color::DarkSeaGreen = Color(143, 188, 143);
const Color Color::DarkSlateBlue = Color(72, 61, 139);
const Color Color::DarkSlateGray = Color(47, 79, 79);
const Color Color::DarkTurquoise = Color(0, 206, 209);
const Color Color::DarkViolet = Color(148, 0, 211);
const Color Color::DeepPink = Color(255, 20, 147);
const Color Color::DeepSkyBlue = Color(0, 191, 255);
const Color Color::DimGray = Color(105, 105, 105);
const Color Color::DodgerBlue = Color(30, 144, 255);
const Color Color::Firebrick = Color(178, 34, 34);
const Color Color::FloralWhite = Color(255, 250, 240);
const Color Color::ForestGreen = Color(34, 139, 34);
const Color Color::Fuchsia = Color(255, 0, 255);
const Color Color::Gainsboro = Color(220, 220, 220);
const Color Color::GhostWhite = Color(248, 248, 255);
const Color Color::Gold = Color(255, 215, 0);
const Color Color::Goldenrod = Color(218, 165, 32);
const Color Color::Gray = Color(128, 128, 128);
const Color Color::Green = Color(0, 128, 0);
const Color Color::GreenYellow = Color(173, 255, 47);
const Color Color::Honeydew = Color(240, 255, 240);
const Color Color::HotPink = Color(255, 105, 180);
const Color Color::IndianRed = Color(205, 92, 92);
const Color Color::Indigo = Color(75, 0, 130);
const Color Color::Ivory = Color(255, 255, 240);
const Color Color::Khaki = Color(240, 230, 140);
const Color Color::Lavender = Color(230, 230, 250);
const Color Color::LavenderBlush = Color(255, 240, 245);
const Color Color::LawnGreen = Color(124, 252, 0);
const Color Color::LemonChiffon = Color(255, 250, 205);
const Color Color::LightBlue = Color(173, 216, 230);
const Color Color::LightCoral = Color(240, 128, 128);
const Color Color::LightCyan = Color(224, 255, 255);
const Color Color::LightGoldenrodYellow = Color(250, 250, 210);
const Color Color::LightGray = Color(211, 211, 211);
const Color Color::LightGreen = Color(144, 238, 144);
const Color Color::LightPink = Color(255, 182, 193);
const Color Color::LightSalmon = Color(255, 160, 122);
const Color Color::LightSeaGreen = Color(32, 178, 170);
const Color Color::LightSkyBlue = Color(135, 206, 250);
const Color Color::LightSlateGray = Color(119, 136, 153);
const Color Color::LightSteelBlue = Color(176, 196, 222);
const Color Color::LightYellow = Color(255, 255, 224);
const Color Color::Lime = Color(0, 255, 0);
const Color Color::LimeGreen = Color(50, 205, 50);
const Color Color::Linen = Color(250, 240, 230);
const Color Color::Magenta = Color(255, 0, 255);
const Color Color::Maroon = Color(128, 0, 0);
const Color Color::MediumAquamarine = Color(102, 205, 170);
const Color Color::MediumBlue = Color(0, 0, 205);
const Color Color::MediumOrchid = Color(186, 85, 211);
const Color Color::MediumPurple = Color(147, 112, 219);
const Color Color::MediumSeaGreen = Color(60, 179, 113);
const Color Color::MediumSlateBlue = Color(123, 104, 238);
const Color Color::MediumSpringGreen = Color(0, 250, 154);
const Color Color::MediumTurquoise = Color(72, 209, 204);
const Color Color::MediumVioletRed = Color(199, 21, 133);
const Color Color::MidnightBlue = Color(25, 25, 112);
const Color Color::MintCream = Color(245, 255, 250);
const Color Color::MistyRose = Color(255, 228, 225);
const Color Color::Moccasin = Color(255, 228, 181);
const Color Color::NavajoWhite = Color(255, 222, 173);
const Color Color::Navy = Color(0, 0, 128);
const Color Color::OldLace = Color(253, 245, 230);
const Color Color::Olive = Color(128, 128, 0);
const Color Color::OliveDrab = Color(107, 142, 35);
const Color Color::Orange = Color(255, 165, 0);
const Color Color::OrangeRed = Color(255, 69, 0);
const Color Color::Orchid = Color(218, 112, 214);
const Color Color::PaleGoldenrod = Color(238, 232, 170);
const Color Color::PaleGreen = Color(152, 251, 152);
const Color Color::PaleTurquoise = Color(175, 238, 238);
const Color Color::PaleVioletRed = Color(219, 112, 147);
const Color Color::PapayaWhip = Color(255, 239, 213);
const Color Color::PeachPuff = Color(255, 218, 185);
const Color Color::Peru = Color(205, 133, 63);
const Color Color::Pink = Color(255, 192, 203);
const Color Color::Plum = Color(221, 160, 221);
const Color Color::PowderBlue = Color(176, 224, 230);
const Color Color::Purple = Color(128, 0, 128);
const Color Color::Red = Color(255, 0, 0);
const Color Color::RosyBrown = Color(188, 143, 143);
const Color Color::RoyalBlue = Color(65, 105, 225);
const Color Color::SaddleBrown = Color(139, 69, 19);
const Color Color::Salmon = Color(250, 128, 114);
const Color Color::SandyBrown = Color(244, 164, 96);
const Color Color::SeaGreen = Color(46, 139, 87);
const Color Color::SeaShell = Color(255, 245, 238);
const Color Color::Sienna = Color(160, 82, 45);
const Color Color::Silver = Color(192, 192, 192);
const Color Color::SkyBlue = Color(135, 206, 235);
const Color Color::SlateBlue = Color(106, 90, 205);
const Color Color::SlateGray = Color(112, 128, 144);
const Color Color::Snow = Color(255, 250, 250);
const Color Color::SpringGreen = Color(0, 255, 127);
const Color Color::SteelBlue = Color(70, 130, 180);
const Color Color::Tan = Color(210, 180, 140);
const Color Color::Teal = Color(0, 128, 128);
const Color Color::Thistle = Color(216, 191, 216);
const Color Color::Tomato = Color(255, 99, 71);
const Color Color::Turquoise = Color(64, 224, 208);
const Color Color::Violet = Color(238, 130, 238);
const Color Color::Wheat = Color(245, 222, 179);
const Color Color::White = Color(255, 255, 255);
const Color Color::WhiteSmoke = Color(245, 245, 245);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::YellowGreen = Color(154, 205, 50);

// Static color name map for lookup
static std::unordered_map<std::string, Color> CreateColorMap() {
    std::unordered_map<std::string, Color> colorMap;
    colorMap["Transparent"] = Color::Transparent;
    colorMap["AliceBlue"] = Color::AliceBlue;
    colorMap["AntiqueWhite"] = Color::AntiqueWhite;
    colorMap["Aqua"] = Color::Aqua;
    colorMap["Aquamarine"] = Color::Aquamarine;
    colorMap["Azure"] = Color::Azure;
    colorMap["Beige"] = Color::Beige;
    colorMap["Bisque"] = Color::Bisque;
    colorMap["Black"] = Color::Black;
    colorMap["BlanchedAlmond"] = Color::BlanchedAlmond;
    colorMap["Blue"] = Color::Blue;
    colorMap["BlueViolet"] = Color::BlueViolet;
    colorMap["Brown"] = Color::Brown;
    colorMap["BurlyWood"] = Color::BurlyWood;
    colorMap["CadetBlue"] = Color::CadetBlue;
    colorMap["Chartreuse"] = Color::Chartreuse;
    colorMap["Chocolate"] = Color::Chocolate;
    colorMap["Coral"] = Color::Coral;
    colorMap["CornflowerBlue"] = Color::CornflowerBlue;
    colorMap["Cornsilk"] = Color::Cornsilk;
    colorMap["Crimson"] = Color::Crimson;
    colorMap["Cyan"] = Color::Cyan;
    colorMap["DarkBlue"] = Color::DarkBlue;
    colorMap["DarkCyan"] = Color::DarkCyan;
    colorMap["DarkGoldenrod"] = Color::DarkGoldenrod;
    colorMap["DarkGray"] = Color::DarkGray;
    colorMap["DarkGreen"] = Color::DarkGreen;
    colorMap["DarkKhaki"] = Color::DarkKhaki;
    colorMap["DarkMagenta"] = Color::DarkMagenta;
    colorMap["DarkOliveGreen"] = Color::DarkOliveGreen;
    colorMap["DarkOrange"] = Color::DarkOrange;
    colorMap["DarkOrchid"] = Color::DarkOrchid;
    colorMap["DarkRed"] = Color::DarkRed;
    colorMap["DarkSalmon"] = Color::DarkSalmon;
    colorMap["DarkSeaGreen"] = Color::DarkSeaGreen;
    colorMap["DarkSlateBlue"] = Color::DarkSlateBlue;
    colorMap["DarkSlateGray"] = Color::DarkSlateGray;
    colorMap["DarkTurquoise"] = Color::DarkTurquoise;
    colorMap["DarkViolet"] = Color::DarkViolet;
    colorMap["DeepPink"] = Color::DeepPink;
    colorMap["DeepSkyBlue"] = Color::DeepSkyBlue;
    colorMap["DimGray"] = Color::DimGray;
    colorMap["DodgerBlue"] = Color::DodgerBlue;
    colorMap["Firebrick"] = Color::Firebrick;
    colorMap["FloralWhite"] = Color::FloralWhite;
    colorMap["ForestGreen"] = Color::ForestGreen;
    colorMap["Fuchsia"] = Color::Fuchsia;
    colorMap["Gainsboro"] = Color::Gainsboro;
    colorMap["GhostWhite"] = Color::GhostWhite;
    colorMap["Gold"] = Color::Gold;
    colorMap["Goldenrod"] = Color::Goldenrod;
    colorMap["Gray"] = Color::Gray;
    colorMap["Green"] = Color::Green;
    colorMap["GreenYellow"] = Color::GreenYellow;
    colorMap["Honeydew"] = Color::Honeydew;
    colorMap["HotPink"] = Color::HotPink;
    colorMap["IndianRed"] = Color::IndianRed;
    colorMap["Indigo"] = Color::Indigo;
    colorMap["Ivory"] = Color::Ivory;
    colorMap["Khaki"] = Color::Khaki;
    colorMap["Lavender"] = Color::Lavender;
    colorMap["LavenderBlush"] = Color::LavenderBlush;
    colorMap["LawnGreen"] = Color::LawnGreen;
    colorMap["LemonChiffon"] = Color::LemonChiffon;
    colorMap["LightBlue"] = Color::LightBlue;
    colorMap["LightCoral"] = Color::LightCoral;
    colorMap["LightCyan"] = Color::LightCyan;
    colorMap["LightGoldenrodYellow"] = Color::LightGoldenrodYellow;
    colorMap["LightGray"] = Color::LightGray;
    colorMap["LightGreen"] = Color::LightGreen;
    colorMap["LightPink"] = Color::LightPink;
    colorMap["LightSalmon"] = Color::LightSalmon;
    colorMap["LightSeaGreen"] = Color::LightSeaGreen;
    colorMap["LightSkyBlue"] = Color::LightSkyBlue;
    colorMap["LightSlateGray"] = Color::LightSlateGray;
    colorMap["LightSteelBlue"] = Color::LightSteelBlue;
    colorMap["LightYellow"] = Color::LightYellow;
    colorMap["Lime"] = Color::Lime;
    colorMap["LimeGreen"] = Color::LimeGreen;
    colorMap["Linen"] = Color::Linen;
    colorMap["Magenta"] = Color::Magenta;
    colorMap["Maroon"] = Color::Maroon;
    colorMap["MediumAquamarine"] = Color::MediumAquamarine;
    colorMap["MediumBlue"] = Color::MediumBlue;
    colorMap["MediumOrchid"] = Color::MediumOrchid;
    colorMap["MediumPurple"] = Color::MediumPurple;
    colorMap["MediumSeaGreen"] = Color::MediumSeaGreen;
    colorMap["MediumSlateBlue"] = Color::MediumSlateBlue;
    colorMap["MediumSpringGreen"] = Color::MediumSpringGreen;
    colorMap["MediumTurquoise"] = Color::MediumTurquoise;
    colorMap["MediumVioletRed"] = Color::MediumVioletRed;
    colorMap["MidnightBlue"] = Color::MidnightBlue;
    colorMap["MintCream"] = Color::MintCream;
    colorMap["MistyRose"] = Color::MistyRose;
    colorMap["Moccasin"] = Color::Moccasin;
    colorMap["NavajoWhite"] = Color::NavajoWhite;
    colorMap["Navy"] = Color::Navy;
    colorMap["OldLace"] = Color::OldLace;
    colorMap["Olive"] = Color::Olive;
    colorMap["OliveDrab"] = Color::OliveDrab;
    colorMap["Orange"] = Color::Orange;
    colorMap["OrangeRed"] = Color::OrangeRed;
    colorMap["Orchid"] = Color::Orchid;
    colorMap["PaleGoldenrod"] = Color::PaleGoldenrod;
    colorMap["PaleGreen"] = Color::PaleGreen;
    colorMap["PaleTurquoise"] = Color::PaleTurquoise;
    colorMap["PaleVioletRed"] = Color::PaleVioletRed;
    colorMap["PapayaWhip"] = Color::PapayaWhip;
    colorMap["PeachPuff"] = Color::PeachPuff;
    colorMap["Peru"] = Color::Peru;
    colorMap["Pink"] = Color::Pink;
    colorMap["Plum"] = Color::Plum;
    colorMap["PowderBlue"] = Color::PowderBlue;
    colorMap["Purple"] = Color::Purple;
    colorMap["Red"] = Color::Red;
    colorMap["RosyBrown"] = Color::RosyBrown;
    colorMap["RoyalBlue"] = Color::RoyalBlue;
    colorMap["SaddleBrown"] = Color::SaddleBrown;
    colorMap["Salmon"] = Color::Salmon;
    colorMap["SandyBrown"] = Color::SandyBrown;
    colorMap["SeaGreen"] = Color::SeaGreen;
    colorMap["SeaShell"] = Color::SeaShell;
    colorMap["Sienna"] = Color::Sienna;
    colorMap["Silver"] = Color::Silver;
    colorMap["SkyBlue"] = Color::SkyBlue;
    colorMap["SlateBlue"] = Color::SlateBlue;
    colorMap["SlateGray"] = Color::SlateGray;
    colorMap["Snow"] = Color::Snow;
    colorMap["SpringGreen"] = Color::SpringGreen;
    colorMap["SteelBlue"] = Color::SteelBlue;
    colorMap["Tan"] = Color::Tan;
    colorMap["Teal"] = Color::Teal;
    colorMap["Thistle"] = Color::Thistle;
    colorMap["Tomato"] = Color::Tomato;
    colorMap["Turquoise"] = Color::Turquoise;
    colorMap["Violet"] = Color::Violet;
    colorMap["Wheat"] = Color::Wheat;
    colorMap["White"] = Color::White;
    colorMap["WhiteSmoke"] = Color::WhiteSmoke;
    colorMap["Yellow"] = Color::Yellow;
    colorMap["YellowGreen"] = Color::YellowGreen;
    return colorMap;
}

static const std::unordered_map<std::string, Color>& GetColorMap() {
    static auto colorMap = CreateColorMap();
    return colorMap;
}

// Factory methods
Color Color::FromArgb(int argb) {
    return Color(
        static_cast<uint8_t>((argb >> 24) & 0xFF), // Alpha
        static_cast<uint8_t>((argb >> 16) & 0xFF), // Red
        static_cast<uint8_t>((argb >> 8) & 0xFF),  // Green
        static_cast<uint8_t>(argb & 0xFF)          // Blue
    );
}

Color Color::FromArgb(int alpha, int red, int green, int blue) {
    return Color(
        static_cast<uint8_t>(std::clamp(alpha, 0, 255)),
        static_cast<uint8_t>(std::clamp(red, 0, 255)),
        static_cast<uint8_t>(std::clamp(green, 0, 255)),
        static_cast<uint8_t>(std::clamp(blue, 0, 255))
    );
}

Color Color::FromArgb(int alpha, const Color& baseColor) {
    return Color(
        static_cast<uint8_t>(std::clamp(alpha, 0, 255)),
        baseColor.R,
        baseColor.G,
        baseColor.B
    );
}

Color Color::FromName(const std::string& name) {
    const auto& colorMap = GetColorMap();
    auto it = colorMap.find(name);
    if (it != colorMap.end()) {
        return it->second;
    }
    return Color::Black; // Default fallback
}

// Conversion methods
int Color::ToArgb() const {
    return (static_cast<int>(A) << 24) |
           (static_cast<int>(R) << 16) |
           (static_cast<int>(G) << 8) |
           static_cast<int>(B);
}

std::string Color::ToString() const {
    std::ostringstream oss;
    oss << "Color [A=" << static_cast<int>(A) 
        << ", R=" << static_cast<int>(R) 
        << ", G=" << static_cast<int>(G) 
        << ", B=" << static_cast<int>(B) << "]";
    return oss.str();
}

// HSL conversion
Color Color::FromHsl(float h, float s, float l) {
    // Normalize inputs
    h = fmod(h, 360.0f);
    if (h < 0) h += 360.0f;
    s = std::clamp(s, 0.0f, 1.0f);
    l = std::clamp(l, 0.0f, 1.0f);
    
    float c = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - std::abs(fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;
    
    float r, g, b;
    if (h < 60) {
        r = c; g = x; b = 0;
    } else if (h < 120) {
        r = x; g = c; b = 0;
    } else if (h < 180) {
        r = 0; g = c; b = x;
    } else if (h < 240) {
        r = 0; g = x; b = c;
    } else if (h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    return Color(
        static_cast<uint8_t>((r + m) * 255),
        static_cast<uint8_t>((g + m) * 255),
        static_cast<uint8_t>((b + m) * 255)
    );
}

void Color::ToHsl(float& h, float& s, float& l) const {
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;
    
    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    float delta = max - min;
    
    // Lightness
    l = (max + min) / 2.0f;
    
    if (delta == 0) {
        h = 0;
        s = 0;
    } else {
        // Saturation
        s = (l > 0.5f) ? delta / (2.0f - max - min) : delta / (max + min);
        
        // Hue
        if (max == r) {
            h = 60.0f * (fmod((g - b) / delta, 6.0f));
        } else if (max == g) {
            h = 60.0f * ((b - r) / delta + 2.0f);
        } else {
            h = 60.0f * ((r - g) / delta + 4.0f);
        }
        
        if (h < 0) h += 360.0f;
    }
}

// HSV conversion
Color Color::FromHsv(float h, float s, float v) {
    h = fmod(h, 360.0f);
    if (h < 0) h += 360.0f;
    s = std::clamp(s, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);
    
    float c = v * s;
    float x = c * (1.0f - std::abs(fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    
    float r, g, b;
    if (h < 60) {
        r = c; g = x; b = 0;
    } else if (h < 120) {
        r = x; g = c; b = 0;
    } else if (h < 180) {
        r = 0; g = c; b = x;
    } else if (h < 240) {
        r = 0; g = x; b = c;
    } else if (h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    return Color(
        static_cast<uint8_t>((r + m) * 255),
        static_cast<uint8_t>((g + m) * 255),
        static_cast<uint8_t>((b + m) * 255)
    );
}

void Color::ToHsv(float& h, float& s, float& v) const {
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;
    
    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    float delta = max - min;
    
    // Value
    v = max;
    
    if (max == 0) {
        s = 0;
        h = 0;
    } else {
        // Saturation
        s = delta / max;
        
        // Hue
        if (delta == 0) {
            h = 0;
        } else if (max == r) {
            h = 60.0f * (fmod((g - b) / delta, 6.0f));
        } else if (max == g) {
            h = 60.0f * ((b - r) / delta + 2.0f);
        } else {
            h = 60.0f * ((r - g) / delta + 4.0f);
        }
        
        if (h < 0) h += 360.0f;
    }
}

// Properties
uint8_t Color::GetBrightness() const {
    return static_cast<uint8_t>((R * 0.299f + G * 0.587f + B * 0.114f));
}

float Color::GetHue() const {
    float h, s, l;
    ToHsl(h, s, l);
    return h;
}

float Color::GetSaturation() const {
    float h, s, l;
    ToHsl(h, s, l);
    return s;
}

bool Color::IsEmpty() const {
    return A == 0 && R == 0 && G == 0 && B == 0;
}

bool Color::IsKnownColor() const {
    const auto& colorMap = GetColorMap();
    for (const auto& pair : colorMap) {
        if (pair.second == *this) {
            return true;
        }
    }
    return false;
}

bool Color::IsNamedColor() const {
    return IsKnownColor();
}

bool Color::IsSystemColor() const {
    // System colors would be implementation-specific
    return false;
}

std::string Color::GetName() const {
    const auto& colorMap = GetColorMap();
    for (const auto& pair : colorMap) {
        if (pair.second == *this) {
            return pair.first;
        }
    }
    
    // Return hex representation if not a named color
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    if (A != 255) {
        oss << std::setw(2) << static_cast<int>(A);
    }
    oss << std::setw(2) << static_cast<int>(R)
        << std::setw(2) << static_cast<int>(G)
        << std::setw(2) << static_cast<int>(B);
    return oss.str();
}

// Operators
bool Color::operator==(const Color& other) const {
    return A == other.A && R == other.R && G == other.G && B == other.B;
}

bool Color::operator!=(const Color& other) const {
    return !(*this == other);
}

// Skia integration
SkColor Color::ToSkColor() const {
    return SkColorSetARGB(A, R, G, B);
}

Color Color::FromSkColor(SkColor skColor) {
    return Color(
        SkColorGetA(skColor),
        SkColorGetR(skColor),
        SkColorGetG(skColor),
        SkColorGetB(skColor)
    );
}

}
}
