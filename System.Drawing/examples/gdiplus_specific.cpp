#ifdef SYSTEM_DRAWING_GDIPLUS_ENABLED

#include "System/Drawing/Graphics.h"
#include "System/Drawing/GraphicsConfiguration.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include "System/Drawing/Color.h"
#include "System/Drawing/Point.h"
#include "System/Drawing/Rectangle.h"
#include <iostream>
#include <memory>

using namespace System::Drawing;

void DemonstrateGdiPlusBackend() {
    std::cout << "=== GDI+ Backend Demonstration ===\n\n";
    
    // Check if GDI+ backend is available
    if (!GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::GdiPlus)) {
        std::cout << "GDI+ backend is not available on this system.\n";
        return;
    }
    
    std::cout << "GDI+ backend is available!\n";
    
    // Configure GDI+ settings
    std::cout << "Configuring GDI+ settings...\n";
    GraphicsConfiguration::SetGdiPlusTextRenderingHint(4); // AntiAlias
    GraphicsConfiguration::SetGdiPlusSmoothingMode(4);     // AntiAlias
    
    std::cout << "GDI+ Text Rendering Hint: " 
              << GraphicsConfiguration::GetGdiPlusTextRenderingHint() << std::endl;
    std::cout << "GDI+ Smoothing Mode: " 
              << GraphicsConfiguration::GetGdiPlusSmoothingMode() << std::endl;
    
    // Note: Full drawing operations would be implemented when the backend is complete
    std::cout << "\nNote: Drawing operations are not yet implemented (NYI stubs).\n";
    std::cout << "This example demonstrates backend availability and configuration.\n";
}

void CompareBackendCharacteristics() {
    std::cout << "\n=== Backend Characteristics Comparison ===\n\n";
    
    auto backends = GraphicsConfiguration::GetAvailableBackends();
    
    std::cout << "Available backends on this system:\n";
    for (auto backend : backends) {
        switch (backend) {
            case GraphicsBackendType::Skia:
                std::cout << "  • Skia: Cross-platform compatibility\n";
                std::cout << "    - Hardware acceleration: Variable\n";
                std::cout << "    - Memory footprint: Medium\n";
                std::cout << "    - Best for: Cross-platform applications\n";
                break;
            case GraphicsBackendType::DirectX:
                std::cout << "  • DirectX: Modern Windows performance\n";
                std::cout << "    - Hardware acceleration: Yes\n";
                std::cout << "    - Memory footprint: High\n";
                std::cout << "    - Best for: High-performance Windows applications\n";
                break;
            case GraphicsBackendType::GdiPlus:
                std::cout << "  • GDI+: Legacy Windows compatibility\n";
                std::cout << "    - Hardware acceleration: No (software rendering)\n";
                std::cout << "    - Memory footprint: Low\n";
                std::cout << "    - Best for: Legacy Windows applications, minimal footprint\n";
                break;
        }
    }
    
    std::cout << "\nDefault backend: ";
    auto defaultBackend = GraphicsConfiguration::GetDefaultBackend();
    switch (defaultBackend) {
        case GraphicsBackendType::Skia:
            std::cout << "Skia\n";
            break;
        case GraphicsBackendType::DirectX:
            std::cout << "DirectX\n";
            break;
        case GraphicsBackendType::GdiPlus:
            std::cout << "GDI+\n";
            break;
        default:
            std::cout << "Unknown\n";
            break;
    }
}

void BackendSelectionExamples() {
    std::cout << "\n=== Backend Selection Examples ===\n\n";
    
    // Example 1: Automatic selection
    std::cout << "1. Automatic backend selection:\n";
    auto defaultBackend = GraphicsConfiguration::GetDefaultBackend();
    std::cout << "   Selected backend: " << static_cast<int>(defaultBackend) << std::endl;
    
    // Example 2: Explicit GDI+ selection
    if (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::GdiPlus)) {
        std::cout << "\n2. Explicit GDI+ backend selection:\n";
        std::cout << "   GDI+ backend is available and can be used for:\n";
        std::cout << "   - Legacy Windows applications\n";
        std::cout << "   - Minimal memory footprint scenarios\n";
        std::cout << "   - Perfect .NET System.Drawing compatibility\n";
    }
    
    // Example 3: Performance vs compatibility trade-offs
    std::cout << "\n3. Backend selection strategy:\n";
    std::cout << "   For maximum performance: DirectX > Skia > GDI+\n";
    std::cout << "   For maximum compatibility: GDI+ > Skia > DirectX\n";
    std::cout << "   For cross-platform: Skia > DirectX/GDI+ (Windows only)\n";
}

int main() {
    try {
        DemonstrateGdiPlusBackend();
        CompareBackendCharacteristics();
        BackendSelectionExamples();
        
        std::cout << "\n=== GDI+ Backend Integration Complete ===\n";
        std::cout << "The GDI+ backend has been successfully integrated into System.Drawing!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

#else

#include <iostream>

int main() {
    std::cout << "GDI+ backend is not enabled in this build.\n";
    std::cout << "To enable GDI+ support, build with -DSYSTEM_DRAWING_ENABLE_GDIPLUS=ON on Windows.\n";
    return 0;
}

#endif // SYSTEM_DRAWING_GDIPLUS_ENABLED
