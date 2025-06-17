
#ifdef _WIN32

#include <iostream>
#include <windows.h>
#include "System/Drawing/Graphics.h"
#include "System/Drawing/GraphicsConfiguration.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include "src/DirectXBackend/DirectXBackend.h"

using namespace System::Drawing;

void DirectXSpecificFeaturesExample() {
    std::cout << "=== DirectX-Specific Features Example ===" << std::endl;
    
    if (!GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
        std::cout << "DirectX backend not available on this system." << std::endl;
        return;
    }
    
    // Create DirectX backend directly
    auto directxBackend = std::make_unique<DirectXBackend>();
    
    std::cout << "DirectX backend created successfully" << std::endl;
    std::cout << "Hardware acceleration: " 
              << (directxBackend->IsHardwareAccelerated() ? "Yes" : "No") << std::endl;
    
    // Configure DirectX-specific settings
    GraphicsConfiguration::SetDirectXMultisampleCount(8);
    std::cout << "Multisample count set to: " 
              << GraphicsConfiguration::GetDirectXMultisampleCount() << "x" << std::endl;
    
    // TODO: Demonstrate DirectX-specific drawing operations
    std::cout << "DirectX-specific drawing operations would be demonstrated here." << std::endl;
}

void DirectXWindowExample() {
    std::cout << "\n=== DirectX Window Rendering Example ===" << std::endl;
    
    if (!GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
        std::cout << "DirectX backend not available." << std::endl;
        return;
    }
    
    // This would typically create a window and render to it
    std::cout << "DirectX window rendering example would be implemented here." << std::endl;
    std::cout << "This would involve:" << std::endl;
    std::cout << "  1. Creating a Windows window (HWND)" << std::endl;
    std::cout << "  2. Initializing DirectX backend with the window" << std::endl;
    std::cout << "  3. Performing hardware-accelerated drawing operations" << std::endl;
    std::cout << "  4. Presenting the results to the screen" << std::endl;
}

void DirectXPerformanceExample() {
    std::cout << "\n=== DirectX Performance Features ===" << std::endl;
    
    if (!GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
        std::cout << "DirectX backend not available." << std::endl;
        return;
    }
    
    std::cout << "DirectX performance features:" << std::endl;
    std::cout << "  - Hardware-accelerated 2D graphics" << std::endl;
    std::cout << "  - GPU-based text rendering with DirectWrite" << std::endl;
    std::cout << "  - Efficient image processing with WIC" << std::endl;
    std::cout << "  - Multi-sample anti-aliasing (MSAA)" << std::endl;
    std::cout << "  - Native Windows integration" << std::endl;
    
    // Show current configuration
    std::cout << "\nCurrent DirectX configuration:" << std::endl;
    std::cout << "  - Multisample count: " 
              << GraphicsConfiguration::GetDirectXMultisampleCount() << "x" << std::endl;
    std::cout << "  - Hardware acceleration preferred: " 
              << (GraphicsConfiguration::GetPreferHardwareAcceleration() ? "Yes" : "No") << std::endl;
}

int main() {
    std::cout << "DirectX Backend Specific Features Demo" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    DirectXSpecificFeaturesExample();
    DirectXWindowExample();
    DirectXPerformanceExample();
    
    return 0;
}

#else

int main() {
    std::cout << "DirectX backend is only available on Windows platforms." << std::endl;
    return 0;
}

#endif // _WIN32
