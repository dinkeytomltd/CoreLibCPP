
#pragma once

#include "Size.h"
#include <cmath>

// Forward declarations
namespace System { namespace Drawing {
    struct PointF;
}}

// Forward declarations for Skia
struct SkPoint;
struct SkIPoint;

namespace System {
namespace Drawing {

struct Point {
    int X, Y;
    
    // Constructors
    Point() : X(0), Y(0) {}
    Point(int x, int y) : X(x), Y(y) {}
    Point(const Size& size) : X(size.Width), Y(size.Height) {}
    
    // Static members
    static const Point Empty;
    
    // Properties
    bool IsEmpty() const { return X == 0 && Y == 0; }
    
    // Methods
    void Offset(int dx, int dy) { X += dx; Y += dy; }
    void Offset(const Point& p) { X += p.X; Y += p.Y; }
    
    // Static methods
    static Point Add(const Point& pt, const Size& sz);
    static Point Subtract(const Point& pt, const Size& sz);
    static Point Ceiling(const PointF& value);
    static Point Round(const PointF& value);
    static Point Truncate(const PointF& value);
    
    // Operators
    Point operator+(const Size& size) const;
    Point operator-(const Size& size) const;
    Size operator-(const Point& pt) const;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    // Skia integration
    SkIPoint ToSkIPoint() const;
    static Point FromSkIPoint(const SkIPoint& skPoint);
    
    // Conversion
    std::string ToString() const;
};

struct PointF {
    float X, Y;
    
    // Constructors
    PointF() : X(0.0f), Y(0.0f) {}
    PointF(float x, float y) : X(x), Y(y) {}
    PointF(const SizeF& size) : X(size.Width), Y(size.Height) {}
    
    // Static members
    static const PointF Empty;
    
    // Properties
    bool IsEmpty() const { return X == 0.0f && Y == 0.0f; }
    
    // Methods
    void Offset(float dx, float dy) { X += dx; Y += dy; }
    void Offset(const PointF& p) { X += p.X; Y += p.Y; }
    
    // Static methods
    static PointF Add(const PointF& pt, const SizeF& sz);
    static PointF Subtract(const PointF& pt, const SizeF& sz);
    static float Distance(const PointF& pt1, const PointF& pt2);
    
    // Operators
    PointF operator+(const SizeF& size) const;
    PointF operator-(const SizeF& size) const;
    SizeF operator-(const PointF& pt) const;
    bool operator==(const PointF& other) const;
    bool operator!=(const PointF& other) const;
    
    // Skia integration
    SkPoint ToSkPoint() const;
    static PointF FromSkPoint(const SkPoint& skPoint);
    
    // Conversion
    std::string ToString() const;
};

}
}
