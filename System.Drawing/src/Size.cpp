
#include "System/Drawing/Size.h"
#include <sstream>
#include <cmath>

// Skia includes
#include "include/core/SkSize.h"

namespace System {
namespace Drawing {

// Static members
const Size Size::Empty = Size(0, 0);
const SizeF SizeF::Empty = SizeF(0.0f, 0.0f);

// Size implementation
Size::Size(const SizeF& size) 
    : Width(static_cast<int>(size.Width)), Height(static_cast<int>(size.Height)) {
}

Size Size::Add(const Size& sz1, const Size& sz2) {
    return Size(sz1.Width + sz2.Width, sz1.Height + sz2.Height);
}

Size Size::Subtract(const Size& sz1, const Size& sz2) {
    return Size(sz1.Width - sz2.Width, sz1.Height - sz2.Height);
}

Size Size::Ceiling(const SizeF& value) {
    return Size(
        static_cast<int>(std::ceil(value.Width)),
        static_cast<int>(std::ceil(value.Height))
    );
}

Size Size::Round(const SizeF& value) {
    return Size(
        static_cast<int>(std::round(value.Width)),
        static_cast<int>(std::round(value.Height))
    );
}

Size Size::Truncate(const SizeF& value) {
    return Size(
        static_cast<int>(value.Width),
        static_cast<int>(value.Height)
    );
}

Size Size::operator+(const Size& other) const {
    return Size(Width + other.Width, Height + other.Height);
}

Size Size::operator-(const Size& other) const {
    return Size(Width - other.Width, Height - other.Height);
}

bool Size::operator==(const Size& other) const {
    return Width == other.Width && Height == other.Height;
}

bool Size::operator!=(const Size& other) const {
    return !(*this == other);
}

SkISize Size::ToSkISize() const {
    return SkISize::Make(Width, Height);
}

Size Size::FromSkISize(const SkISize& skSize) {
    return Size(skSize.width(), skSize.height());
}

std::string Size::ToString() const {
    std::ostringstream oss;
    oss << "{Width=" << Width << ", Height=" << Height << "}";
    return oss.str();
}

// SizeF implementation
SizeF SizeF::Add(const SizeF& sz1, const SizeF& sz2) {
    return SizeF(sz1.Width + sz2.Width, sz1.Height + sz2.Height);
}

SizeF SizeF::Subtract(const SizeF& sz1, const SizeF& sz2) {
    return SizeF(sz1.Width - sz2.Width, sz1.Height - sz2.Height);
}

SizeF SizeF::operator+(const SizeF& other) const {
    return SizeF(Width + other.Width, Height + other.Height);
}

SizeF SizeF::operator-(const SizeF& other) const {
    return SizeF(Width - other.Width, Height - other.Height);
}

bool SizeF::operator==(const SizeF& other) const {
    return std::abs(Width - other.Width) < 1e-6f && std::abs(Height - other.Height) < 1e-6f;
}

bool SizeF::operator!=(const SizeF& other) const {
    return !(*this == other);
}

SkSize SizeF::ToSkSize() const {
    return SkSize::Make(Width, Height);
}

SizeF SizeF::FromSkSize(const SkSize& skSize) {
    return SizeF(skSize.width(), skSize.height());
}

std::string SizeF::ToString() const {
    std::ostringstream oss;
    oss << "{Width=" << Width << ", Height=" << Height << "}";
    return oss.str();
}

}
}
