#include <gtest/gtest.h>

#include <string>

std::string fizzBuzz(int value);

void checkFizzBuzz(int value, std::string expectedResult) {
    std::string result = fizzBuzz(value);
    ASSERT_STREQ(expectedResult.c_str(), result.c_str());
}

TEST(FizzBuzzTest, returns1With1PassedIn) { checkFizzBuzz(1, "1"); }
TEST(FizzBuzzTest, returns2With2PassedIn) { checkFizzBuzz(2, "2"); }
TEST(FizzBuzzTest, returnsFizzWith3PassedIn) { checkFizzBuzz(3, "Fizz"); }
TEST(FizzBuzzTest, returnsBuzzWith5PassedIn) { checkFizzBuzz(5, "Buzz"); }
TEST(FizzBuzzTest, returnsFizzWith6PassedIn) { checkFizzBuzz(6, "Fizz"); }
TEST(FizzBuzzTest, returnsBuzzWith10PassedIn) { checkFizzBuzz(10, "Buzz"); }
TEST(FizzBuzzTest, returnsFizzBuzzWith15PassedIn) {
    checkFizzBuzz(15, "FizzBuzz");
}

inline bool isMultiple(const int& value, const int& base) {
    return value % base == 0;
}

std::string fizzBuzz(int value) {
    if (isMultiple(value, 3) && isMultiple(value, 5)) {
        return "FizzBuzz";
    } else if (isMultiple(value, 3)) {
        return "Fizz";
    } else if (isMultiple(value, 5)) {
        return "Buzz";
    }
    return std::to_string(value);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}