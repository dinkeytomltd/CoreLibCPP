
#include "System/Drawing/Brush.h"
#include "System/Drawing/Image.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkPaint.h"
#include "include/core/SkShader.h"
#include "include/effects/SkGradientShader.h"

namespace System {
namespace Drawing {

// SolidBrush implementation
SolidBrush::SolidBrush(const Color& color) : _color(color) {
}

std::unique_ptr<Brush> SolidBrush::Clone() const {
    return std::make_unique<SolidBrush>(_color);
}

void SolidBrush::ApplyToSkPaint(SkPaint& paint) const {
    paint.setColor(_color.ToSkColor());
}

// TextureBrush implementation
TextureBrush::TextureBrush(std::shared_ptr<Image> image) 
    : _image(image), _dstRect(0, 0, static_cast<float>(image->Width()), static_cast<float>(image->Height())) {
}

TextureBrush::TextureBrush(std::shared_ptr<Image> image, const RectangleF& dstRect)
    : _image(image), _dstRect(dstRect) {
}

std::unique_ptr<Brush> TextureBrush::Clone() const {
    return std::make_unique<TextureBrush>(_image, _dstRect);
}

void TextureBrush::ApplyToSkPaint(SkPaint& paint) const {
    if (!_image || !_image->GetSkImage()) {
        paint.setColor(SK_ColorBLACK);
        return;
    }
    
    // Create a shader from the image
    sk_sp<SkShader> shader = _image->GetSkImage()->makeShader(
        SkTileMode::kRepeat, SkTileMode::kRepeat, SkSamplingOptions());
    
    paint.setShader(shader);
}

// HatchBrush implementation
HatchBrush::HatchBrush(HatchStyle hatchStyle, const Color& foreColor)
    : _hatchStyle(hatchStyle), _foreColor(foreColor), _backColor(Color::Transparent) {
}

HatchBrush::HatchBrush(HatchStyle hatchStyle, const Color& foreColor, const Color& backColor)
    : _hatchStyle(hatchStyle), _foreColor(foreColor), _backColor(backColor) {
}

std::unique_ptr<Brush> HatchBrush::Clone() const {
    return std::make_unique<HatchBrush>(_hatchStyle, _foreColor, _backColor);
}

void HatchBrush::ApplyToSkPaint(SkPaint& paint) const {
    sk_sp<SkShader> shader = CreateHatchShader();
    paint.setShader(shader);
}

sk_sp<SkShader> HatchBrush::CreateHatchShader() const {
    // For now, create a simple pattern based on hatch style
    // This is a simplified implementation - a full implementation would
    // create proper hatch patterns using SkPath or bitmap patterns
    
    SkColor colors[2] = { _backColor.ToSkColor(), _foreColor.ToSkColor() };
    SkScalar positions[2] = { 0.0f, 1.0f };
    
    switch (_hatchStyle) {
        case HatchStyle::Horizontal:
        case HatchStyle::Vertical:
        case HatchStyle::Cross:
            // Create a simple linear gradient as placeholder
            return SkGradientShader::MakeLinear(
                SkPoint::Make(0, 0), SkPoint::Make(8, 8),
                colors, positions, 2, SkTileMode::kRepeat);
        
        default:
            // Fallback to solid color
            return SkShaders::Color(_foreColor.ToSkColor());
    }
}

}
}
