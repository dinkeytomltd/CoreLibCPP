
#include "System/Drawing/Point.h"
#include <sstream>
#include <cmath>

// Skia includes
#include "include/core/SkPoint.h"

namespace System {
namespace Drawing {

// Static members
const Point Point::Empty = Point(0, 0);
const PointF PointF::Empty = PointF(0.0f, 0.0f);

// Point implementation
Point Point::Add(const Point& pt, const Size& sz) {
    return Point(pt.X + sz.Width, pt.Y + sz.Height);
}

Point Point::Subtract(const Point& pt, const Size& sz) {
    return Point(pt.X - sz.Width, pt.Y - sz.Height);
}

Point Point::Ceiling(const PointF& value) {
    return Point(
        static_cast<int>(std::ceil(value.X)),
        static_cast<int>(std::ceil(value.Y))
    );
}

Point Point::Round(const PointF& value) {
    return Point(
        static_cast<int>(std::round(value.X)),
        static_cast<int>(std::round(value.Y))
    );
}

Point Point::Truncate(const PointF& value) {
    return Point(
        static_cast<int>(value.X),
        static_cast<int>(value.Y)
    );
}

Point Point::operator+(const Size& size) const {
    return Point(X + size.Width, Y + size.Height);
}

Point Point::operator-(const Size& size) const {
    return Point(X - size.Width, Y - size.Height);
}

Size Point::operator-(const Point& pt) const {
    return Size(X - pt.X, Y - pt.Y);
}

bool Point::operator==(const Point& other) const {
    return X == other.X && Y == other.Y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

SkIPoint Point::ToSkIPoint() const {
    return SkIPoint::Make(X, Y);
}

Point Point::FromSkIPoint(const SkIPoint& skPoint) {
    return Point(skPoint.x(), skPoint.y());
}

std::string Point::ToString() const {
    std::ostringstream oss;
    oss << "{X=" << X << ",Y=" << Y << "}";
    return oss.str();
}

// PointF implementation
PointF PointF::Add(const PointF& pt, const SizeF& sz) {
    return PointF(pt.X + sz.Width, pt.Y + sz.Height);
}

PointF PointF::Subtract(const PointF& pt, const SizeF& sz) {
    return PointF(pt.X - sz.Width, pt.Y - sz.Height);
}

float PointF::Distance(const PointF& pt1, const PointF& pt2) {
    float dx = pt2.X - pt1.X;
    float dy = pt2.Y - pt1.Y;
    return std::sqrt(dx * dx + dy * dy);
}

PointF PointF::operator+(const SizeF& size) const {
    return PointF(X + size.Width, Y + size.Height);
}

PointF PointF::operator-(const SizeF& size) const {
    return PointF(X - size.Width, Y - size.Height);
}

SizeF PointF::operator-(const PointF& pt) const {
    return SizeF(X - pt.X, Y - pt.Y);
}

bool PointF::operator==(const PointF& other) const {
    return std::abs(X - other.X) < 1e-6f && std::abs(Y - other.Y) < 1e-6f;
}

bool PointF::operator!=(const PointF& other) const {
    return !(*this == other);
}

SkPoint PointF::ToSkPoint() const {
    return SkPoint::Make(X, Y);
}

PointF PointF::FromSkPoint(const SkPoint& skPoint) {
    return PointF(skPoint.x(), skPoint.y());
}

std::string PointF::ToString() const {
    std::ostringstream oss;
    oss << "{X=" << X << ",Y=" << Y << "}";
    return oss.str();
}

}
}
