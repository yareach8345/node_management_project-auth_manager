//
// Created by yareach on 26. 6. 11..
//

#include <gtest/gtest.h>

TEST(TestTest, Sample1) {
    constexpr int a = 10;
    constexpr int b = 20;

    EXPECT_EQ(a + b, 30);
    EXPECT_NE(a + b, 29);
}

TEST(TestTest, Sample2) {
    constexpr int a = 10;
    constexpr int da = a * 2;

    EXPECT_LT(da, 25);
}