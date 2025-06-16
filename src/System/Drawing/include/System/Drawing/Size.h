
#pragma once

#include <string>

// Forward declarations for Skia
struct SkSize;
struct SkISize;

namespace System {
namespace Drawing {

// Forward declaration
struct SizeF;

struct Size {
    int Width, Height;
    
    // Constructors
    Size() : Width(0), Height(0) {}
    Size(int width, int height) : Width(width), Height(height) {}
    explicit Size(const SizeF& size);
    
    // Static members
    static const Size Empty;
    
    // Properties
    bool IsEmpty() const { return Width == 0 && Height == 0; }
    
    // Static methods
    static Size Add(const Size& sz1, const Size& sz2);
    static Size Subtract(const Size& sz1, const Size& sz2);
    static Size Ceiling(const SizeF& value);
    static Size Round(const SizeF& value);
    static Size Truncate(const SizeF& value);
    
    // Operators
    Size operator+(const Size& other) const;
    Size operator-(const Size& other) const;
    bool operator==(const Size& other) const;
    bool operator!=(const Size& other) const;
    
    // Skia integration
    SkISize ToSkISize() const;
    static Size FromSkISize(const SkISize& skSize);
    
    // Conversion
    std::string ToString() const;
};

struct SizeF {
    float Width, Height;
    
    // Constructors
    SizeF() : Width(0.0f), Height(0.0f) {}
    SizeF(float width, float height) : Width(width), Height(height) {}
    SizeF(const Size& size) : Width(static_cast<float>(size.Width)), Height(static_cast<float>(size.Height)) {}
    
    // Static members
    static const SizeF Empty;
    
    // Properties
    bool IsEmpty() const { return Width == 0.0f && Height == 0.0f; }
    
    // Static methods
    static SizeF Add(const SizeF& sz1, const SizeF& sz2);
    static SizeF Subtract(const SizeF& sz1, const SizeF& sz2);
    
    // Operators
    SizeF operator+(const SizeF& other) const;
    SizeF operator-(const SizeF& other) const;
    bool operator==(const SizeF& other) const;
    bool operator!=(const SizeF& other) const;
    
    // Skia integration
    SkSize ToSkSize() const;
    static SizeF FromSkSize(const SkSize& skSize);
    
    // Conversion
    std::string ToString() const;
};

}
}
