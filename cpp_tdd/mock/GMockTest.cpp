#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// g++ -lpthread -lgtest -lgmock GMockTest.o -o GMockTest

using ::testing::_;
using ::testing::Return;

class QueueInterface {
   public:
    virtual ~QueueInterface() { ; }
    virtual void enqueue(int data) = 0;
    virtual int dequeue() = 0;
};

class MockQueue : public QueueInterface {
   public:
    MOCK_METHOD0(dequeue, int());
    MOCK_METHOD1(enqueue, void(int data));
};

class DataHolder {
   public:
    DataHolder() { ; }
    DataHolder(QueueInterface *queue) : queue(queue) { ; }
    virtual ~DataHolder() { ; }

    void addData(int data) { queue->enqueue(data); }
    int getData() const { return queue->dequeue(); }

   protected:
    QueueInterface *queue = nullptr;
};

class GMockTests : public ::testing::Test, DataHolder {
   public:
    GMockTests() : dh(&myMockObj) { ; }
    ~GMockTests() { queue = nullptr; }

   protected:
    MockQueue myMockObj;
    DataHolder dh;
};

TEST_F(GMockTests, CanAddData) {
    // underscore accepts any data
    EXPECT_CALL(myMockObj, enqueue(_));
    dh.addData(1);
}

TEST_F(GMockTests, CanAddAndGetData) {
    EXPECT_CALL(myMockObj, enqueue(1));
    EXPECT_CALL(myMockObj, dequeue()).WillOnce(Return(1));
    dh.addData(1);
    int data = dh.getData();
    ASSERT_EQ(data, 1);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}