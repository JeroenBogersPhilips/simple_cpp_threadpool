#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "include/ThreadPool.h"

using namespace std::chrono_literals;
using utils::ThreadPool;

TEST(TestThreadPool, constructedWithSuitableNumberOfThreads__Succeeds) {
    EXPECT_NO_THROW(ThreadPool(3));
}

TEST(TestThreadPool, constructedWithTooManyDesiredThreads__Throws) {
    EXPECT_ANY_THROW(ThreadPool(10));
}

auto createLongRunningTask(std::string identifier, std::chrono::duration<int> duration) {
    auto result = [identifier = std::move(identifier), duration = std::move(duration)]() {
        std::cout << "Long running task with identifier: " + identifier + " started." << std::endl;
        std::this_thread::sleep_for(duration);
        std::cout << "Long running task with identifier: " + identifier + " finished." << std::endl;
    };

    return result;
}

struct IdAndDuration {
    std::string identifier;
    std::chrono::duration<int> duration;
};

static ThreadPool thread_pool(1);

TEST(TestThreadPool, basicFunctionalityWithFourThreads) {
    std::cout << "Init threadpool" << std::endl;
    thread_pool.init();

    std::vector<IdAndDuration> some_long_running_tasks_input {
       {"task1"s, 1s}, {"task2"s, 5s}, {"task3"s, 4s}, {"task4"s, 8s}, {"task5"s, 6s}
    };

    for (auto&& task_info : some_long_running_tasks_input) {
        auto long_running_task = createLongRunningTask(task_info.identifier, task_info.duration);
        thread_pool.add_task(long_running_task);
    }

    std::this_thread::sleep_for(10s);
    std::cout << "Trigger shutdown of threadpool" << std::endl;
    thread_pool.shutdown();
    std::cout << "Threadpool shutdown finished: all its tasks finished!" << std::endl;
}