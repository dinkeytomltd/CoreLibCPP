
#include "System/Drawing/Drawing2D/Matrix.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

// Skia includes
#include "include/core/SkMatrix.h"

namespace System {
namespace Drawing {
namespace Drawing2D {

Matrix::Matrix() {
    SetElements(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f); // Identity matrix
}

Matrix::Matrix(float m11, float m12, float m21, float m22, float dx, float dy) {
    SetElements(m11, m12, m21, m22, dx, dy);
}

Matrix::Matrix(const Matrix& other) {
    std::copy(other._elements, other._elements + 6, _elements);
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        std::copy(other._elements, other._elements + 6, _elements);
    }
    return *this;
}

Matrix::Matrix(Matrix&& other) noexcept {
    std::copy(other._elements, other._elements + 6, _elements);
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        std::copy(other._elements, other._elements + 6, _elements);
    }
    return *this;
}

std::vector<float> Matrix::Elements() const {
    return std::vector<float>(_elements, _elements + 6);
}

bool Matrix::IsIdentity() const {
    return std::abs(_elements[0] - 1.0f) < 1e-6f &&
           std::abs(_elements[1]) < 1e-6f &&
           std::abs(_elements[2]) < 1e-6f &&
           std::abs(_elements[3] - 1.0f) < 1e-6f &&
           std::abs(_elements[4]) < 1e-6f &&
           std::abs(_elements[5]) < 1e-6f;
}

bool Matrix::IsInvertible() const {
    float determinant = _elements[0] * _elements[3] - _elements[1] * _elements[2];
    return std::abs(determinant) > 1e-6f;
}

void Matrix::Translate(float offsetX, float offsetY) {
    Translate(offsetX, offsetY, MatrixOrder::Prepend);
}

void Matrix::Translate(float offsetX, float offsetY, MatrixOrder order) {
    Matrix translation(1.0f, 0.0f, 0.0f, 1.0f, offsetX, offsetY);
    Multiply(translation, order);
}

void Matrix::Scale(float scaleX, float scaleY) {
    Scale(scaleX, scaleY, MatrixOrder::Prepend);
}

void Matrix::Scale(float scaleX, float scaleY, MatrixOrder order) {
    Matrix scaling(scaleX, 0.0f, 0.0f, scaleY, 0.0f, 0.0f);
    Multiply(scaling, order);
}

void Matrix::Rotate(float angle) {
    Rotate(angle, MatrixOrder::Prepend);
}

void Matrix::Rotate(float angle, MatrixOrder order) {
    float radians = angle * static_cast<float>(M_PI) / 180.0f;
    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);
    
    Matrix rotation(cosAngle, sinAngle, -sinAngle, cosAngle, 0.0f, 0.0f);
    Multiply(rotation, order);
}

void Matrix::RotateAt(float angle, const PointF& point) {
    RotateAt(angle, point, MatrixOrder::Prepend);
}

void Matrix::RotateAt(float angle, const PointF& point, MatrixOrder order) {
    Translate(-point.X, -point.Y, order);
    Rotate(angle, order);
    Translate(point.X, point.Y, order);
}

void Matrix::Shear(float shearX, float shearY) {
    Shear(shearX, shearY, MatrixOrder::Prepend);
}

void Matrix::Shear(float shearX, float shearY, MatrixOrder order) {
    Matrix shearing(1.0f, shearY, shearX, 1.0f, 0.0f, 0.0f);
    Multiply(shearing, order);
}

void Matrix::Multiply(const Matrix& matrix) {
    Multiply(matrix, MatrixOrder::Prepend);
}

void Matrix::Multiply(const Matrix& matrix, MatrixOrder order) {
    const float* a = (order == MatrixOrder::Prepend) ? matrix._elements : _elements;
    const float* b = (order == MatrixOrder::Prepend) ? _elements : matrix._elements;
    
    float result[6];
    result[0] = a[0] * b[0] + a[1] * b[2];           // m11
    result[1] = a[0] * b[1] + a[1] * b[3];           // m12
    result[2] = a[2] * b[0] + a[3] * b[2];           // m21
    result[3] = a[2] * b[1] + a[3] * b[3];           // m22
    result[4] = a[4] * b[0] + a[5] * b[2] + b[4];    // dx
    result[5] = a[4] * b[1] + a[5] * b[3] + b[5];    // dy
    
    std::copy(result, result + 6, _elements);
}

