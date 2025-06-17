
#include "System/Drawing/Rectangle.h"
#include <gtest/gtest.h>

using namespace System::Drawing;

TEST(RectangleTest, DefaultConstructor) {
    Rectangle rect;
    EXPECT_EQ(rect.X, 0);
    EXPECT_EQ(rect.Y, 0);
    EXPECT_EQ(rect.Width, 0);
    EXPECT_EQ(rect.Height, 0);
}

TEST(RectangleTest, ParameterizedConstructor) {
    Rectangle rect(10, 20, 100, 200);
    EXPECT_EQ(rect.X, 10);
    EXPECT_EQ(rect.Y, 20);
    EXPECT_EQ(rect.Width, 100);
    EXPECT_EQ(rect.Height, 200);
}

TEST(RectangleTest, LocationAndSize) {
    Point location(10, 20);
    Size size(100, 200);
    Rectangle rect(location, size);
    
    EXPECT_EQ(rect.X, 10);
    EXPECT_EQ(rect.Y, 20);
    EXPECT_EQ(rect.Width, 100);
    EXPECT_EQ(rect.Height, 200);
}

TEST(RectangleTest, Properties) {
    Rectangle rect(10, 20, 100, 200);
    
    EXPECT_EQ(rect.Left(), 10);
    EXPECT_EQ(rect.Top(), 20);
    EXPECT_EQ(rect.Right(), 110);
    EXPECT_EQ(rect.Bottom(), 220);
}

TEST(RectangleTest, Contains) {
    Rectangle rect(10, 20, 100, 200);
    
    EXPECT_TRUE(rect.Contains(50, 100));
    EXPECT_TRUE(rect.Contains(Point(50, 100)));
    EXPECT_FALSE(rect.Contains(5, 100));
    EXPECT_FALSE(rect.Contains(Point(150, 100)));
}

TEST(RectangleTest, IntersectsWith) {
    Rectangle rect1(10, 20, 100, 200);
    Rectangle rect2(50, 100, 100, 200);
    Rectangle rect3(200, 300, 100, 200);
    
    EXPECT_TRUE(rect1.IntersectsWith(rect2));
    EXPECT_FALSE(rect1.IntersectsWith(rect3));
}

TEST(RectangleTest, Union) {
    Rectangle rect1(10, 20, 100, 200);
    Rectangle rect2(50, 100, 100, 200);
    Rectangle result = Rectangle::Union(rect1, rect2);
    
    EXPECT_EQ(result.X, 10);
    EXPECT_EQ(result.Y, 20);
    EXPECT_EQ(result.Width, 140);
    EXPECT_EQ(result.Height, 280);
}

TEST(RectangleTest, Intersect) {
    Rectangle rect1(10, 20, 100, 200);
    Rectangle rect2(50, 100, 100, 200);
    Rectangle result = Rectangle::Intersect(rect1, rect2);
    
    EXPECT_EQ(result.X, 50);
    EXPECT_EQ(result.Y, 100);
    EXPECT_EQ(result.Width, 60);
    EXPECT_EQ(result.Height, 120);
}
