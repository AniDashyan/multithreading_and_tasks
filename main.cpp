#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <atomic>

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
              << time.count() << " ms\n";
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

void benchmark_parallel_sum(const std::vector<int>& data) {
    for (int threads = 1; threads <= 8; ++threads) {
        auto start = std::chrono::steady_clock::now();
        int total = parallel_sum(data, threads);
        auto end = std::chrono::steady_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "Threads: " << threads
                  << ", Time: " << duration << " ms"
                  << ", Sum: " << total << "\n";
    }
}

void benchmark_threadpool(const std::vector<int>& data) {
    int threads = 8;
    auto start = std::chrono::steady_clock::now();
    int total = threadpool_sum(data, threads);
    auto end = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "ThreadPool Time: " << duration << " ms"
              << ", Sum: " << total << "\n";
}

int main() {
    const size_t N = 1'000'000;
    std::vector<int> data(N, 1);

    std::cout << "\n[ Benchmark: std::thread scaling ]\n";
    benchmark_parallel_sum(data);

    std::cout << "\n[ Benchmark: Thread creation overhead ]\n";
    measure_thread_overhead(100'000);

    std::cout << "\n[ Benchmark: ThreadPool with " << 8 << " workers ]\n";
    benchmark_threadpool(data);

    return 0;
}