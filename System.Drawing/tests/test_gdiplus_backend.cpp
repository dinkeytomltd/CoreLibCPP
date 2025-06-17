#ifdef SYSTEM_DRAWING_GDIPLUS_ENABLED

#include <gtest/gtest.h>
#include "System/Drawing/Graphics.h"
#include "System/Drawing/GraphicsConfiguration.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include "System/Drawing/Font.h"
#include "System/Drawing/Color.h"
#include "System/Drawing/Point.h"
#include "System/Drawing/Rectangle.h"

using namespace System::Drawing;

class GdiPlusBackendTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::GdiPlus)) {
            GTEST_SKIP() << "GDI+ backend not available";
        }
        // Note: For now, we'll test basic availability since full implementation is NYI
    }
};

TEST_F(GdiPlusBackendTest, BackendAvailability) {
    // Test that GDI+ backend is available on Windows
    #ifdef _WIN32
    EXPECT_TRUE(GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::GdiPlus));
    #else
    EXPECT_FALSE(GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::GdiPlus));
    #endif
}

TEST_F(GdiPlusBackendTest, BackendInAvailableList) {
    auto backends = GraphicsConfiguration::GetAvailableBackends();
    
    #ifdef _WIN32
    // On Windows, GDI+ should be in the available backends list
    bool found = false;
    for (auto backend : backends) {
        if (backend == GraphicsBackendType::GdiPlus) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found) << "GDI+ backend should be available on Windows";
    #endif
}

TEST_F(GdiPlusBackendTest, ConfigurationSettings) {
    // Test GDI+ configuration settings
    GraphicsConfiguration::SetGdiPlusTextRenderingHint(4); // AntiAlias
    EXPECT_EQ(GraphicsConfiguration::GetGdiPlusTextRenderingHint(), 4);
    
    GraphicsConfiguration::SetGdiPlusSmoothingMode(2); // HighQuality
    EXPECT_EQ(GraphicsConfiguration::GetGdiPlusSmoothingMode(), 2);
}

// Cross-backend compatibility tests
class AllBackendsTest : public ::testing::TestWithParam<GraphicsBackendType> {
protected:
    void SetUp() override {
        m_backendType = GetParam();
        if (!GraphicsConfiguration::IsBackendAvailable(m_backendType)) {
            GTEST_SKIP() << "Backend not available: " << static_cast<int>(m_backendType);
        }
    }
    
    GraphicsBackendType m_backendType;
};

INSTANTIATE_TEST_SUITE_P(
    AllBackends,
    AllBackendsTest,
    ::testing::Values(
        GraphicsBackendType::Skia
#ifdef _WIN32
        , GraphicsBackendType::DirectX
        , GraphicsBackendType::GdiPlus
#endif
    )
);

TEST_P(AllBackendsTest, BackendEnumeration) {
    // Test that all backends can be enumerated
    EXPECT_TRUE(GraphicsConfiguration::IsBackendAvailable(m_backendType));
}

TEST_P(AllBackendsTest, DefaultBackendSelection) {
    // Test that default backend selection works
    auto defaultBackend = GraphicsConfiguration::GetDefaultBackend();
    EXPECT_NE(defaultBackend, GraphicsBackendType::Auto);
}

#endif // SYSTEM_DRAWING_GDIPLUS_ENABLED
