
#include "System/Drawing/FontFamily.h"
#include "System/Drawing/Font.h"
#include "System/Drawing/Graphics.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkTypeface.h"
#include "include/core/SkFontMgr.h"

namespace System {
namespace Drawing {

FontFamily::FontFamily(const std::string& name) : _name(name) {
}

FontFamily::FontFamily(const FontFamily& other) : _name(other._name) {
}

FontFamily& FontFamily::operator=(const FontFamily& other) {
    if (this != &other) {
        _name = other._name;
        _fontManager.reset();
    }
    return *this;
}

FontFamily::FontFamily(FontFamily&& other) noexcept 
    : _name(std::move(other._name)), _fontManager(std::move(other._fontManager)) {
}

FontFamily& FontFamily::operator=(FontFamily&& other) noexcept {
    if (this != &other) {
        _name = std::move(other._name);
        _fontManager = std::move(other._fontManager);
    }
    return *this;
}

bool FontFamily::IsStyleAvailable(FontStyle style) const {
    InitializeFontManager();
    
    // Convert FontStyle to Skia font style
    SkFontStyle skStyle = SkFontStyle::Normal();
    
    if ((static_cast<int>(style) & static_cast<int>(FontStyle::Bold)) != 0) {
        skStyle = SkFontStyle(SkFontStyle::kBold_Weight, SkFontStyle::kNormal_Width, SkFontStyle::kUpright_Slant);
    }
    
    if ((static_cast<int>(style) & static_cast<int>(FontStyle::Italic)) != 0) {
        skStyle = SkFontStyle(skStyle.weight(), skStyle.width(), SkFontStyle::kItalic_Slant);
    }
    
    sk_sp<SkTypeface> typeface = _fontManager->matchFamilyStyle(_name.c_str(), skStyle);
    return typeface != nullptr;
}

int FontFamily::GetEmHeight(FontStyle style) const {
    // TODO: Implement proper em height calculation
    return 2048; // Typical em square size
}

int FontFamily::GetCellAscent(FontStyle style) const {
    // TODO: Implement proper cell ascent calculation
    return 1638; // Approximate ascent for typical fonts
}

int FontFamily::GetCellDescent(FontStyle style) const {
    // TODO: Implement proper cell descent calculation
    return 410; // Approximate descent for typical fonts
}

int FontFamily::GetLineSpacing(FontStyle style) const {
    return GetCellAscent(style) + GetCellDescent(style);
}

std::vector<FontFamily> FontFamily::GetFamilies() {
    auto fontMgr = SkFontMgr::RefDefault();
    std::vector<FontFamily> families;
    
    int count = fontMgr->countFamilies();
    for (int i = 0; i < count; ++i) {
        SkString familyName;
        fontMgr->getFamilyName(i, &familyName);
        families.emplace_back(familyName.c_str());
    }
    
    return families;
}

std::vector<FontFamily> FontFamily::GetFamilies(const Graphics& graphics) {
    // For now, return the same as GetFamilies()
    // In a full implementation, this might consider graphics-specific font rendering
    return GetFamilies();
}

FontFamily FontFamily::GenericSansSerif() {
    return FontFamily("Arial");
}

FontFamily FontFamily::GenericSerif() {
    return FontFamily("Times New Roman");
}

FontFamily FontFamily::GenericMonospace() {
    return FontFamily("Courier New");
}

bool FontFamily::operator==(const FontFamily& other) const {
    return _name == other._name;
}

bool FontFamily::operator!=(const FontFamily& other) const {
    return !(*this == other);
}

std::string FontFamily::ToString() const {
    return "[FontFamily: Name=" + _name + "]";
}

std::shared_ptr<SkTypeface> FontFamily::CreateTypeface(FontStyle style) const {
    InitializeFontManager();
    
    // Convert FontStyle to Skia font style
    SkFontStyle::Weight weight = SkFontStyle::kNormal_Weight;
    SkFontStyle::Slant slant = SkFontStyle::kUpright_Slant;
    
    if ((static_cast<int>(style) & static_cast<int>(FontStyle::Bold)) != 0) {
        weight = SkFontStyle::kBold_Weight;
    }
    
    if ((static_cast<int>(style) & static_cast<int>(FontStyle::Italic)) != 0) {
        slant = SkFontStyle::kItalic_Slant;
    }
    
    SkFontStyle skStyle(weight, SkFontStyle::kNormal_Width, slant);
    sk_sp<SkTypeface> typeface = _fontManager->matchFamilyStyle(_name.c_str(), skStyle);
    
    if (!typeface) {
        // Fallback to default typeface
        typeface = SkTypeface::MakeDefault();
    }
    
    return std::shared_ptr<SkTypeface>(typeface.release());
}

std::shared_ptr<SkFontMgr> FontFamily::GetFontManager() const {
    InitializeFontManager();
    return _fontManager;
}

void FontFamily::InitializeFontManager() const {
    if (!_fontManager) {
        _fontManager = std::shared_ptr<SkFontMgr>(SkFontMgr::RefDefault().release());
    }
}

}
}
