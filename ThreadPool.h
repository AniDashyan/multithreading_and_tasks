#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t num_threads) {
        running = true;
        for (size_t i = 0; i < num_threads; ++i) {
            workers.push_back(std::thread([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::lock_guard<std::mutex> lock(queue_mutex);
                        if (!tasks.empty()) {
                            task = std::move(tasks.front());
                            tasks.pop();
                        } else if (!running) {
                            break;
                        }
                    }

                    if (task) {
                        task();
                    } else {
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }
                }
            }));
        }
    }

    void enqueue(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        tasks.push(std::move(task));
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            running = false;
        }
        for (auto& worker : workers)
            worker.join();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    bool running = true;
};