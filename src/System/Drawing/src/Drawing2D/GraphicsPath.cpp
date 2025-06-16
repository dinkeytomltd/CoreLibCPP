
#include "System/Drawing/Drawing2D/GraphicsPath.h"
#include "System/Drawing/Drawing2D/Matrix.h"
#include "System/Drawing/Graphics.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/FontFamily.h"
#include <stdexcept>

// Skia includes
#include "include/core/SkPath.h"

namespace System {
namespace Drawing {
namespace Drawing2D {

GraphicsPath::GraphicsPath() : _fillMode(FillMode::Alternate) {
    InitializeSkPath();
}

GraphicsPath::GraphicsPath(FillMode fillMode) : _fillMode(fillMode) {
    InitializeSkPath();
}

GraphicsPath::GraphicsPath(const std::vector<Point>& pts, const std::vector<uint8_t>& types)
    : GraphicsPath(pts, types, FillMode::Alternate) {
}

GraphicsPath::GraphicsPath(const std::vector<PointF>& pts, const std::vector<uint8_t>& types)
    : GraphicsPath(pts, types, FillMode::Alternate) {
}

GraphicsPath::GraphicsPath(const std::vector<Point>& pts, const std::vector<uint8_t>& types, FillMode fillMode)
    : _fillMode(fillMode) {
    InitializeSkPath();
    // TODO: Implement path construction from points and types
}

GraphicsPath::GraphicsPath(const std::vector<PointF>& pts, const std::vector<uint8_t>& types, FillMode fillMode)
    : _fillMode(fillMode) {
    InitializeSkPath();
    // TODO: Implement path construction from points and types
}

GraphicsPath::GraphicsPath(const GraphicsPath& other) : _fillMode(other._fillMode) {
    InitializeSkPath();
    if (other._skPath) {
        *_skPath = *other._skPath;
    }
}

GraphicsPath& GraphicsPath::operator=(const GraphicsPath& other) {
    if (this != &other) {
        _fillMode = other._fillMode;
        if (!_skPath) {
            InitializeSkPath();
        }
        if (other._skPath) {
            *_skPath = *other._skPath;
        }
    }
    return *this;
}

GraphicsPath::GraphicsPath(GraphicsPath&& other) noexcept 
    : _skPath(std::move(other._skPath)), _fillMode(other._fillMode) {
}

GraphicsPath& GraphicsPath::operator=(GraphicsPath&& other) noexcept {
    if (this != &other) {
        _skPath = std::move(other._skPath);
        _fillMode = other._fillMode;
    }
    return *this;
}

void GraphicsPath::SetFillMode(FillMode fillMode) {
    _fillMode = fillMode;
    UpdateSkPathFillType();
}

std::vector<PointF> GraphicsPath::PathPoints() const {
    // TODO: Implement path point extraction from Skia path
    throw std::runtime_error("PathPoints not yet implemented");
}

std::vector<uint8_t> GraphicsPath::PathTypes() const {
    // TODO: Implement path type extraction from Skia path
    throw std::runtime_error("PathTypes not yet implemented");
}

int GraphicsPath::PointCount() const {
    if (!_skPath) return 0;
    return _skPath->countPoints();
}

void GraphicsPath::AddLine(int x1, int y1, int x2, int y2) {
    AddLine(static_cast<float>(x1), static_cast<float>(y1), 
            static_cast<float>(x2), static_cast<float>(y2));
}

void GraphicsPath::AddLine(float x1, float y1, float x2, float y2) {
    if (!_skPath) InitializeSkPath();
    
    if (_skPath->isEmpty()) {
        _skPath->moveTo(x1, y1);
    }
    _skPath->lineTo(x2, y2);
}

void GraphicsPath::AddLine(const Point& pt1, const Point& pt2) {
    AddLine(pt1.X, pt1.Y, pt2.X, pt2.Y);
}

void GraphicsPath::AddLine(const PointF& pt1, const PointF& pt2) {
    AddLine(pt1.X, pt1.Y, pt2.X, pt2.Y);
}

void GraphicsPath::AddLines(const std::vector<Point>& points) {
    if (points.empty()) return;
    
    if (!_skPath) InitializeSkPath();
    
    _skPath->moveTo(points[0].X, points[0].Y);
    for (size_t i = 1; i < points.size(); ++i) {
        _skPath->lineTo(points[i].X, points[i].Y);
    }
}

void GraphicsPath::AddLines(const std::vector<PointF>& points) {
    if (points.empty()) return;
    
    if (!_skPath) InitializeSkPath();
    
    _skPath->moveTo(points[0].X, points[0].Y);
    for (size_t i = 1; i < points.size(); ++i) {
        _skPath->lineTo(points[i].X, points[i].Y);
    }
}

void GraphicsPath::AddRectangle(const Rectangle& rect) {
    AddRectangle(RectangleF(rect));
}

void GraphicsPath::AddRectangle(const RectangleF& rect) {
    if (!_skPath) InitializeSkPath();
    
    SkRect skRect = rect.ToSkRect();
    _skPath->addRect(skRect);
}

void GraphicsPath::AddRectangles(const std::vector<Rectangle>& rects) {
    for (const auto& rect : rects) {
        AddRectangle(rect);
    }
}

void GraphicsPath::AddRectangles(const std::vector<RectangleF>& rects) {
    for (const auto& rect : rects) {
        AddRectangle(rect);
    }
}

void GraphicsPath::AddEllipse(int x, int y, int width, int height) {
    AddEllipse(static_cast<float>(x), static_cast<float>(y), 
               static_cast<float>(width), static_cast<float>(height));
}

void GraphicsPath::AddEllipse(float x, float y, float width, float height) {
    if (!_skPath) InitializeSkPath();
    
    SkRect skRect = SkRect::MakeXYWH(x, y, width, height);
    _skPath->addOval(skRect);
}

void GraphicsPath::AddEllipse(const Rectangle& rect) {
    AddEllipse(rect.X, rect.Y, rect.Width, rect.Height);
}

void GraphicsPath::AddEllipse(const RectangleF& rect) {
    AddEllipse(rect.X, rect.Y, rect.Width, rect.Height);
}

void GraphicsPath::AddArc(int x, int y, int width, int height, float startAngle, float sweepAngle) {
    AddArc(static_cast<float>(x), static_cast<float>(y), 
           static_cast<float>(width), static_cast<float>(height), startAngle, sweepAngle);
}

void GraphicsPath::AddArc(float x, float y, float width, float height, float startAngle, float sweepAngle) {
    if (!_skPath) InitializeSkPath();
    
    SkRect skRect = SkRect::MakeXYWH(x, y, width, height);
    _skPath->addArc(skRect, startAngle, sweepAngle);
}

void GraphicsPath::AddArc(const Rectangle& rect, float startAngle, float sweepAngle) {
    AddArc(rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
}

void GraphicsPath::AddArc(const RectangleF& rect, float startAngle, float sweepAngle) {
    AddArc(rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
}

void GraphicsPath::CloseFigure() {
    if (!_skPath) InitializeSkPath();
    _skPath->close();
}

void GraphicsPath::CloseAllFigures() {
    // TODO: Implement closing all figures
    CloseFigure();
}

void GraphicsPath::StartFigure() {
    // In Skia, starting a new figure is implicit when you moveTo
    // This method can be used to mark the intention
}

void GraphicsPath::Reset() {
    if (!_skPath) InitializeSkPath();
    _skPath->reset();
}

void GraphicsPath::Reverse() {
    if (!_skPath) InitializeSkPath();
    
    SkPath reversedPath;
    _skPath->reverseAddPath(&reversedPath);
    *_skPath = reversedPath;
}

RectangleF GraphicsPath::GetBounds() const {
    if (!_skPath) return RectangleF::Empty;
    
    SkRect bounds = _skPath->getBounds();
    return RectangleF::FromSkRect(bounds);
}

RectangleF GraphicsPath::GetBounds(const Matrix& matrix) const {
    if (!_skPath) return RectangleF::Empty;
    
    SkPath transformedPath;
    SkMatrix skMatrix = matrix.ToSkMatrix();
    _skPath->transform(skMatrix, &transformedPath);
    
    SkRect bounds = transformedPath.getBounds();
    return RectangleF::FromSkRect(bounds);
}

bool GraphicsPath::IsVisible(int x, int y) const {
    return IsVisible(static_cast<float>(x), static_cast<float>(y));
}

bool GraphicsPath::IsVisible(float x, float y) const {
    if (!_skPath) return false;
    return _skPath->contains(x, y);
}

bool GraphicsPath::IsVisible(const Point& point) const {
    return IsVisible(point.X, point.Y);
}

bool GraphicsPath::IsVisible(const PointF& point) const {
    return IsVisible(point.X, point.Y);
}

void GraphicsPath::Transform(const Matrix& matrix) {
    if (!_skPath) InitializeSkPath();
    
    SkMatrix skMatrix = matrix.ToSkMatrix();
    _skPath->transform(skMatrix);
}

std::unique_ptr<GraphicsPath> GraphicsPath::Clone() const {
    auto cloned = std::make_unique<GraphicsPath>(_fillMode);
    if (_skPath) {
        *cloned->_skPath = *_skPath;
    }
    return cloned;
}

void GraphicsPath::InitializeSkPath() {
    _skPath = std::make_unique<SkPath>();
    UpdateSkPathFillType();
}

void GraphicsPath::UpdateSkPathFillType() {
    if (!_skPath) return;
    
    switch (_fillMode) {
        case FillMode::Alternate:
            _skPath->setFillType(SkPathFillType::kEvenOdd);
            break;
        case FillMode::Winding:
            _skPath->setFillType(SkPathFillType::kWinding);
            break;
    }
}

// Placeholder implementations for methods not yet implemented
void GraphicsPath::AddPie(int x, int y, int width, int height, float startAngle, float sweepAngle) {
    throw std::runtime_error("AddPie not yet implemented");
}

void GraphicsPath::AddPie(float x, float y, float width, float height, float startAngle, float sweepAngle) {
    throw std::runtime_error("AddPie not yet implemented");
}

void GraphicsPath::AddPie(const Rectangle& rect, float startAngle, float sweepAngle) {
    throw std::runtime_error("AddPie not yet implemented");
}

void GraphicsPath::AddPie(const RectangleF& rect, float startAngle, float sweepAngle) {
    throw std::runtime_error("AddPie not yet implemented");
}

void GraphicsPath::AddPolygon(const std::vector<Point>& points) {
    throw std::runtime_error("AddPolygon not yet implemented");
}

void GraphicsPath::AddPolygon(const std::vector<PointF>& points) {
    throw std::runtime_error("AddPolygon not yet implemented");
}

void GraphicsPath::AddBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    throw std::runtime_error("AddBezier not yet implemented");
}

void GraphicsPath::AddBezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    throw std::runtime_error("AddBezier not yet implemented");
}

void GraphicsPath::AddBezier(const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4) {
    throw std::runtime_error("AddBezier not yet implemented");
}

void GraphicsPath::AddBezier(const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4) {
    throw std::runtime_error("AddBezier not yet implemented");
}

void GraphicsPath::AddBeziers(const std::vector<Point>& points) {
    throw std::runtime_error("AddBeziers not yet implemented");
}

void GraphicsPath::AddBeziers(const std::vector<PointF>& points) {
    throw std::runtime_error("AddBeziers not yet implemented");
}

void GraphicsPath::AddCurve(const std::vector<Point>& points) {
    throw std::runtime_error("AddCurve not yet implemented");
}

void GraphicsPath::AddCurve(const std::vector<PointF>& points) {
    throw std::runtime_error("AddCurve not yet implemented");
}

void GraphicsPath::AddCurve(const std::vector<Point>& points, float tension) {
    throw std::runtime_error("AddCurve not yet implemented");
}

void GraphicsPath::AddCurve(const std::vector<PointF>& points, float tension) {
    throw std::runtime_error("AddCurve not yet implemented");
}

void GraphicsPath::AddCurve(const std::vector<Point>& points, int offset, int numberOfSegments, float tension) {
    throw std::runtime_error("AddCurve not yet implemented");
}

void GraphicsPath::AddCurve(const std::vector<PointF>& points, int offset, int numberOfSegments, float tension) {
    throw std::runtime_error("AddCurve not yet implemented");
}

void GraphicsPath::AddClosedCurve(const std::vector<Point>& points) {
    throw std::runtime_error("AddClosedCurve not yet implemented");
}

void GraphicsPath::AddClosedCurve(const std::vector<PointF>& points) {
    throw std::runtime_error("AddClosedCurve not yet implemented");
}

void GraphicsPath::AddClosedCurve(const std::vector<Point>& points, float tension) {
    throw std::runtime_error("AddClosedCurve not yet implemented");
}

void GraphicsPath::AddClosedCurve(const std::vector<PointF>& points, float tension) {
    throw std::runtime_error("AddClosedCurve not yet implemented");
}

void GraphicsPath::AddPath(const GraphicsPath& addingPath, bool connect) {
    throw std::runtime_error("AddPath not yet implemented");
}

void GraphicsPath::AddString(const std::string& s, const FontFamily& family, int style, float emSize, const Point& origin) {
    throw std::runtime_error("AddString not yet implemented");
}

void GraphicsPath::AddString(const std::string& s, const FontFamily& family, int style, float emSize, const PointF& origin) {
    throw std::runtime_error("AddString not yet implemented");
}

void GraphicsPath::AddString(const std::string& s, const FontFamily& family, int style, float emSize, const Rectangle& layoutRect) {
    throw std::runtime_error("AddString not yet implemented");
}

void GraphicsPath::AddString(const std::string& s, const FontFamily& family, int style, float emSize, const RectangleF& layoutRect) {
    throw std::runtime_error("AddString not yet implemented");
}

bool GraphicsPath::IsVisible(int x, int y, const Graphics& graphics) const {
    return IsVisible(x, y);
}

bool GraphicsPath::IsVisible(float x, float y, const Graphics& graphics) const {
    return IsVisible(x, y);
}

bool GraphicsPath::IsVisible(const Point& point, const Graphics& graphics) const {
    return IsVisible(point);
}

bool GraphicsPath::IsVisible(const PointF& point, const Graphics& graphics) const {
    return IsVisible(point);
}

bool GraphicsPath::IsOutlineVisible(int x, int y, const Pen& pen) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

bool GraphicsPath::IsOutlineVisible(float x, float y, const Pen& pen) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

bool GraphicsPath::IsOutlineVisible(const Point& point, const Pen& pen) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

bool GraphicsPath::IsOutlineVisible(const PointF& point, const Pen& pen) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

bool GraphicsPath::IsOutlineVisible(int x, int y, const Pen& pen, const Graphics& graphics) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

bool GraphicsPath::IsOutlineVisible(float x, float y, const Pen& pen, const Graphics& graphics) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

bool GraphicsPath::IsOutlineVisible(const Point& point, const Pen& pen, const Graphics& graphics) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

bool GraphicsPath::IsOutlineVisible(const PointF& point, const Pen& pen, const Graphics& graphics) const {
    throw std::runtime_error("IsOutlineVisible not yet implemented");
}

void GraphicsPath::Flatten() {
    throw std::runtime_error("Flatten not yet implemented");
}

void GraphicsPath::Flatten(const Matrix& matrix) {
    throw std::runtime_error("Flatten not yet implemented");
}

void GraphicsPath::Flatten(const Matrix& matrix, float flatness) {
    throw std::runtime_error("Flatten not yet implemented");
}

void GraphicsPath::Widen(const Pen& pen) {
    throw std::runtime_error("Widen not yet implemented");
}

void GraphicsPath::Widen(const Pen& pen, const Matrix& matrix) {
    throw std::runtime_error("Widen not yet implemented");
}

void GraphicsPath::Widen(const Pen& pen, const Matrix& matrix, float flatness) {
    throw std::runtime_error("Widen not yet implemented");
}

void GraphicsPath::Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect) {
    throw std::runtime_error("Warp not yet implemented");
}

void GraphicsPath::Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect, const Matrix& matrix) {
    throw std::runtime_error("Warp not yet implemented");
}

void GraphicsPath::Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect, const Matrix& matrix, int warpMode) {
    throw std::runtime_error("Warp not yet implemented");
}

void GraphicsPath::Warp(const std::vector<PointF>& destPoints, const RectangleF& srcRect, const Matrix& matrix, int warpMode, float flatness) {
    throw std::runtime_error("Warp not yet implemented");
}

}
}
}
