
#pragma once

#include "Point.h"
#include "Size.h"

// Forward declarations
namespace System { namespace Drawing {
    struct RectangleF;
}}

// Forward declarations for Skia
struct SkRect;
struct SkIRect;

namespace System {
namespace Drawing {

struct Rectangle {
    int X, Y, Width, Height;
    
    // Constructors
    Rectangle() : X(0), Y(0), Width(0), Height(0) {}
    Rectangle(int x, int y, int width, int height) : X(x), Y(y), Width(width), Height(height) {}
    Rectangle(const Point& location, const Size& size) : X(location.X), Y(location.Y), Width(size.Width), Height(size.Height) {}
    
    // Static members
    static const Rectangle Empty;
    
    // Properties
    Point Location() const { return Point(X, Y); }
    void SetLocation(const Point& location) { X = location.X; Y = location.Y; }
    Size GetSize() const { return Size(Width, Height); }
    void SetSize(const Size& size) { Width = size.Width; Height = size.Height; }
    int Left() const { return X; }
    int Top() const { return Y; }
    int Right() const { return X + Width; }
    int Bottom() const { return Y + Height; }
    bool IsEmpty() const { return Width <= 0 || Height <= 0; }
    
    // Methods
    bool Contains(int x, int y) const;
    bool Contains(const Point& pt) const;
    bool Contains(const Rectangle& rect) const;
    void Inflate(int width, int height);
    void Inflate(const Size& size);
    void Intersect(const Rectangle& rect);
    bool IntersectsWith(const Rectangle& rect) const;
    void Offset(int x, int y);
    void Offset(const Point& pos);
    
    // Static methods
    static Rectangle Ceiling(const RectangleF& value);
    static Rectangle Round(const RectangleF& value);
    static Rectangle Truncate(const RectangleF& value);
    static Rectangle FromLTRB(int left, int top, int right, int bottom);
    static Rectangle Inflate(const Rectangle& rect, int x, int y);
    static Rectangle Intersect(const Rectangle& a, const Rectangle& b);
    static Rectangle Union(const Rectangle& a, const Rectangle& b);
    
    // Operators
    bool operator==(const Rectangle& other) const;
    bool operator!=(const Rectangle& other) const;
    
    // Skia integration
    SkIRect ToSkIRect() const;
    static Rectangle FromSkIRect(const SkIRect& skRect);
    
    // Conversion
    std::string ToString() const;
};

struct RectangleF {
    float X, Y, Width, Height;
    
    // Constructors
    RectangleF() : X(0.0f), Y(0.0f), Width(0.0f), Height(0.0f) {}
    RectangleF(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height) {}
    RectangleF(const PointF& location, const SizeF& size) : X(location.X), Y(location.Y), Width(size.Width), Height(size.Height) {}
    RectangleF(const Rectangle& rect) : X(static_cast<float>(rect.X)), Y(static_cast<float>(rect.Y)), Width(static_cast<float>(rect.Width)), Height(static_cast<float>(rect.Height)) {}
    
    // Static members
    static const RectangleF Empty;
    
    // Properties
    PointF Location() const { return PointF(X, Y); }
    void SetLocation(const PointF& location) { X = location.X; Y = location.Y; }
    SizeF GetSize() const { return SizeF(Width, Height); }
    void SetSize(const SizeF& size) { Width = size.Width; Height = size.Height; }
    float Left() const { return X; }
    float Top() const { return Y; }
    float Right() const { return X + Width; }
    float Bottom() const { return Y + Height; }
    bool IsEmpty() const { return Width <= 0.0f || Height <= 0.0f; }
    
    // Methods
    bool Contains(float x, float y) const;
    bool Contains(const PointF& pt) const;
    bool Contains(const RectangleF& rect) const;
    void Inflate(float width, float height);
    void Inflate(const SizeF& size);
    void Intersect(const RectangleF& rect);
    bool IntersectsWith(const RectangleF& rect) const;
    void Offset(float x, float y);
    void Offset(const PointF& pos);
    
    // Static methods
    static RectangleF FromLTRB(float left, float top, float right, float bottom);
    static RectangleF Inflate(const RectangleF& rect, float x, float y);
    static RectangleF Intersect(const RectangleF& a, const RectangleF& b);
    static RectangleF Union(const RectangleF& a, const RectangleF& b);
    
    // Operators
    bool operator==(const RectangleF& other) const;
    bool operator!=(const RectangleF& other) const;
    
    // Skia integration
    SkRect ToSkRect() const;
    static RectangleF FromSkRect(const SkRect& skRect);
    
    // Conversion
    std::string ToString() const;
};

}
}
