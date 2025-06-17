
#include "System/Drawing/Size.h"
#include <gtest/gtest.h>

using namespace System::Drawing;

TEST(SizeTest, DefaultConstructor) {
    Size size;
    EXPECT_EQ(size.Width, 0);
    EXPECT_EQ(size.Height, 0);
}

TEST(SizeTest, ParameterizedConstructor) {
    Size size(100, 200);
    EXPECT_EQ(size.Width, 100);
    EXPECT_EQ(size.Height, 200);
}

TEST(SizeTest, IsEmpty) {
    Size empty;
    Size nonEmpty(1, 0);
    
    EXPECT_TRUE(empty.IsEmpty());
    EXPECT_FALSE(nonEmpty.IsEmpty());
}

TEST(SizeTest, Addition) {
    Size size1(100, 200);
    Size size2(50, 75);
    Size result = size1 + size2;
    
    EXPECT_EQ(result.Width, 150);
    EXPECT_EQ(result.Height, 275);
}

TEST(SizeTest, Subtraction) {
    Size size1(100, 200);
    Size size2(50, 75);
    Size result = size1 - size2;
    
    EXPECT_EQ(result.Width, 50);
    EXPECT_EQ(result.Height, 125);
}

TEST(SizeTest, Equality) {
    Size size1(100, 200);
    Size size2(100, 200);
    Size size3(100, 201);
    
    EXPECT_TRUE(size1 == size2);
    EXPECT_FALSE(size1 == size3);
    EXPECT_TRUE(size1 != size3);
}

TEST(SizeFTest, DefaultConstructor) {
    SizeF size;
    EXPECT_FLOAT_EQ(size.Width, 0.0f);
    EXPECT_FLOAT_EQ(size.Height, 0.0f);
}

TEST(SizeFTest, ParameterizedConstructor) {
    SizeF size(100.5f, 200.7f);
    EXPECT_FLOAT_EQ(size.Width, 100.5f);
    EXPECT_FLOAT_EQ(size.Height, 200.7f);
}

TEST(SizeFTest, FromSize) {
    Size intSize(100, 200);
    SizeF floatSize(intSize);
    
    EXPECT_FLOAT_EQ(floatSize.Width, 100.0f);
    EXPECT_FLOAT_EQ(floatSize.Height, 200.0f);
}
