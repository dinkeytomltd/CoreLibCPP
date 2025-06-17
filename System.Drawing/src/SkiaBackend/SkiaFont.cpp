
#include "System/Drawing/Font.h"
#include "System/Drawing/FontFamily.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkFont.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkFontMgr.h"

namespace System {
namespace Drawing {

// Helper functions for Skia font operations
sk_sp<SkTypeface> CreateSkTypefaceFromName(const std::string& familyName, bool bold, bool italic) {
    auto fontMgr = SkFontMgr::RefDefault();
    
    SkFontStyle::Weight weight = bold ? SkFontStyle::kBold_Weight : SkFontStyle::kNormal_Weight;
    SkFontStyle::Slant slant = italic ? SkFontStyle::kItalic_Slant : SkFontStyle::kUpright_Slant;
    SkFontStyle style(weight, SkFontStyle::kNormal_Width, slant);
    
    sk_sp<SkTypeface> typeface = fontMgr->matchFamilyStyle(familyName.c_str(), style);
    
    if (!typeface) {
        // Fallback to default typeface
        typeface = SkTypeface::MakeDefault();
    }
    
    return typeface;
}

SkFont CreateSkFontFromTypeface(const sk_sp<SkTypeface>& typeface, float size) {
    SkFont font(typeface, size);
    font.setEdging(SkFont::Edging::kAntiAlias);
    font.setHinting(SkFontHinting::kNormal);
    return font;
}

float MeasureTextWidth(const SkFont& font, const std::string& text) {
    SkRect bounds;
    font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
    return bounds.width();
}

float GetFontHeight(const SkFont& font) {
    SkFontMetrics metrics;
    font.getMetrics(&metrics);
    return metrics.fDescent - metrics.fAscent;
}

}
}
