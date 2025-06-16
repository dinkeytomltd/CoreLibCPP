
#pragma once

#include "../Brush.h"
#include "../Point.h"
#include "../Rectangle.h"
#include "../Color.h"
#include "Matrix.h"
#include <vector>

// Forward declarations for Skia
class SkShader;

namespace System {
namespace Drawing {
namespace Drawing2D {

enum class LinearGradientMode {
    Horizontal = 0,
    Vertical = 1,
    ForwardDiagonal = 2,
    BackwardDiagonal = 3
};

enum class WrapMode {
    Tile = 0,
    TileFlipX = 1,
    TileFlipY = 2,
    TileFlipXY = 3,
    Clamp = 4
};

struct ColorBlend {
    std::vector<Color> Colors;
    std::vector<float> Positions;
    
    ColorBlend() = default;
    ColorBlend(const std::vector<Color>& colors, const std::vector<float>& positions)
        : Colors(colors), Positions(positions) {}
};

struct Blend {
    std::vector<float> Factors;
    std::vector<float> Positions;
    
    Blend() = default;
    Blend(const std::vector<float>& factors, const std::vector<float>& positions)
        : Factors(factors), Positions(positions) {}
};

class LinearGradientBrush : public Brush {
private:
    PointF _point1, _point2;
    Color _color1, _color2;
    LinearGradientMode _linearGradientMode;
    WrapMode _wrapMode;
    bool _gammaCorrection;
    Matrix _transform;
    ColorBlend _interpolationColors;
    Blend _blend;
    
public:
    // Constructors
    LinearGradientBrush(const Point& point1, const Point& point2, const Color& color1, const Color& color2);
    LinearGradientBrush(const PointF& point1, const PointF& point2, const Color& color1, const Color& color2);
    LinearGradientBrush(const Rectangle& rect, const Color& color1, const Color& color2, LinearGradientMode linearGradientMode);
    LinearGradientBrush(const RectangleF& rect, const Color& color1, const Color& color2, LinearGradientMode linearGradientMode);
    LinearGradientBrush(const Rectangle& rect, const Color& color1, const Color& color2, float angle);
    LinearGradientBrush(const RectangleF& rect, const Color& color1, const Color& color2, float angle);
    LinearGradientBrush(const Rectangle& rect, const Color& color1, const Color& color2, float angle, bool isAngleScaleable);
    LinearGradientBrush(const RectangleF& rect, const Color& color1, const Color& color2, float angle, bool isAngleScaleable);
    
    // Copy constructor and assignment
    LinearGradientBrush(const LinearGradientBrush& other);
    LinearGradientBrush& operator=(const LinearGradientBrush& other);
    
    // Move constructor and assignment
    LinearGradientBrush(LinearGradientBrush&& other) noexcept;
    LinearGradientBrush& operator=(LinearGradientBrush&& other) noexcept;
    
    // Destructor
    virtual ~LinearGradientBrush() = default;
    
    // Properties
    std::vector<Color> LinearColors() const;
    void SetLinearColors(const Color& color1, const Color& color2);
    
    RectangleF Rectangle() const;
    
    bool GammaCorrection() const { return _gammaCorrection; }
    void SetGammaCorrection(bool value) { _gammaCorrection = value; }
    
    ColorBlend InterpolationColors() const { return _interpolationColors; }
    void SetInterpolationColors(const ColorBlend& value) { _interpolationColors = value; }
    
    Blend GetBlend() const { return _blend; }
    void SetBlend(const Blend& value) { _blend = value; }
    
    Matrix Transform() const { return _transform; }
    void SetTransform(const Matrix& value) { _transform = value; }
    
    WrapMode GetWrapMode() const { return _wrapMode; }
    void SetWrapMode(WrapMode value) { _wrapMode = value; }
    
    // Transformation methods
    void MultiplyTransform(const Matrix& matrix);
    void MultiplyTransform(const Matrix& matrix, MatrixOrder order);
    void ResetTransform();
    void RotateTransform(float angle);
    void RotateTransform(float angle, MatrixOrder order);
    void ScaleTransform(float sx, float sy);
    void ScaleTransform(float sx, float sy, MatrixOrder order);
    void TranslateTransform(float dx, float dy);
    void TranslateTransform(float dx, float dy, MatrixOrder order);
    
    // Brush interface
    std::unique_ptr<Brush> Clone() const override;
    void ApplyToSkPaint(SkPaint& paint) const override;
    
private:
    sk_sp<SkShader> CreateSkShader() const;
    void CalculatePointsFromRect(const RectangleF& rect, LinearGradientMode mode);
    void CalculatePointsFromAngle(const RectangleF& rect, float angle, bool isAngleScaleable);
    void CopyFrom(const LinearGradientBrush& other);
};

}
}
}