void Matrix::Invert() {
    if (!IsInvertible()) {
        throw std::runtime_error("Matrix is not invertible");
    }
    
    float determinant = _elements[0] * _elements[3] - _elements[1] * _elements[2];
    float invDet = 1.0f / determinant;
    
    float result[6];
    result[0] = _elements[3] * invDet;                                           // m11
    result[1] = -_elements[1] * invDet;                                          // m12
    result[2] = -_elements[2] * invDet;                                          // m21
    result[3] = _elements[0] * invDet;                                           // m22
    result[4] = (_elements[2] * _elements[5] - _elements[3] * _elements[4]) * invDet; // dx
    result[5] = (_elements[1] * _elements[4] - _elements[0] * _elements[5]) * invDet; // dy
    
    std::copy(result, result + 6, _elements);
}

void Matrix::Reset() {
    SetElements(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
}

std::vector<PointF> Matrix::TransformPoints(const std::vector<PointF>& pts) const {
    std::vector<PointF> result;
    result.reserve(pts.size());
    
    for (const auto& pt : pts) {
        result.push_back(TransformPoint(pt));
    }
    
    return result;
}

std::vector<Point> Matrix::TransformPoints(const std::vector<Point>& pts) const {
    std::vector<Point> result;
    result.reserve(pts.size());
    
    for (const auto& pt : pts) {
        result.push_back(TransformPoint(pt));
    }
    
    return result;
}

std::vector<PointF> Matrix::TransformVectors(const std::vector<PointF>& pts) const {
    std::vector<PointF> result;
    result.reserve(pts.size());
    
    for (const auto& pt : pts) {
        result.push_back(TransformVector(pt));
    }
    
    return result;
}

PointF Matrix::TransformPoint(const PointF& point) const {
    float x = point.X * _elements[0] + point.Y * _elements[2] + _elements[4];
    float y = point.X * _elements[1] + point.Y * _elements[3] + _elements[5];
    return PointF(x, y);
}

Point Matrix::TransformPoint(const Point& point) const {
    PointF result = TransformPoint(PointF(static_cast<float>(point.X), static_cast<float>(point.Y)));
    return Point(static_cast<int>(std::round(result.X)), static_cast<int>(std::round(result.Y)));
}

PointF Matrix::TransformVector(const PointF& vector) const {
    // Transform vector (no translation)
    float x = vector.X * _elements[0] + vector.Y * _elements[2];
    float y = vector.X * _elements[1] + vector.Y * _elements[3];
    return PointF(x, y);
}

bool Matrix::operator==(const Matrix& other) const {
    for (int i = 0; i < 6; ++i) {
        if (std::abs(_elements[i] - other._elements[i]) > 1e-6f) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

std::string Matrix::ToString() const {
    std::ostringstream oss;
    oss << "[" << _elements[0] << ", " << _elements[1] << ", " << _elements[2] 
        << ", " << _elements[3] << ", " << _elements[4] << ", " << _elements[5] << "]";
    return oss.str();
}

SkMatrix Matrix::ToSkMatrix() const {
    SkMatrix skMatrix;
    skMatrix.setAll(
        _elements[0], _elements[2], _elements[4],  // scaleX, skewX, transX
        _elements[1], _elements[3], _elements[5],  // skewY, scaleY, transY
        0, 0, 1                                    // persp0, persp1, persp2
    );
    return skMatrix;
}

Matrix Matrix::FromSkMatrix(const SkMatrix& skMatrix) {
    return Matrix(
        skMatrix.getScaleX(),    // m11
        skMatrix.getSkewY(),     // m12
        skMatrix.getSkewX(),     // m21
        skMatrix.getScaleY(),    // m22
        skMatrix.getTranslateX(), // dx
        skMatrix.getTranslateY()  // dy
    );
}

void Matrix::SetSkMatrix(const SkMatrix& skMatrix) {
    *this = FromSkMatrix(skMatrix);
}

void Matrix::SetElements(float m11, float m12, float m21, float m22, float dx, float dy) {
    _elements[0] = m11;
    _elements[1] = m12;
    _elements[2] = m21;
    _elements[3] = m22;
    _elements[4] = dx;
    _elements[5] = dy;
}

Matrix Matrix::CreateMatrix(float m11, float m12, float m21, float m22, float dx, float dy) const {
    return Matrix(m11, m12, m21, m22, dx, dy);
}

}
}
}
