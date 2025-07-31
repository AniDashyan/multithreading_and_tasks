# Multithreading Basics and Task Based Programming

## Overview

This project benchmarks multithreading techniques in C++ by comparing:

- Raw `std::thread` parallelism
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
Threads: 1, Time: 3063 ms, Sum: 1000000
Threads: 2, Time: 1182 ms, Sum: 1000000
Threads: 3, Time: 860 ms, Sum: 1000000
Threads: 4, Time: 806 ms, Sum: 1000000
Threads: 5, Time: 655 ms, Sum: 1000000
Threads: 6, Time: 584 ms, Sum: 1000000
Threads: 7, Time: 804 ms, Sum: 1000000
Threads: 8, Time: 711 ms, Sum: 1000000

[ Benchmark: Thread creation overhead ]
Thread creation/destruction time for 100000 iterations: 8.86722 ms

[ Benchmark: ThreadPool with 8 workers ]
ThreadPool Time: 1301 ms, Sum: 1000000
```

## How does it work?

### 1. Parallel Sum with `std::thread`

Splits the input data into equal-sized chunks and assigns each chunk to a separate thread that computes a partial sum. Threads are joined and partials summed at the end. Overhead increases with thread count due to creation cost.

### 2. Thread Creation Overhead

Measures how much time it takes to create and destroy empty threads repeatedly. This illustrates the fixed cost of thread management in C++.

### 3. Thread Pool Sum

Implements a `ThreadPool` to reuse a fixed number of worker threads. Tasks are enqueued to the pool, and each worker processes its share of the input data. This minimizes creation overhead and improves responsiveness for repeated task execution.
