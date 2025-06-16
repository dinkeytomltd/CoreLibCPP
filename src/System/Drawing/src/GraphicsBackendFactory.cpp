
#include "System/Drawing/IGraphicsBackend.h"
#include "System/Drawing/GraphicsConfiguration.h"
#include <memory>

// Forward declarations to avoid circular dependencies
namespace System { namespace Drawing {
    class SkiaBackend;
#ifdef _WIN32
    class DirectXBackend;
    class GdiPlusBackend;
#endif
}}

#ifdef _WIN32
#include "DirectXBackend/DirectXBackend.h"
#include "GdiPlusBackend/GdiPlusBackend.h"
#endif

#include "SkiaBackend/SkiaBackend.h"

namespace System {
namespace Drawing {

// Backend factory
class GraphicsBackendFactory {
public:
    static std::unique_ptr<IGraphicsBackend> Create(
        GraphicsBackendType type = GraphicsBackendType::Auto,
        void* deviceContext = nullptr
    ) {
        if (type == GraphicsBackendType::Auto) {
            type = GraphicsConfiguration::GetDefaultBackend();
        }
        
        switch (type) {
#ifdef _WIN32
            case GraphicsBackendType::DirectX:
                if (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
                    return std::make_unique<DirectXBackend>();
                }
                // Fall through to GDI+ if DirectX not available
                [[fallthrough]];
            case GraphicsBackendType::GdiPlus:
                if (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::GdiPlus)) {
                    return std::make_unique<GdiPlusBackend>();
                }
                // Fall through to Skia if GDI+ not available
                [[fallthrough]];
#endif
            case GraphicsBackendType::Skia:
            default:
                return std::make_unique<SkiaBackend>();
        }
    }
    
    static bool IsBackendAvailable(GraphicsBackendType type) {
        switch (type) {
            case GraphicsBackendType::Skia:
                return true;  // Always available
#ifdef _WIN32
            case GraphicsBackendType::DirectX:
                // TODO: Check for DirectX availability
                return true;  // Assume available on Windows for now
            case GraphicsBackendType::GdiPlus:
                return true;  // GDI+ is built into Windows
#endif
            default:
                return false;
        }
    }
    
    static GraphicsBackendType GetDefaultBackend() {
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
};

}
}
