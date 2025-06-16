
#pragma once

#include "../Point.h"
#include "../Rectangle.h"
#include <vector>
#include <memory>

// Forward declarations for Skia
class SkPath;

namespace System {
namespace Drawing {

// Forward declarations
namespace Drawing2D {
    class Matrix;
}

enum class FillMode {
    Alternate = 0,
    Winding = 1
};

namespace Drawing2D {

class GraphicsPath {
private:
    std::unique_ptr<SkPath> _skPath;
    FillMode _fillMode;
    
public:
    // Constructors
    GraphicsPath();
    explicit GraphicsPath(FillMode fillMode);
    GraphicsPath(const std::vector<Point>& pts, const std::vector<uint8_t>& types);
    GraphicsPath(const std::vector<PointF>& pts, const std::vector<uint8_t>& types);
    GraphicsPath(const std::vector<Point>& pts, const std::vector<uint8_t>& types, FillMode fillMode);
    GraphicsPath(const std::vector<PointF>& pts, const std::vector<uint8_t>& types, FillMode fillMode);
    
    // Copy constructor and assignment
    GraphicsPath(const GraphicsPath& other);
    GraphicsPath& operator=(const GraphicsPath& other);
    
    // Move constructor and assignment
    GraphicsPath(GraphicsPath&& other) noexcept;
    GraphicsPath& operator=(GraphicsPath&& other) noexcept;
    
    // Destructor
    ~GraphicsPath() = default;
    
    // Properties
    FillMode GetFillMode() const { return _fillMode; }
    void SetFillMode(FillMode fillMode);
    
    std::vector<PointF> PathPoints() const;
    std::vector<uint8_t> PathTypes() const;
    int PointCount() const;
    
    // Path building methods
    void AddLine(int x1, int y1, int x2, int y2);
    void AddLine(float x1, float y1, float x2, float y2);
    void AddLine(const Point& pt1, const Point& pt2);
    void AddLine(const PointF& pt1, const PointF& pt2);
    void AddLines(const std::vector<Point>& points);
    void AddLines(const std::vector<PointF>& points);
    
    void AddRectangle(const Rectangle& rect);
    void AddRectangle(const RectangleF& rect);
    void AddRectangles(const std::vector<Rectangle>& rects);
    void AddRectangles(const std::vector<RectangleF>& rects);
    
    void AddEllipse(int x, int y, int width, int height);
    void AddEllipse(float x, float y, float width, float height);
    void AddEllipse(const Rectangle& rect);
    void AddEllipse(const RectangleF& rect);
    
    void AddArc(int x, int y, int width, int height, float startAngle, float sweepAngle);
    void AddArc(float x, float y, float width, float height, float startAngle, float sweepAngle);
    void AddArc(const Rectangle& rect, float startAngle, float sweepAngle);
    void AddArc(const RectangleF& rect, float startAngle, float sweepAngle);
    
    void AddPie(int x, int y, int width, int height, float startAngle, float sweepAngle);
    void AddPie(float x, float y, float width, float height, float startAngle, float sweepAngle);
    void AddPie(const Rectangle& rect, float startAngle, float sweepAngle);
    void AddPie(const RectangleF& rect, float startAngle, float sweepAngle);
    
    void AddPolygon(const std::vector<Point>& points);
    void AddPolygon(const std::vector<PointF>& points);
    
    void AddBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    void AddBezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    void AddBezier(const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4);
    void AddBezier(const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4);
    void AddBeziers(const std::vector<Point>& points);
    void AddBeziers(const std::vector<PointF>& points);
    
    void AddCurve(const std::vector<Point>& points);
    void AddCurve(const std::vector<PointF>& points);
    void AddCurve(const std::vector<Point>& points, float tension);
    void AddCurve(const std::vector<PointF>& points, float tension);
    void AddCurve(const std::vector<Point>& points, int offset, int numberOfSegments, float tension);
    void AddCurve(const std::vector<PointF>& points, int offset, int numberOfSegments, float tension);
    
    void AddClosedCurve(const std::vector<Point>& points);
    void AddClosedCurve(const std::vector<PointF>& points);
    void AddClosedCurve(const std::vector<Point>& points, float tension);
    void AddClosedCurve(const std::vector<PointF>& points, float tension);
    
    void AddPath(const GraphicsPath& addingPath, bool connect);
    
    void AddString(const std::string& s, const FontFamily& family, int style, float emSize, const Point& origin);
    void AddString(const std::string& s, const FontFamily& family, int style, float emSize, const PointF& origin);
    void AddString(const std::string& s, const FontFamily& family, int style, float emSize, const Rectangle& layoutRect);
    void AddString(const std::string& s, const FontFamily& family, int style, float emSize, const RectangleF& layoutRect);
    
    // Path manipulation
    void CloseFigure();
    void CloseAllFigures();
    void StartFigure();
    void Reset();
    void Reverse();
    
    // Geometric operations
    RectangleF GetBounds() const;
    RectangleF GetBounds(const Matrix& matrix) const;
    
    bool IsVisible(int x, int y) const;
    bool IsVisible(float x, float y) const;
    bool IsVisible(const Point& point) const;
    bool IsVisible(const PointF& point) const;
    bool IsVisible(int x, int y, const Graphics& graphics) const;
    bool IsVisible(float x, float y, const Graphics& graphics) const;
    bool IsVisible(const Point& point, const Graphics& graphics) const;
    bool IsVisible(const PointF& point, const Graphics& graphics) const;
    
    bool IsOutlineVisible(int x, int y, const Pen& pen) const;
    bool IsOutlineVisible(float x, float y, const Pen& pen) const;
    bool IsOutlineVisible(const Point& point, const Pen& pen) const;
    bool IsOutlineVisible(const PointF& point, const Pen& pen) const;
    bool IsOutlineVisible(int x, int y, const Pen& pen, const Graphics& graphics) const;
    bool IsOutlineVisible(float x, float y, const Pen& pen, const Graphics& graphics) const;
    bool IsOutlineVisible(const Point& point, const Pen& pen, const Graphics& graphics) const;
    bool IsOutlineVisible(const PointF& point, const Pen& pen, const Graphics& graphics) const;
    
    // Transformations
    void Transform(const Matrix& matrix);
    
    // Flattening
    void Flatten();
    void Flatten(const Matrix& matrix);
    void Flatten(const Matrix& matrix, float flatness);
    
    // Widening
    void Widen(const Pen& pen);
    void Widen(const Pen& pen, const Matrix& matrix);
    void Widen(const Pen& pen, const Matrix& matrix, float flatness);
    
    // Warping
    void Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect);
    void Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect, const Matrix& matrix);
    void Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect, const Matrix& matrix, int warpMode);
    void Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect, const Matrix& matrix, int warpMode, float flatness);
    
    // Cloning
    std::unique_ptr<GraphicsPath> Clone() const;
    
    // Skia integration
    SkPath* GetSkPath() const { return _skPath.get(); }
    void SetSkPath(std::unique_ptr<SkPath> skPath) { _skPath = std::move(skPath); }
    
private:
    void InitializeSkPath();
    void UpdateSkPathFillType();
};

}
}
}
