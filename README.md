# Multithreading Basics and Task Based Programming

## Overview

This project benchmarks multithreading techniques in C++ by comparing:

- Raw `std::thread` parallelism
- Task-based parallelism with `std::async`
- Thread creation/destruction overhead
- Thread pool efficiency using a custom `ThreadPool`

The goal is to analyze the performance trade-offs between frequent thread creation and thread reuse via pooling, especially for compute-bound tasks like summing a large array in parallel.

---

## Build & Run

```bash
# Clone the repository
git clone https://github.com/AniDashyan/multithreading_and_tasks.git
cd multithreading_and_tasks

# Build the project
cmake -S . -B build
cmake --build build

# Run the executable
./build/main
````

## Example Output

```
[ Benchmark: std::thread scaling ]

[ std::thread Benchmark ]
std::thread: 1, Time: 1619300 ns, Sum: 1000000
std::thread: 2, Time: 494800 ns, Sum: 1000000
std::thread: 3, Time: 429300 ns, Sum: 1000000
std::thread: 4, Time: 536600 ns, Sum: 1000000
std::thread: 5, Time: 865900 ns, Sum: 1000000
std::thread: 6, Time: 560500 ns, Sum: 1000000
std::thread: 7, Time: 630300 ns, Sum: 1000000
std::thread: 8, Time: 647500 ns, Sum: 1000000

[ std::async Benchmark ]
std::async: 1, Time: 471600 ns, Sum: 1000000
std::async: 2, Time: 337900 ns, Sum: 1000000
std::async: 3, Time: 843800 ns, Sum: 1000000
std::async: 4, Time: 748100 ns, Sum: 1000000
std::async: 5, Time: 684200 ns, Sum: 1000000
std::async: 6, Time: 621100 ns, Sum: 1000000
std::async: 7, Time: 1535900 ns, Sum: 1000000
std::async: 8, Time: 1361100 ns, Sum: 1000000

[ Benchmark: Thread creation overhead ]
Thread creation/destruction time for 100000 iterations: 9.77219 ns

[ Benchmark: ThreadPool with 8 workers ]

[ std::thread Benchmark ]
std::thread: 1, Time: 529300 ns, Sum: 1000000
std::thread: 2, Time: 475400 ns, Sum: 1000000
std::thread: 3, Time: 505300 ns, Sum: 1000000
std::thread: 4, Time: 736700 ns, Sum: 1000000
std::thread: 5, Time: 655000 ns, Sum: 1000000
std::thread: 6, Time: 545100 ns, Sum: 1000000
std::thread: 7, Time: 761900 ns, Sum: 1000000
std::thread: 8, Time: 1422300 ns, Sum: 1000000
```

## How does it work?

### 1. Parallel Sum with `std::thread`

Splits the input data into equal-sized chunks and assigns each chunk to a separate thread that computes a partial sum. Threads are joined and partials summed at the end. Overhead increases with thread count due to creation cost.

### 2. `std::async`
Uses standard library facilities to asynchronously launch tasks, simplifying concurrency management.

### 3. Thread Creation Overhead

Measures how much time it takes to create and destroy empty threads repeatedly. This illustrates the fixed cost of thread management in C++.

### 4. Thread Pool Sum

Implements a `ThreadPool` to reuse a fixed number of worker threads. Tasks are enqueued to the pool, and each worker processes its share of the input data. This minimizes creation overhead and improves responsiveness for repeated task execution.
