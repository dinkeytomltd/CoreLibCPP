
#pragma once

#include <vector>

namespace System {
namespace Drawing {

// Backend types
enum class GraphicsBackendType {
    Skia,      // Cross-platform compatibility
    DirectX,   // Modern Windows performance (hardware accelerated)
    GdiPlus,   // Legacy Windows compatibility (software rendering)
    Auto       // Choose best available backend
};

class GraphicsConfiguration {
public:
    static GraphicsBackendType GetDefaultBackend();
    static void SetDefaultBackend(GraphicsBackendType type);
    static bool IsBackendAvailable(GraphicsBackendType type);
    static std::vector<GraphicsBackendType> GetAvailableBackends();
    
    // Performance preferences
    static void SetPreferHardwareAcceleration(bool prefer);
    static bool GetPreferHardwareAcceleration();
    
    // Backend-specific settings
    static void SetDirectXMultisampleCount(int count);
    static int GetDirectXMultisampleCount();
    
    static void SetSkiaGpuAcceleration(bool enable);
    static bool GetSkiaGpuAcceleration();
    
    // GDI+ settings
    static void SetGdiPlusTextRenderingHint(int hint);
    static int GetGdiPlusTextRenderingHint();
    static void SetGdiPlusSmoothingMode(int mode);
    static int GetGdiPlusSmoothingMode();

private:
    static GraphicsBackendType s_defaultBackend;
    static bool s_preferHardwareAcceleration;
    static int s_directXMultisampleCount;
    static bool s_skiaGpuAcceleration;
    static int s_gdiPlusTextRenderingHint;
    static int s_gdiPlusSmoothingMode;
};

}
}
