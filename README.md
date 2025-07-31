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
[ std::thread Benchmark ]
std::thread (1 threads): 2838 us, Sum = 1000000
std::thread (2 threads): 881 us, Sum = 1000000
std::thread (3 threads): 585 us, Sum = 1000000
std::thread (4 threads): 1834 us, Sum = 1000000
std::thread (5 threads): 802 us, Sum = 1000000
std::thread (6 threads): 863 us, Sum = 1000000
std::thread (7 threads): 1014 us, Sum = 1000000
std::thread (8 threads): 845 us, Sum = 1000000
-> Average Time: 1207 us
-> Speedup over 1-thread: 2.34982x

[ std::async Benchmark ]
std::async (1 threads): 427 us, Sum = 1000000
std::async (2 threads): 483 us, Sum = 1000000
std::async (3 threads): 1818 us, Sum = 1000000
std::async (4 threads): 1460 us, Sum = 1000000
std::async (5 threads): 932 us, Sum = 1000000
std::async (6 threads): 1091 us, Sum = 1000000
std::async (7 threads): 1968 us, Sum = 1000000
std::async (8 threads): 835 us, Sum = 1000000
-> Average Time: 1126 us
-> Speedup over 1-thread: 0.378966x

[ Benchmark: Thread creation overhead ]
Thread creation/destruction time for 100000 iterations: 95137.9 us per thread

[ ThreadPool Benchmark ]
ThreadPool (1 threads): 676 us, Sum = 1000000
ThreadPool (2 threads): 703 us, Sum = 1000000
ThreadPool (3 threads): 549 us, Sum = 1000000
ThreadPool (4 threads): 489 us, Sum = 1000000
ThreadPool (5 threads): 1114 us, Sum = 1000000
ThreadPool (6 threads): 573 us, Sum = 1000000
ThreadPool (7 threads): 1052 us, Sum = 1000000
ThreadPool (8 threads): 6325 us, Sum = 1000000
-> Average Time: 1435 us
-> Speedup over 1-thread: 0.471039x
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
