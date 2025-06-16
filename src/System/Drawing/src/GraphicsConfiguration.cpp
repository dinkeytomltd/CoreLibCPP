
#include "System/Drawing/GraphicsConfiguration.h"
#include "System/Drawing/IGraphicsBackend.h"

namespace System {
namespace Drawing {

// Static member definitions
GraphicsBackendType GraphicsConfiguration::s_defaultBackend = GraphicsBackendType::Auto;
bool GraphicsConfiguration::s_preferHardwareAcceleration = true;
int GraphicsConfiguration::s_directXMultisampleCount = 4;
bool GraphicsConfiguration::s_skiaGpuAcceleration = true;
int GraphicsConfiguration::s_gdiPlusTextRenderingHint = 4; // AntiAlias
int GraphicsConfiguration::s_gdiPlusSmoothingMode = 4;     // AntiAlias

GraphicsBackendType GraphicsConfiguration::GetDefaultBackend() {
    if (s_defaultBackend == GraphicsBackendType::Auto) {
#ifdef _WIN32
        // Windows: Prefer DirectX > GDI+ > Skia
        if (IsBackendAvailable(GraphicsBackendType::DirectX)) {
            return GraphicsBackendType::DirectX;
        } else if (IsBackendAvailable(GraphicsBackendType::GdiPlus)) {
            return GraphicsBackendType::GdiPlus;
        }
#endif
        return GraphicsBackendType::Skia;
    }
    return s_defaultBackend;
}

void GraphicsConfiguration::SetDefaultBackend(GraphicsBackendType type) {
    s_defaultBackend = type;
}

bool GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType type) {
    switch (type) {
        case GraphicsBackendType::Skia:
            return true;  // Always available
#ifdef _WIN32
        case GraphicsBackendType::DirectX:
            // TODO: Check for DirectX availability at runtime
            return true;  // Assume available on Windows for now
        case GraphicsBackendType::GdiPlus:
            return true;  // GDI+ is built into Windows
#endif
        default:
            return false;
    }
}

std::vector<GraphicsBackendType> GraphicsConfiguration::GetAvailableBackends() {
    std::vector<GraphicsBackendType> backends;
    
    if (IsBackendAvailable(GraphicsBackendType::Skia)) {
        backends.push_back(GraphicsBackendType::Skia);
    }
    
#ifdef _WIN32
    if (IsBackendAvailable(GraphicsBackendType::DirectX)) {
        backends.push_back(GraphicsBackendType::DirectX);
    }
    if (IsBackendAvailable(GraphicsBackendType::GdiPlus)) {
        backends.push_back(GraphicsBackendType::GdiPlus);
    }
#endif
    
    return backends;
}

void GraphicsConfiguration::SetPreferHardwareAcceleration(bool prefer) {
    s_preferHardwareAcceleration = prefer;
}

bool GraphicsConfiguration::GetPreferHardwareAcceleration() {
    return s_preferHardwareAcceleration;
}

void GraphicsConfiguration::SetDirectXMultisampleCount(int count) {
    s_directXMultisampleCount = count;
}

int GraphicsConfiguration::GetDirectXMultisampleCount() {
    return s_directXMultisampleCount;
}

void GraphicsConfiguration::SetSkiaGpuAcceleration(bool enable) {
    s_skiaGpuAcceleration = enable;
}

bool GraphicsConfiguration::GetSkiaGpuAcceleration() {
    return s_skiaGpuAcceleration;
}

void GraphicsConfiguration::SetGdiPlusTextRenderingHint(int hint) {
    s_gdiPlusTextRenderingHint = hint;
}

int GraphicsConfiguration::GetGdiPlusTextRenderingHint() {
    return s_gdiPlusTextRenderingHint;
}

void GraphicsConfiguration::SetGdiPlusSmoothingMode(int mode) {
    s_gdiPlusSmoothingMode = mode;
}

int GraphicsConfiguration::GetGdiPlusSmoothingMode() {
    return s_gdiPlusSmoothingMode;
}

}
}
