
#include "System/Drawing/Point.h"
#include <gtest/gtest.h>

using namespace System::Drawing;

TEST(PointTest, DefaultConstructor) {
    Point point;
    EXPECT_EQ(point.X, 0);
    EXPECT_EQ(point.Y, 0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point point(10, 20);
    EXPECT_EQ(point.X, 10);
    EXPECT_EQ(point.Y, 20);
}

TEST(PointTest, IsEmpty) {
    Point empty;
    Point nonEmpty(1, 0);
    
    EXPECT_TRUE(empty.IsEmpty());
    EXPECT_FALSE(nonEmpty.IsEmpty());
}

TEST(PointTest, Addition) {
    Point point(10, 20);
    Size size(5, 15);
    Point result = point + size;
    
    EXPECT_EQ(result.X, 15);
    EXPECT_EQ(result.Y, 35);
}

TEST(PointTest, Subtraction) {
    Point point(10, 20);
    Size size(5, 15);
    Point result = point - size;
    
    EXPECT_EQ(result.X, 5);
    EXPECT_EQ(result.Y, 5);
}

TEST(PointTest, Equality) {
    Point point1(10, 20);
    Point point2(10, 20);
    Point point3(10, 21);
    
    EXPECT_TRUE(point1 == point2);
    EXPECT_FALSE(point1 == point3);
    EXPECT_TRUE(point1 != point3);
}

TEST(PointFTest, DefaultConstructor) {
    PointF point;
    EXPECT_FLOAT_EQ(point.X, 0.0f);
    EXPECT_FLOAT_EQ(point.Y, 0.0f);
}

TEST(PointFTest, ParameterizedConstructor) {
    PointF point(10.5f, 20.7f);
    EXPECT_FLOAT_EQ(point.X, 10.5f);
    EXPECT_FLOAT_EQ(point.Y, 20.7f);
}

TEST(PointFTest, Distance) {
    PointF point1(0.0f, 0.0f);
    PointF point2(3.0f, 4.0f);
    float distance = PointF::Distance(point1, point2);
    
    EXPECT_FLOAT_EQ(distance, 5.0f);
}
