#pragma once

#include <atomic>
#include <functional>
#include <stdexcept>
#include <thread>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "ThreadSafeQueue.h"

using namespace std::literals;

static constexpr auto MAX_NR_OF_THREADS = 4;

namespace utils {

    class ThreadPool {
    using Task = std::function<void(void)>;

    public:
        explicit ThreadPool(std::size_t number_of_threads)
          : _number_of_threads{number_of_threads} {
            if (number_of_threads > MAX_NR_OF_THREADS) {
                throw std::runtime_error(
                        "Can't create threadpool with more than: "s
                        + boost::lexical_cast<std::string>(MAX_NR_OF_THREADS) + " threads."s);
            }
        }

        void init() {
            _should_run = true;

            for (std::size_t i = 0; i < _number_of_threads; i++) {
                _threads.push_back(
                        std::thread([this]() {
                            this->event_loop();
                    }));
            }
        }

        void shutdown() {
            _should_run = false;

            for (auto&& t : _threads) {
                t.join();
            }
        }

        void add_task(Task task) {
            _queue.push(task);
        }

    private:
        std::size_t _number_of_threads;
        ThreadSafeQueue<Task> _queue;
        std::vector<std::thread> _threads;
        std::atomic<bool> _should_run = false;

        void event_loop() {
            while (!_queue.empty() || _should_run) {
                if (auto popped_task = _queue.try_pop(); popped_task != boost::none) {
                    (*popped_task)();
                }
            }
        }
    };

}