
#include "System/Drawing/Drawing2D/LinearGradientBrush.h"
#include <cmath>
#include <stdexcept>

// Skia includes
#include "include/core/SkPaint.h"
#include "include/effects/SkGradientShader.h"

namespace System {
namespace Drawing {
namespace Drawing2D {

LinearGradientBrush::LinearGradientBrush(const Point& point1, const Point& point2, const Color& color1, const Color& color2)
    : LinearGradientBrush(PointF(static_cast<float>(point1.X), static_cast<float>(point1.Y)),
                         PointF(static_cast<float>(point2.X), static_cast<float>(point2.Y)),
                         color1, color2) {
}

LinearGradientBrush::LinearGradientBrush(const PointF& point1, const PointF& point2, const Color& color1, const Color& color2)
    : _point1(point1), _point2(point2), _color1(color1), _color2(color2)
    , _linearGradientMode(LinearGradientMode::Horizontal)
    , _wrapMode(WrapMode::Tile)
    , _gammaCorrection(false) {
}

LinearGradientBrush::LinearGradientBrush(const Rectangle& rect, const Color& color1, const Color& color2, LinearGradientMode linearGradientMode)
    : LinearGradientBrush(RectangleF(rect), color1, color2, linearGradientMode) {
}

LinearGradientBrush::LinearGradientBrush(const RectangleF& rect, const Color& color1, const Color& color2, LinearGradientMode linearGradientMode)
    : _color1(color1), _color2(color2)
    , _linearGradientMode(linearGradientMode)
    , _wrapMode(WrapMode::Tile)
    , _gammaCorrection(false) {
    
    CalculatePointsFromRect(rect, linearGradientMode);
}

LinearGradientBrush::LinearGradientBrush(const Rectangle& rect, const Color& color1, const Color& color2, float angle)
    : LinearGradientBrush(RectangleF(rect), color1, color2, angle, false) {
}

LinearGradientBrush::LinearGradientBrush(const RectangleF& rect, const Color& color1, const Color& color2, float angle)
    : LinearGradientBrush(rect, color1, color2, angle, false) {
}

LinearGradientBrush::LinearGradientBrush(const Rectangle& rect, const Color& color1, const Color& color2, float angle, bool isAngleScaleable)
    : LinearGradientBrush(RectangleF(rect), color1, color2, angle, isAngleScaleable) {
}

LinearGradientBrush::LinearGradientBrush(const RectangleF& rect, const Color& color1, const Color& color2, float angle, bool isAngleScaleable)
    : _color1(color1), _color2(color2)
    , _linearGradientMode(LinearGradientMode::Horizontal)
    , _wrapMode(WrapMode::Tile)
    , _gammaCorrection(false) {
    
    CalculatePointsFromAngle(rect, angle, isAngleScaleable);
}

LinearGradientBrush::LinearGradientBrush(const LinearGradientBrush& other) {
    CopyFrom(other);
}

LinearGradientBrush& LinearGradientBrush::operator=(const LinearGradientBrush& other) {
    if (this != &other) {
        CopyFrom(other);
    }
    return *this;
}

LinearGradientBrush::LinearGradientBrush(LinearGradientBrush&& other) noexcept
    : _point1(other._point1), _point2(other._point2)
    , _color1(other._color1), _color2(other._color2)
    , _linearGradientMode(other._linearGradientMode)
    , _wrapMode(other._wrapMode)
    , _gammaCorrection(other._gammaCorrection)
    , _transform(std::move(other._transform))
    , _interpolationColors(std::move(other._interpolationColors))
    , _blend(std::move(other._blend)) {
}

LinearGradientBrush& LinearGradientBrush::operator=(LinearGradientBrush&& other) noexcept {
    if (this != &other) {
        _point1 = other._point1;
        _point2 = other._point2;
        _color1 = other._color1;
        _color2 = other._color2;
        _linearGradientMode = other._linearGradientMode;
        _wrapMode = other._wrapMode;
        _gammaCorrection = other._gammaCorrection;
        _transform = std::move(other._transform);
        _interpolationColors = std::move(other._interpolationColors);
        _blend = std::move(other._blend);
    }
    return *this;
}

std::vector<Color> LinearGradientBrush::LinearColors() const {
    return {_color1, _color2};
}

void LinearGradientBrush::SetLinearColors(const Color& color1, const Color& color2) {
    _color1 = color1;
    _color2 = color2;
}

RectangleF LinearGradientBrush::Rectangle() const {
    float minX = std::min(_point1.X, _point2.X);
    float minY = std::min(_point1.Y, _point2.Y);
    float maxX = std::max(_point1.X, _point2.X);
    float maxY = std::max(_point1.Y, _point2.Y);
    
    return RectangleF(minX, minY, maxX - minX, maxY - minY);
}

void LinearGradientBrush::MultiplyTransform(const Matrix& matrix) {
    MultiplyTransform(matrix, MatrixOrder::Prepend);
}

void LinearGradientBrush::MultiplyTransform(const Matrix& matrix, MatrixOrder order) {
    _transform.Multiply(matrix, order);
}

void LinearGradientBrush::ResetTransform() {
    _transform.Reset();
}

void LinearGradientBrush::RotateTransform(float angle) {
    RotateTransform(angle, MatrixOrder::Prepend);
}

void LinearGradientBrush::RotateTransform(float angle, MatrixOrder order) {
    _transform.Rotate(angle, order);
}

void LinearGradientBrush::ScaleTransform(float sx, float sy) {
    ScaleTransform(sx, sy, MatrixOrder::Prepend);
}

void LinearGradientBrush::ScaleTransform(float sx, float sy, MatrixOrder order) {
    _transform.Scale(sx, sy, order);
}

void LinearGradientBrush::TranslateTransform(float dx, float dy) {
    TranslateTransform(dx, dy, MatrixOrder::Prepend);
}

void LinearGradientBrush::TranslateTransform(float dx, float dy, MatrixOrder order) {
    _transform.Translate(dx, dy, order);
}

std::unique_ptr<Brush> LinearGradientBrush::Clone() const {
    return std::make_unique<LinearGradientBrush>(*this);
}

void LinearGradientBrush::ApplyToSkPaint(SkPaint& paint) const {
    sk_sp<SkShader> shader = CreateSkShader();
    paint.setShader(shader);
}

sk_sp<SkShader> LinearGradientBrush::CreateSkShader() const {
    SkPoint points[2] = {
        SkPoint::Make(_point1.X, _point1.Y),
        SkPoint::Make(_point2.X, _point2.Y)
    };
    
    std::vector<SkColor> colors;
    std::vector<SkScalar> positions;
    
    if (!_interpolationColors.Colors.empty() && !_interpolationColors.Positions.empty()) {
        // Use interpolation colors if available
        for (const auto& color : _interpolationColors.Colors) {
            colors.push_back(color.ToSkColor());
        }
        for (float pos : _interpolationColors.Positions) {
            positions.push_back(pos);
        }
    } else {
        // Use simple two-color gradient
        colors.push_back(_color1.ToSkColor());
        colors.push_back(_color2.ToSkColor());
        positions.push_back(0.0f);
        positions.push_back(1.0f);
    }
    
    // Convert wrap mode to Skia tile mode
    SkTileMode tileMode;
    switch (_wrapMode) {
        case WrapMode::Tile:
            tileMode = SkTileMode::kRepeat;
            break;
        case WrapMode::TileFlipX:
        case WrapMode::TileFlipY:
        case WrapMode::TileFlipXY:
            tileMode = SkTileMode::kMirror;
            break;
        case WrapMode::Clamp:
            tileMode = SkTileMode::kClamp;
            break;
        default:
            tileMode = SkTileMode::kClamp;
            break;
    }
    
    sk_sp<SkShader> shader = SkGradientShader::MakeLinear(
        points, colors.data(), positions.data(), static_cast<int>(colors.size()), tileMode);
    
    // Apply transformation if not identity
    if (!_transform.IsIdentity()) {
        SkMatrix skMatrix = _transform.ToSkMatrix();
        shader = shader->makeWithLocalMatrix(skMatrix);
    }
    
    return shader;
}

void LinearGradientBrush::CalculatePointsFromRect(const RectangleF& rect, LinearGradientMode mode) {
    switch (mode) {
        case LinearGradientMode::Horizontal:
            _point1 = PointF(rect.X, rect.Y + rect.Height / 2);
            _point2 = PointF(rect.X + rect.Width, rect.Y + rect.Height / 2);
            break;
        case LinearGradientMode::Vertical:
            _point1 = PointF(rect.X + rect.Width / 2, rect.Y);
            _point2 = PointF(rect.X + rect.Width / 2, rect.Y + rect.Height);
            break;
        case LinearGradientMode::ForwardDiagonal:
            _point1 = PointF(rect.X, rect.Y);
            _point2 = PointF(rect.X + rect.Width, rect.Y + rect.Height);
            break;
        case LinearGradientMode::BackwardDiagonal:
            _point1 = PointF(rect.X + rect.Width, rect.Y);
            _point2 = PointF(rect.X, rect.Y + rect.Height);
            break;
    }
}

void LinearGradientBrush::CalculatePointsFromAngle(const RectangleF& rect, float angle, bool isAngleScaleable) {
    float radians = angle * static_cast<float>(M_PI) / 180.0f;
    float centerX = rect.X + rect.Width / 2;
    float centerY = rect.Y + rect.Height / 2;
    
    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);
    
    float halfWidth = rect.Width / 2;
    float halfHeight = rect.Height / 2;
    
    if (isAngleScaleable) {
        // Scale the gradient line to fit the rectangle
        float scale = std::max(std::abs(cosAngle * halfWidth), std::abs(sinAngle * halfHeight));
        _point1 = PointF(centerX - cosAngle * scale, centerY - sinAngle * scale);
        _point2 = PointF(centerX + cosAngle * scale, centerY + sinAngle * scale);
    } else {
        // Use fixed-length gradient line
        float length = std::sqrt(halfWidth * halfWidth + halfHeight * halfHeight);
        _point1 = PointF(centerX - cosAngle * length, centerY - sinAngle * length);
        _point2 = PointF(centerX + cosAngle * length, centerY + sinAngle * length);
    }
}

void LinearGradientBrush::CopyFrom(const LinearGradientBrush& other) {
    _point1 = other._point1;
    _point2 = other._point2;
    _color1 = other._color1;
    _color2 = other._color2;
    _linearGradientMode = other._linearGradientMode;
    _wrapMode = other._wrapMode;
    _gammaCorrection = other._gammaCorrection;
    _transform = other._transform;
    _interpolationColors = other._interpolationColors;
    _blend = other._blend;
}

}
}
}
