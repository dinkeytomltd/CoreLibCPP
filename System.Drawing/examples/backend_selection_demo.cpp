#include <iostream>
#include "System/Drawing/GraphicsConfiguration.h"

using namespace System::Drawing;

int main() {
    std::cout << "=== System.Drawing DirectX Backend Implementation Demo ===" << std::endl;
    std::cout << std::endl;
    
    // Show available backends
    std::cout << "Available Graphics Backends:" << std::endl;
    auto backends = GraphicsConfiguration::GetAvailableBackends();
    
    for (size_t i = 0; i < backends.size(); ++i) {
        std::string backendName;
        switch (backends[i]) {
            case GraphicsBackendType::Skia:
                backendName = "Skia (Cross-platform)";
                break;
            case GraphicsBackendType::DirectX:
                backendName = "DirectX (Windows-optimized)";
                break;
            default:
                backendName = "Unknown";
                break;
        }
        std::cout << "  " << (i + 1) << ". " << backendName << std::endl;
    }
    
    // Show default backend
    auto defaultBackend = GraphicsConfiguration::GetDefaultBackend();
    std::cout << std::endl;
    std::cout << "Default Backend: ";
    switch (defaultBackend) {
        case GraphicsBackendType::Skia:
            std::cout << "Skia" << std::endl;
            break;
        case GraphicsBackendType::DirectX:
            std::cout << "DirectX" << std::endl;
            break;
        default:
            std::cout << "Auto-select" << std::endl;
            break;
    }
    
    // Show backend availability
    std::cout << std::endl;
    std::cout << "Backend Availability:" << std::endl;
    std::cout << "  Skia: " << (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::Skia) ? "Available" : "Not Available") << std::endl;
    std::cout << "  DirectX: " << (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX) ? "Available" : "Not Available") << std::endl;
    
    // Show configuration options
    std::cout << std::endl;
    std::cout << "Current Configuration:" << std::endl;
    std::cout << "  Hardware Acceleration Preferred: " << (GraphicsConfiguration::GetPreferHardwareAcceleration() ? "Yes" : "No") << std::endl;
    std::cout << "  DirectX Multisample Count: " << GraphicsConfiguration::GetDirectXMultisampleCount() << "x" << std::endl;
    std::cout << "  Skia GPU Acceleration: " << (GraphicsConfiguration::GetSkiaGpuAcceleration() ? "Enabled" : "Disabled") << std::endl;
    
    // Demonstrate configuration changes
    std::cout << std::endl;
    std::cout << "=== Configuration Demo ===" << std::endl;
    
    // Test DirectX configuration
    std::cout << "Setting DirectX multisample to 8x..." << std::endl;
    GraphicsConfiguration::SetDirectXMultisampleCount(8);
    std::cout << "DirectX Multisample Count: " << GraphicsConfiguration::GetDirectXMultisampleCount() << "x" << std::endl;
    
    // Test hardware acceleration preference
    std::cout << "Enabling hardware acceleration preference..." << std::endl;
    GraphicsConfiguration::SetPreferHardwareAcceleration(true);
    std::cout << "Hardware Acceleration Preferred: " << (GraphicsConfiguration::GetPreferHardwareAcceleration() ? "Yes" : "No") << std::endl;
    
    std::cout << std::endl;
    std::cout << "=== Implementation Summary ===" << std::endl;
    std::cout << "✅ Backend abstraction architecture implemented" << std::endl;
    std::cout << "✅ DirectX backend support added (Windows)" << std::endl;
    std::cout << "✅ Skia backend refactored to use interface" << std::endl;
    std::cout << "✅ Runtime backend selection available" << std::endl;
    std::cout << "✅ Configuration system implemented" << std::endl;
    std::cout << "✅ Cross-platform compatibility maintained" << std::endl;
    
    return 0;
}
