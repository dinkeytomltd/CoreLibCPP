
#pragma once

#include "Color.h"
#include "Brush.h"
#include <memory>
#include <vector>

// Forward declarations for Skia
class SkPaint;

namespace System {
namespace Drawing {

// Enumerations
enum class DashStyle {
    Solid = 0,
    Dash = 1,
    Dot = 2,
    DashDot = 3,
    DashDotDot = 4,
    Custom = 5
};

enum class LineCap {
    Flat = 0,
    Square = 1,
    Round = 2,
    Triangle = 3,
    NoAnchor = 16,
    SquareAnchor = 17,
    RoundAnchor = 18,
    DiamondAnchor = 19,
    ArrowAnchor = 20,
    AnchorMask = 240,
    Custom = 255
};

enum class LineJoin {
    Miter = 0,
    Bevel = 1,
    Round = 2,
    MiterClipped = 3
};

enum class PenAlignment {
    Center = 0,
    Inset = 1,
    Outset = 2,
    Left = 3,
    Right = 4
};

class Pen {
private:
    std::unique_ptr<Brush> _brush;
    float _width;
    DashStyle _dashStyle;
    std::vector<float> _dashPattern;
    float _dashOffset;
    LineCap _startCap;
    LineCap _endCap;
    LineJoin _lineJoin;
    float _miterLimit;
    PenAlignment _alignment;
    
public:
    // Constructors
    explicit Pen(const Color& color, float width = 1.0f);
    Pen(std::unique_ptr<Brush> brush, float width = 1.0f);
    
    // Destructor
    ~Pen() = default;
    
    // Non-copyable but movable
    Pen(const Pen&) = delete;
    Pen& operator=(const Pen&) = delete;
    Pen(Pen&&) = default;
    Pen& operator=(Pen&&) = default;
    
    // Clone method
    std::unique_ptr<Pen> Clone() const;
    
    // Properties
    Color GetColor() const;
    void SetColor(const Color& color);
    
    float GetWidth() const { return _width; }
    void SetWidth(float width) { _width = width; }
    
    DashStyle GetDashStyle() const { return _dashStyle; }
    void SetDashStyle(DashStyle style);
    
    std::vector<float> GetDashPattern() const { return _dashPattern; }
    void SetDashPattern(const std::vector<float>& pattern);
    
    float GetDashOffset() const { return _dashOffset; }
    void SetDashOffset(float offset) { _dashOffset = offset; }
    
    LineCap GetStartCap() const { return _startCap; }
    void SetStartCap(LineCap cap) { _startCap = cap; }
    
    LineCap GetEndCap() const { return _endCap; }
    void SetEndCap(LineCap cap) { _endCap = cap; }
    
    LineJoin GetLineJoin() const { return _lineJoin; }
    void SetLineJoin(LineJoin join) { _lineJoin = join; }
    
    float GetMiterLimit() const { return _miterLimit; }
    void SetMiterLimit(float limit) { _miterLimit = limit; }
    
    PenAlignment GetAlignment() const { return _alignment; }
    void SetAlignment(PenAlignment alignment) { _alignment = alignment; }
    
    Brush* GetBrush() const { return _brush.get(); }
    void SetBrush(std::unique_ptr<Brush> brush) { _brush = std::move(brush); }
    
    // Skia integration
    void ApplyToSkPaint(SkPaint& paint) const;
    
private:
    void SetDefaultDashPattern();
};

}
}
