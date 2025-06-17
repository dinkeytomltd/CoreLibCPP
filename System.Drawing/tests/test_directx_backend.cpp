
#ifdef _WIN32

#include <gtest/gtest.h>
#include "System/Drawing/GraphicsConfiguration.h"
#include "src/DirectXBackend/DirectXBackend.h"

using namespace System::Drawing;

class DirectXBackendTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!GraphicsConfiguration::IsBackendAvailable(GraphicsBackendType::DirectX)) {
            GTEST_SKIP() << "DirectX backend not available";
        }
        
        m_backend = std::make_unique<DirectXBackend>();
    }
    
    std::unique_ptr<DirectXBackend> m_backend;
};

TEST_F(DirectXBackendTest, Initialization) {
    EXPECT_NE(m_backend, nullptr);
    // TODO: Test proper initialization
}

TEST_F(DirectXBackendTest, HardwareAcceleration) {
    // TODO: Test hardware acceleration detection
    EXPECT_TRUE(m_backend->IsHardwareAccelerated());
}

TEST_F(DirectXBackendTest, BasicDrawing) {
    // TODO: Test basic drawing operations
    EXPECT_TRUE(true);  // Placeholder
}

TEST_F(DirectXBackendTest, StateManagement) {
    // TODO: Test graphics state management
    EXPECT_TRUE(true);  // Placeholder
}

#endif // _WIN32
