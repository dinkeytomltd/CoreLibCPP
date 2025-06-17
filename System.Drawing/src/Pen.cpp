
#include "System/Drawing/Pen.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkPaint.h"
#include "include/core/SkPathEffect.h"
#include "include/effects/SkDashPathEffect.h"

namespace System {
namespace Drawing {

Pen::Pen(const Color& color, float width)
    : _brush(std::make_unique<SolidBrush>(color))
    , _width(width)
    , _dashStyle(DashStyle::Solid)
    , _dashOffset(0.0f)
    , _startCap(LineCap::Flat)
    , _endCap(LineCap::Flat)
    , _lineJoin(LineJoin::Miter)
    , _miterLimit(10.0f)
    , _alignment(PenAlignment::Center) {
}

Pen::Pen(std::unique_ptr<Brush> brush, float width)
    : _brush(std::move(brush))
    , _width(width)
    , _dashStyle(DashStyle::Solid)
    , _dashOffset(0.0f)
    , _startCap(LineCap::Flat)
    , _endCap(LineCap::Flat)
    , _lineJoin(LineJoin::Miter)
    , _miterLimit(10.0f)
    , _alignment(PenAlignment::Center) {
}

std::unique_ptr<Pen> Pen::Clone() const {
    auto clonedPen = std::make_unique<Pen>(_brush->Clone(), _width);
    clonedPen->_dashStyle = _dashStyle;
    clonedPen->_dashPattern = _dashPattern;
    clonedPen->_dashOffset = _dashOffset;
    clonedPen->_startCap = _startCap;
    clonedPen->_endCap = _endCap;
    clonedPen->_lineJoin = _lineJoin;
    clonedPen->_miterLimit = _miterLimit;
    clonedPen->_alignment = _alignment;
    return clonedPen;
}

Color Pen::GetColor() const {
    auto solidBrush = dynamic_cast<const SolidBrush*>(_brush.get());
    if (solidBrush) {
        return solidBrush->GetColor();
    }
    return Color::Black;
}

void Pen::SetColor(const Color& color) {
    _brush = std::make_unique<SolidBrush>(color);
}

void Pen::SetDashStyle(DashStyle style) {
    _dashStyle = style;
    if (style != DashStyle::Custom) {
        SetDefaultDashPattern();
    }
}

void Pen::SetDashPattern(const std::vector<float>& pattern) {
    _dashPattern = pattern;
    _dashStyle = DashStyle::Custom;
}

void Pen::ApplyToSkPaint(SkPaint& paint) const {
    // Apply brush properties
    _brush->ApplyToSkPaint(paint);
    
    // Set stroke properties
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(_width);
    
    // Set line cap
    switch (_startCap) {
        case LineCap::Flat:
            paint.setStrokeCap(SkPaint::kButt_Cap);
            break;
        case LineCap::Round:
            paint.setStrokeCap(SkPaint::kRound_Cap);
            break;
        case LineCap::Square:
            paint.setStrokeCap(SkPaint::kSquare_Cap);
            break;
        default:
            paint.setStrokeCap(SkPaint::kButt_Cap);
            break;
    }
    
    // Set line join
    switch (_lineJoin) {
        case LineJoin::Miter:
            paint.setStrokeJoin(SkPaint::kMiter_Join);
            break;
        case LineJoin::Bevel:
            paint.setStrokeJoin(SkPaint::kBevel_Join);
            break;
        case LineJoin::Round:
            paint.setStrokeJoin(SkPaint::kRound_Join);
            break;
        case LineJoin::MiterClipped:
            paint.setStrokeJoin(SkPaint::kMiter_Join);
            break;
    }
    
    paint.setStrokeMiter(_miterLimit);
    
    // Set dash pattern
    if (_dashStyle != DashStyle::Solid && !_dashPattern.empty()) {
        std::vector<SkScalar> intervals;
        for (float interval : _dashPattern) {
            intervals.push_back(interval * _width);
        }
        
        sk_sp<SkPathEffect> dashEffect = SkDashPathEffect::Make(
            intervals.data(), static_cast<int>(intervals.size()), _dashOffset);
        paint.setPathEffect(dashEffect);
    }
}

void Pen::SetDefaultDashPattern() {
    switch (_dashStyle) {
        case DashStyle::Solid:
            _dashPattern.clear();
            break;
        case DashStyle::Dash:
            _dashPattern = {3.0f, 1.0f};
            break;
        case DashStyle::Dot:
            _dashPattern = {1.0f, 1.0f};
            break;
        case DashStyle::DashDot:
            _dashPattern = {3.0f, 1.0f, 1.0f, 1.0f};
            break;
        case DashStyle::DashDotDot:
            _dashPattern = {3.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
            break;
        case DashStyle::Custom:
            // Keep existing pattern
            break;
    }
}

}
}
