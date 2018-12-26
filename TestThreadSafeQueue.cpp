#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/ThreadSafeQueue.h"

using utils::ThreadSafeQueue;

TEST(TestThreadSafeQueue, pushAndPop__OneThread__Successfull) {
    ThreadSafeQueue<int> q;
    q.push(10);
    EXPECT_EQ(10, q.back());

    q.push(20);
    EXPECT_EQ(20, q.back());

    q.pop();
    q.pop();

    EXPECT_EQ(0, q.size());
}

TEST(TestThreadSafeQueue, tryPop__OneThreadSomethingToPop__PopsCorrectly) {
    ThreadSafeQueue<int> q;
    q.push(10);
    q.push(20);

    auto result = q.try_pop();

    EXPECT_EQ(10, result.value());
    EXPECT_EQ(20, q.front());
}

TEST(TestThreadSafeQueue, tryPop__OneThreadNothingToPop__ReturnsOptional) {
    ThreadSafeQueue<int> q;

    EXPECT_EQ(boost::none, q.try_pop());
}