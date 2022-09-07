#include <gtest/gtest.h>

#include "what_to_test.cpp"

TEST(SquareRootTest, PositiveNos) {
    ASSERT_EQ(6, squareRoot(36.0));
    ASSERT_EQ(18.0, squareRoot(324.0));
    ASSERT_EQ(0, squareRoot(0.0));
}

TEST(SquareRootTest, NegativeNos) {
    EXPECT_EQ(-1.0, squareRoot(1.0));
}

TEST(FactorialTest, PositiveNos) {
    ASSERT_EQ(3628800, factorial(10));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}