
#include <gtest/gtest.h>
#include "System/Drawing/Graphics.h"
#include "System/Drawing/GraphicsConfiguration.h"
#include "System/Drawing/IGraphicsBackend.h"

using namespace System::Drawing;

class BackendSelectionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset to default configuration
        GraphicsConfiguration::SetDefaultBackend(GraphicsBackendType::Auto);
    }
};

TEST_F(BackendSelectionTest, AvailableBackends) {
    auto backends = GraphicsConfiguration::GetAvailableBackends();
    
    // Skia should always be available
    EXPECT_TRUE(std::find(backends.begin(), backends.end(), GraphicsBackendType::Skia) != backends.end());
    
#ifdef _WIN32
    // DirectX should be available on Windows
    EXPECT_TRUE(std::find(backends.begin(), backends.end(), GraphicsBackendType::DirectX) != backends.end());
#endif
}

TEST_F(BackendSelectionTest, DefaultBackendSelection) {
    auto defaultBackend = GraphicsConfiguration::GetDefaultBackend();
    
#ifdef _WIN32
    // On Windows, DirectX should be preferred if available
    if (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
        EXPECT_EQ(defaultBackend, GraphicsBackendType::DirectX);
    } else {
        EXPECT_EQ(defaultBackend, GraphicsBackendType::Skia);
    }
#else
    // On other platforms, Skia should be default
    EXPECT_EQ(defaultBackend, GraphicsBackendType::Skia);
#endif
}

TEST_F(BackendSelectionTest, ExplicitBackendSelection) {
    // Test explicit Skia selection
    GraphicsConfiguration::SetDefaultBackend(GraphicsBackendType::Skia);
    EXPECT_EQ(GraphicsConfiguration::GetDefaultBackend(), GraphicsBackendType::Skia);
    
#ifdef _WIN32
    // Test explicit DirectX selection on Windows
    if (GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
        GraphicsConfiguration::SetDefaultBackend(GraphicsBackendType::DirectX);
        EXPECT_EQ(GraphicsConfiguration::GetDefaultBackend(), GraphicsBackendType::DirectX);
    }
#endif
}

TEST_F(BackendSelectionTest, BackendAvailability) {
    // Skia should always be available
    EXPECT_TRUE(GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::Skia));
    
#ifdef _WIN32
    // DirectX should be available on Windows
    EXPECT_TRUE(GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX));
#else
    // DirectX should not be available on non-Windows platforms
    EXPECT_FALSE(GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX));
#endif
}

TEST_F(BackendSelectionTest, ConfigurationSettings) {
    // Test hardware acceleration preference
    GraphicsConfiguration::SetPreferHardwareAcceleration(true);
    EXPECT_TRUE(GraphicsConfiguration::GetPreferHardwareAcceleration());
    
    GraphicsConfiguration::SetPreferHardwareAcceleration(false);
    EXPECT_FALSE(GraphicsConfiguration::GetPreferHardwareAcceleration());
    
    // Test DirectX multisample count
    GraphicsConfiguration::SetDirectXMultisampleCount(8);
    EXPECT_EQ(GraphicsConfiguration::GetDirectXMultisampleCount(), 8);
    
    // Test Skia GPU acceleration
    GraphicsConfiguration::SetSkiaGpuAcceleration(false);
    EXPECT_FALSE(GraphicsConfiguration::GetSkiaGpuAcceleration());
}

// Backend compatibility tests
class BackendCompatibilityTest : public ::testing::TestWithParam<GraphicsBackendType> {
protected:
    void SetUp() override {
        m_backendType = GetParam();
        if (!GraphicsConfiguration::IsBackendAvailable(m_backendType)) {
            GTEST_SKIP() << "Backend not available: " << static_cast<int>(m_backendType);
        }
        
        // TODO: Create Graphics instance with specific backend
        // m_graphics = std::make_shared<Graphics>(m_backendType);
    }
    
    GraphicsBackendType m_backendType;
    // std::shared_ptr<Graphics> m_graphics;
};

INSTANTIATE_TEST_SUITE_P(
    AllBackends,
    BackendCompatibilityTest,
    ::testing::Values(
        GraphicsBackendType::Skia
#ifdef _WIN32
        , GraphicsBackendType::DirectX
#endif
    )
);

TEST_P(BackendCompatibilityTest, BackendCreation) {
    // Test that backend can be created
    EXPECT_TRUE(GraphicsConfiguration::IsBackendAvailable(m_backendType));
}

// DirectX-specific tests
#ifdef _WIN32
TEST(DirectXBackendTest, HardwareAcceleration) {
    if (!GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
        GTEST_SKIP() << "DirectX backend not available";
    }
    
    // TODO: Test DirectX-specific features
    EXPECT_TRUE(true);  // Placeholder
}
#endif
