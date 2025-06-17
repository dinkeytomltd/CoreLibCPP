
#include "System/Drawing/Color.h"
#include <gtest/gtest.h>

using namespace System::Drawing;

TEST(ColorTest, DefaultConstructor) {
    Color color;
    EXPECT_EQ(color.A, 255);
    EXPECT_EQ(color.R, 0);
    EXPECT_EQ(color.G, 0);
    EXPECT_EQ(color.B, 0);
}

TEST(ColorTest, RGBConstructor) {
    Color color(255, 128, 64);
    EXPECT_EQ(color.A, 255);
    EXPECT_EQ(color.R, 255);
    EXPECT_EQ(color.G, 128);
    EXPECT_EQ(color.B, 64);
}

TEST(ColorTest, ARGBConstructor) {
    Color color(128, 255, 128, 64);
    EXPECT_EQ(color.A, 128);
    EXPECT_EQ(color.R, 255);
    EXPECT_EQ(color.G, 128);
    EXPECT_EQ(color.B, 64);
}

TEST(ColorTest, FromArgb) {
    Color color = Color::FromArgb(0x80FF8040);
    EXPECT_EQ(color.A, 0x80);
    EXPECT_EQ(color.R, 0xFF);
    EXPECT_EQ(color.G, 0x80);
    EXPECT_EQ(color.B, 0x40);
}

TEST(ColorTest, ToArgb) {
    Color color(128, 255, 128, 64);
    int argb = color.ToArgb();
    EXPECT_EQ(argb, 0x80FF8040);
}

TEST(ColorTest, NamedColors) {
    EXPECT_EQ(Color::Red.R, 255);
    EXPECT_EQ(Color::Red.G, 0);
    EXPECT_EQ(Color::Red.B, 0);
    
    EXPECT_EQ(Color::Green.R, 0);
    EXPECT_EQ(Color::Green.G, 128);
    EXPECT_EQ(Color::Green.B, 0);
    
    EXPECT_EQ(Color::Blue.R, 0);
    EXPECT_EQ(Color::Blue.G, 0);
    EXPECT_EQ(Color::Blue.B, 255);
}

TEST(ColorTest, Equality) {
    Color color1(255, 128, 64);
    Color color2(255, 128, 64);
    Color color3(255, 128, 65);
    
    EXPECT_TRUE(color1 == color2);
    EXPECT_FALSE(color1 == color3);
    EXPECT_TRUE(color1 != color3);
}
