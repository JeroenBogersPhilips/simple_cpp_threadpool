#pragma once

#include <mutex>
#include <queue>

#include <boost/optional.hpp>

namespace utils {

    // Currently only works for copyable types..
    template <typename T>
    class ThreadSafeQueue {
    public:
        T& front() {
            std::lock_guard<std::mutex> l(_mutex);
            return _queue.front();
        }

        const T& front() const {
            std::lock_guard<std::mutex> l(_mutex);
            return _queue.front();
        }

        T& back() {
            std::lock_guard<std::mutex> l(_mutex);
            return _queue.back();
        }

        const T& back() const {
            std::lock_guard<std::mutex> l(_mutex);
            return _queue.back();
        }

        bool empty() const {
            std::lock_guard<std::mutex> l(_mutex);
            return _queue.empty();
        }

        std::size_t size() const {
            std::lock_guard<std::mutex> l(_mutex);
            return _queue.size();
        }

        void push(const T& value) {
            std::lock_guard<std::mutex> l(_mutex);
            _queue.push(value);
        }

        void push(T&& value) {
            std::lock_guard<std::mutex> l(_mutex);
            _queue.push(std::move(value));
        }

        void pop() {
            std::lock_guard<std::mutex> l(_mutex);
            _queue.pop();
        }

        boost::optional<T> try_pop() {
            std::lock_guard<std::mutex> l(_mutex);
            if (_queue.empty()) {
                return boost::none;
            } else {
                auto result = _queue.front();
                _queue.pop();
                return std::move(result);
            }
        }

    private:
        std::queue<T> _queue;
        mutable std::mutex _mutex;
    };

}  // namespace utils