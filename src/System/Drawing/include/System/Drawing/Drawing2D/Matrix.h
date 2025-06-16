
#pragma once

#include "../Point.h"
#include <vector>
#include <string>

// Forward declarations for Skia
class SkMatrix;

namespace System {
namespace Drawing {
namespace Drawing2D {

enum class MatrixOrder {
    Prepend = 0,
    Append = 1
};

class Matrix {
private:
    float _elements[6]; // 2x3 transformation matrix: [m11, m12, m21, m22, dx, dy]
    
public:
    // Constructors
    Matrix();
    Matrix(float m11, float m12, float m21, float m22, float dx, float dy);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;
    
    // Destructor
    ~Matrix() = default;
    
    // Properties
    std::vector<float> Elements() const;
    bool IsIdentity() const;
    bool IsInvertible() const;
    float OffsetX() const { return _elements[4]; }
    float OffsetY() const { return _elements[5]; }
    
    // Transformation methods
    void Translate(float offsetX, float offsetY);
    void Translate(float offsetX, float offsetY, MatrixOrder order);
    void Scale(float scaleX, float scaleY);
    void Scale(float scaleX, float scaleY, MatrixOrder order);
    void Rotate(float angle);
    void Rotate(float angle, MatrixOrder order);
    void RotateAt(float angle, const PointF& point);
    void RotateAt(float angle, const PointF& point, MatrixOrder order);
    void Shear(float shearX, float shearY);
    void Shear(float shearX, float shearY, MatrixOrder order);
    
    // Matrix operations
    void Multiply(const Matrix& matrix);
    void Multiply(const Matrix& matrix, MatrixOrder order);
    void Invert();
    void Reset();
    
    // Point transformation
    std::vector<PointF> TransformPoints(const std::vector<PointF>& pts) const;
    std::vector<Point> TransformPoints(const std::vector<Point>& pts) const;
    std::vector<PointF> TransformVectors(const std::vector<PointF>& pts) const;
    
    // Single point transformation
    PointF TransformPoint(const PointF& point) const;
    Point TransformPoint(const Point& point) const;
    PointF TransformVector(const PointF& vector) const;
    
    // Equality
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    
    // String representation
    std::string ToString() const;
    
    // Skia integration
    SkMatrix ToSkMatrix() const;
    static Matrix FromSkMatrix(const SkMatrix& skMatrix);
    void SetSkMatrix(const SkMatrix& skMatrix);
    
private:
    void SetElements(float m11, float m12, float m21, float m22, float dx, float dy);
    Matrix CreateMatrix(float m11, float m12, float m21, float m22, float dx, float dy) const;
};

}
}
}
