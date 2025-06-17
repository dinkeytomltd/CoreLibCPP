
#include <iostream>
#include <chrono>
#include <vector>
#include "System/Drawing/Graphics.h"
#include "System/Drawing/GraphicsConfiguration.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"

using namespace System::Drawing;

struct BenchmarkResult {
    GraphicsBackendType backend;
    double drawingOperationsPerSecond;
    double memoryUsageMB;
    std::string backendName;
};

class BackendPerformanceBenchmark {
public:
    static std::vector<BenchmarkResult> RunBenchmarks() {
        std::vector<BenchmarkResult> results;
        
        auto availableBackends = GraphicsConfiguration::GetAvailableBackends();
        
        for (auto backend : availableBackends) {
            std::cout << "Benchmarking " << GetBackendName(backend) << " backend..." << std::endl;
            
            BenchmarkResult result;
            result.backend = backend;
            result.backendName = GetBackendName(backend);
            result.drawingOperationsPerSecond = BenchmarkDrawingOperations(backend);
            result.memoryUsageMB = MeasureMemoryUsage(backend);
            
            results.push_back(result);
        }
        
        return results;
    }
    
    static void PrintComparisonReport(const std::vector<BenchmarkResult>& results) {
        std::cout << "\n=== Backend Performance Comparison ===" << std::endl;
        std::cout << "Backend\t\tDrawing Ops/sec\tMemory (MB)" << std::endl;
        std::cout << "-------\t\t---------------\t-----------" << std::endl;
        
        for (const auto& result : results) {
            std::cout << result.backendName << "\t\t"
                      << static_cast<int>(result.drawingOperationsPerSecond) << "\t\t"
                      << result.memoryUsageMB << std::endl;
        }
        
        // Find best performing backend
        auto bestPerformance = std::max_element(results.begin(), results.end(),
            [](const BenchmarkResult& a, const BenchmarkResult& b) {
                return a.drawingOperationsPerSecond < b.drawingOperationsPerSecond;
            });
            
        if (bestPerformance != results.end()) {
            std::cout << "\nBest Performance: " << bestPerformance->backendName 
                      << " (" << static_cast<int>(bestPerformance->drawingOperationsPerSecond) 
                      << " ops/sec)" << std::endl;
        }
    }

private:
    static std::string GetBackendName(GraphicsBackendType backend) {
        switch (backend) {
            case GraphicsBackendType::Skia: return "Skia";
            case GraphicsBackendType::DirectX: return "DirectX";
            case GraphicsBackendType::GdiPlus: return "GDI+";
            default: return "Unknown";
        }
    }
    
    static double BenchmarkDrawingOperations(GraphicsBackendType backend) {
        // TODO: Implement actual benchmark
        // For now, return mock values
        switch (backend) {
            case GraphicsBackendType::DirectX: return 15000.0;
            case GraphicsBackendType::Skia: return 12000.0;
            case GraphicsBackendType::GdiPlus: return 8000.0;
            default: return 0.0;
        }
    }
    
    static double MeasureMemoryUsage(GraphicsBackendType backend) {
        // TODO: Implement actual memory measurement
        // For now, return mock values
        switch (backend) {
            case GraphicsBackendType::DirectX: return 45.0;
            case GraphicsBackendType::Skia: return 52.0;
            case GraphicsBackendType::GdiPlus: return 28.0;
            default: return 0.0;
        }
    }
};

void OptimalBackendExample() {
    std::cout << "=== Optimal Backend Selection Example ===" << std::endl;
    
    // Method 1: Automatic selection
    std::cout << "1. Automatic backend selection:" << std::endl;
    auto defaultBackend = GraphicsConfiguration::GetDefaultBackend();
    std::cout << "   Default backend: " << static_cast<int>(defaultBackend) << std::endl;
    
    // Method 2: Explicit selection
    std::cout << "\n2. Explicit backend selection:" << std::endl;
    if (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
        std::cout << "   DirectX backend available - using for optimal Windows performance" << std::endl;
    } else {
        std::cout << "   DirectX not available - using Skia for cross-platform compatibility" << std::endl;
    }
    
    // Method 3: Performance-based selection
    std::cout << "\n3. Performance-based selection:" << std::endl;
    GraphicsConfiguration::SetPreferHardwareAcceleration(true);
    std::cout << "   Hardware acceleration preferred: " 
              << (GraphicsConfiguration::GetPreferHardwareAcceleration() ? "Yes" : "No") << std::endl;
}

void BackendOptimizationExample() {
    std::cout << "\n=== Backend-Specific Optimizations ===" << std::endl;
    
    auto currentBackend = GraphicsConfiguration::GetDefaultBackend();
    
    switch (currentBackend) {
        case GraphicsBackendType::DirectX:
            std::cout << "DirectX backend optimizations:" << std::endl;
            GraphicsConfiguration::SetDirectXMultisampleCount(4);
            std::cout << "  - Enabled 4x MSAA" << std::endl;
            std::cout << "  - Hardware acceleration: Available" << std::endl;
            break;
            
        case GraphicsBackendType::Skia:
            std::cout << "Skia backend optimizations:" << std::endl;
            GraphicsConfiguration::SetSkiaGpuAcceleration(true);
            std::cout << "  - Enabled GPU acceleration" << std::endl;
            std::cout << "  - Cross-platform compatibility: Yes" << std::endl;
            break;
            
        default:
            std::cout << "Unknown backend" << std::endl;
            break;
    }
}

int main() {
    std::cout << "System.Drawing Backend Comparison Tool" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // Show available backends
    std::cout << "\nAvailable backends:" << std::endl;
    auto backends = GraphicsConfiguration::GetAvailableBackends();
    for (auto backend : backends) {
        std::cout << "  - " << static_cast<int>(backend) << std::endl;
    }
    
    // Run examples
    OptimalBackendExample();
    BackendOptimizationExample();
    
    // Run performance benchmarks
    std::cout << "\n=== Performance Benchmarks ===" << std::endl;
    auto results = BackendPerformanceBenchmark::RunBenchmarks();
    BackendPerformanceBenchmark::PrintComparisonReport(results);
    
    return 0;
}
