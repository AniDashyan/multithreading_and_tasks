#include <iostream>
#include <numeric>
#include <chrono>
#include <atomic>
#include <future>

#include "ThreadPool.h"

void partial_sum(const std::vector<int>& data, size_t start, size_t end, int& result) {
    result = std::accumulate(data.begin() + start, data.begin() + end, 0);
}

int parallel_sum(const std::vector<int>& data, int num_threads) {
    std::vector<std::thread> threads;
    std::vector<int> partials(num_threads, 0);
    size_t chunk_size = data.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? data.size() : start + chunk_size;
        threads.push_back(std::thread(partial_sum, std::cref(data), start, end, std::ref(partials[i])));
    }

    for (auto& t : threads) t.join();
    return std::accumulate(partials.begin(), partials.end(), 0);
}

// ---------------------- THREAD CREATION OVERHEAD -----------------------------
void measure_thread_overhead(int iterations) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::thread([]() {}).join();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time = end - start;
    std::cout << "Thread creation/destruction time for " << iterations << " iterations: "
              << time.count() << " ns\n";
}


// -------- std::async-based parallel sum --------
int async_sum(const std::vector<int>& data, int num_tasks) {
    std::vector<std::future<int>> futures;
    size_t chunk_size = data.size() / num_tasks;

    for (int i = 0; i < num_tasks; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_tasks - 1) ? data.size() : start + chunk_size;
        futures.push_back(std::async(std::launch::async, [&, start, end]() {
            return std::accumulate(data.begin() + start, data.begin() + end, 0);
        }));
    }

    int total = 0;
    for (auto& f : futures) total += f.get();
    return total;
}

// ------------------ THREAD POOL PARTIAL SUM ----------------------------------
int threadpool_sum(const std::vector<int>& data, int num_threads) {
    ThreadPool pool(num_threads);
    std::vector<int> partials(num_threads, 0);
    size_t chunk_size = data.size() / num_threads;

    std::atomic<int> tasks_remaining(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? data.size() : start + chunk_size;
        pool.enqueue([&, i, start, end]() {
            partials[i] = std::accumulate(data.begin() + start, data.begin() + end, 0);
            --tasks_remaining;
        });
    }

    while (tasks_remaining > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    return std::accumulate(partials.begin(), partials.end(), 0);
}

template<typename Func>
void measure_time(const std::string& label, int from, int to, Func compute) {
    std::cout << "\n[ " << label << " Benchmark ]\n";
    for (int i = from; i <= to; ++i) {
        auto start = std::chrono::steady_clock::now();
        int total = compute(i);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << label << ": " << i
                  << ", Time: " << duration << " ns"
                  << ", Sum: " << total << "\n";
    }
}

int main() {
    const size_t N = 1'000'000;
    std::vector<int> data(N, 1);

    std::cout << "\n[ Benchmark: std::thread scaling ]\n";
    measure_time("std::thread", 1, 8, [&](int threads) {
        return parallel_sum(data, threads);
    });

    measure_time("std::async", 1, 8, [&](int tasks) {
        return async_sum(data, tasks);
    });

    std::cout << "\n[ Benchmark: Thread creation overhead ]\n";
    measure_thread_overhead(100'000);

    std::cout << "\n[ Benchmark: ThreadPool with " << 8 << " workers ]\n";
    measure_time("std::thread", 1, 8, [&](int threads) {
        return threadpool_sum(data, threads);
    });

    return 0;
}