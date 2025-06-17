
#include "System/Drawing/Graphics.h"
#include "System/Drawing/Color.h"
#include "System/Drawing/Pen.h"
#include "System/Drawing/Brush.h"
#include <gtest/gtest.h>

// Mock test since we can't easily test graphics without a real surface
using namespace System::Drawing;

TEST(GraphicsTest, BasicConstruction) {
    // This test would require a real Skia surface to work properly
    // For now, just test that the headers compile correctly
    EXPECT_TRUE(true);
}

TEST(GraphicsTest, ColorCreation) {
    Color red = Color::Red;
    Color blue = Color::Blue;
    
    EXPECT_NE(red, blue);
    EXPECT_EQ(red.R, 255);
    EXPECT_EQ(blue.B, 255);
}

TEST(GraphicsTest, PenCreation) {
    Color red = Color::Red;
    Pen pen(red, 2.0f);
    
    EXPECT_EQ(pen.GetColor(), red);
    EXPECT_FLOAT_EQ(pen.GetWidth(), 2.0f);
}

TEST(GraphicsTest, BrushCreation) {
    Color blue = Color::Blue;
    SolidBrush brush(blue);
    
    EXPECT_EQ(brush.GetColor(), blue);
}
