
#include "System/Drawing/Rectangle.h"
#include <algorithm>
#include <sstream>
#include <cmath>

// Skia includes
#include "include/core/SkRect.h"

namespace System {
namespace Drawing {

// Static members
const Rectangle Rectangle::Empty = Rectangle(0, 0, 0, 0);
const RectangleF RectangleF::Empty = RectangleF(0.0f, 0.0f, 0.0f, 0.0f);

// Rectangle implementation
bool Rectangle::Contains(int x, int y) const {
    return x >= X && x < X + Width && y >= Y && y < Y + Height;
}

bool Rectangle::Contains(const Point& pt) const {
    return Contains(pt.X, pt.Y);
}

bool Rectangle::Contains(const Rectangle& rect) const {
    return rect.X >= X && rect.Y >= Y && 
           rect.X + rect.Width <= X + Width && 
           rect.Y + rect.Height <= Y + Height;
}

void Rectangle::Inflate(int width, int height) {
    X -= width;
    Y -= height;
    Width += 2 * width;
    Height += 2 * height;
}

void Rectangle::Inflate(const Size& size) {
    Inflate(size.Width, size.Height);
}

void Rectangle::Intersect(const Rectangle& rect) {
    *this = Intersect(*this, rect);
}

bool Rectangle::IntersectsWith(const Rectangle& rect) const {
    return rect.X < X + Width && X < rect.X + rect.Width &&
           rect.Y < Y + Height && Y < rect.Y + rect.Height;
}

void Rectangle::Offset(int x, int y) {
    X += x;
    Y += y;
}

void Rectangle::Offset(const Point& pos) {
    Offset(pos.X, pos.Y);
}

Rectangle Rectangle::Ceiling(const RectangleF& value) {
    return Rectangle(
        static_cast<int>(std::floor(value.X)),
        static_cast<int>(std::floor(value.Y)),
        static_cast<int>(std::ceil(value.X + value.Width) - std::floor(value.X)),
        static_cast<int>(std::ceil(value.Y + value.Height) - std::floor(value.Y))
    );
}

Rectangle Rectangle::Round(const RectangleF& value) {
    return Rectangle(
        static_cast<int>(std::round(value.X)),
        static_cast<int>(std::round(value.Y)),
        static_cast<int>(std::round(value.Width)),
        static_cast<int>(std::round(value.Height))
    );
}

Rectangle Rectangle::Truncate(const RectangleF& value) {
    return Rectangle(
        static_cast<int>(value.X),
        static_cast<int>(value.Y),
        static_cast<int>(value.Width),
        static_cast<int>(value.Height)
    );
}

Rectangle Rectangle::FromLTRB(int left, int top, int right, int bottom) {
    return Rectangle(left, top, right - left, bottom - top);
}

Rectangle Rectangle::Inflate(const Rectangle& rect, int x, int y) {
    Rectangle result = rect;
    result.Inflate(x, y);
    return result;
}

Rectangle Rectangle::Intersect(const Rectangle& a, const Rectangle& b) {
    int x1 = std::max(a.X, b.X);
    int x2 = std::min(a.X + a.Width, b.X + b.Width);
    int y1 = std::max(a.Y, b.Y);
    int y2 = std::min(a.Y + a.Height, b.Y + b.Height);
    
    if (x2 >= x1 && y2 >= y1) {
        return Rectangle(x1, y1, x2 - x1, y2 - y1);
    }
    return Rectangle::Empty;
}

Rectangle Rectangle::Union(const Rectangle& a, const Rectangle& b) {
    if (a.IsEmpty()) return b;
    if (b.IsEmpty()) return a;
    
    int x1 = std::min(a.X, b.X);
    int x2 = std::max(a.X + a.Width, b.X + b.Width);
    int y1 = std::min(a.Y, b.Y);
    int y2 = std::max(a.Y + a.Height, b.Y + b.Height);
    
    return Rectangle(x1, y1, x2 - x1, y2 - y1);
}

bool Rectangle::operator==(const Rectangle& other) const {
    return X == other.X && Y == other.Y && Width == other.Width && Height == other.Height;
}

bool Rectangle::operator!=(const Rectangle& other) const {
    return !(*this == other);
}

SkIRect Rectangle::ToSkIRect() const {
    return SkIRect::MakeXYWH(X, Y, Width, Height);
}

Rectangle Rectangle::FromSkIRect(const SkIRect& skRect) {
    return Rectangle(skRect.x(), skRect.y(), skRect.width(), skRect.height());
}

std::string Rectangle::ToString() const {
    std::ostringstream oss;
    oss << "{X=" << X << ",Y=" << Y << ",Width=" << Width << ",Height=" << Height << "}";
    return oss.str();
}

// RectangleF implementation
bool RectangleF::Contains(float x, float y) const {
    return x >= X && x < X + Width && y >= Y && y < Y + Height;
}

bool RectangleF::Contains(const PointF& pt) const {
    return Contains(pt.X, pt.Y);
}

bool RectangleF::Contains(const RectangleF& rect) const {
    return rect.X >= X && rect.Y >= Y && 
           rect.X + rect.Width <= X + Width && 
           rect.Y + rect.Height <= Y + Height;
}

void RectangleF::Inflate(float width, float height) {
    X -= width;
    Y -= height;
    Width += 2 * width;
    Height += 2 * height;
}

void RectangleF::Inflate(const SizeF& size) {
    Inflate(size.Width, size.Height);
}

void RectangleF::Intersect(const RectangleF& rect) {
    *this = Intersect(*this, rect);
}

bool RectangleF::IntersectsWith(const RectangleF& rect) const {
    return rect.X < X + Width && X < rect.X + rect.Width &&
           rect.Y < Y + Height && Y < rect.Y + rect.Height;
}

void RectangleF::Offset(float x, float y) {
    X += x;
    Y += y;
}

void RectangleF::Offset(const PointF& pos) {
    Offset(pos.X, pos.Y);
}

RectangleF RectangleF::FromLTRB(float left, float top, float right, float bottom) {
    return RectangleF(left, top, right - left, bottom - top);
}

RectangleF RectangleF::Inflate(const RectangleF& rect, float x, float y) {
    RectangleF result = rect;
    result.Inflate(x, y);
    return result;
}

RectangleF RectangleF::Intersect(const RectangleF& a, const RectangleF& b) {
    float x1 = std::max(a.X, b.X);
    float x2 = std::min(a.X + a.Width, b.X + b.Width);
    float y1 = std::max(a.Y, b.Y);
    float y2 = std::min(a.Y + a.Height, b.Y + b.Height);
    
    if (x2 >= x1 && y2 >= y1) {
        return RectangleF(x1, y1, x2 - x1, y2 - y1);
    }
    return RectangleF::Empty;
}

RectangleF RectangleF::Union(const RectangleF& a, const RectangleF& b) {
    if (a.IsEmpty()) return b;
    if (b.IsEmpty()) return a;
    
    float x1 = std::min(a.X, b.X);
    float x2 = std::max(a.X + a.Width, b.X + b.Width);
    float y1 = std::min(a.Y, b.Y);
    float y2 = std::max(a.Y + a.Height, b.Y + b.Height);
    
    return RectangleF(x1, y1, x2 - x1, y2 - y1);
}

bool RectangleF::operator==(const RectangleF& other) const {
    return std::abs(X - other.X) < 1e-6f && std::abs(Y - other.Y) < 1e-6f &&
           std::abs(Width - other.Width) < 1e-6f && std::abs(Height - other.Height) < 1e-6f;
}

bool RectangleF::operator!=(const RectangleF& other) const {
    return !(*this == other);
}

SkRect RectangleF::ToSkRect() const {
    return SkRect::MakeXYWH(X, Y, Width, Height);
}

RectangleF RectangleF::FromSkRect(const SkRect& skRect) {
    return RectangleF(skRect.x(), skRect.y(), skRect.width(), skRect.height());
}

std::string RectangleF::ToString() const {
    std::ostringstream oss;
    oss << "{X=" << X << ",Y=" << Y << ",Width=" << Width << ",Height=" << Height << "}";
    return oss.str();
}

}
}
