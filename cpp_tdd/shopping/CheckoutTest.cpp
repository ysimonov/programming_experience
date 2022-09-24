#include <gtest/gtest.h>

#include <iostream>

#include "Checkout.h"

class CheckoutTests : public ::testing::Test {
   public:
   protected:
    Checkout checkOut;
};

// test fixture that can access checkOut instance of the class Checkout
TEST_F(CheckoutTests, CanCalculateTotal) {
    checkOut.addItemPrice("a", 1);
    checkOut.addItem("a");
    int total = checkOut.calculateTotal();
    ASSERT_EQ(total, 1);
}

TEST_F(CheckoutTests, CanGetTotalForMultipleItems) {
    checkOut.addItemPrice("a", 1);
    checkOut.addItemPrice("b", 2);
    checkOut.addItem("a");
    checkOut.addItem("b");
    int total = checkOut.calculateTotal();
    ASSERT_EQ(total, 3);
}

TEST_F(CheckoutTests, CanAddDiscount) { checkOut.addDiscount("a", 3, 2); }

TEST_F(CheckoutTests, CanCalculateTotalWithDiscount) {
    checkOut.addItemPrice("a", 1);
    checkOut.addDiscount("a", 3, 2);
    checkOut.addItem("a");
    checkOut.addItem("a");
    checkOut.addItem("a");
    int total = checkOut.calculateTotal();
    ASSERT_EQ(total, 2);
}

TEST_F(CheckoutTests, ItemWithNoPriceThrowsException) {
    ASSERT_THROW(checkOut.addItem("a"), std::invalid_argument);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}