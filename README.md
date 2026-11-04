# Pure C++ Parallel Profiler
Benchmarking utility written in pure C++ to compare the execution time of single threaded vs. multi threaded sorting.

## How it Works
The program runs a comparative experiment on your hardware:
1. It generates a massive array of randomized integers.
2. It benchmarks a single-threaded baseline sort using `std::sort` (Introsort) and records the execution duration.
3. It resets the array, calculates slices based on custom configuration, and forks the data chunks across multiple parallel `std::thread` workers.
4. The main thread synchronizes the workers via a `.join()` barrier and executes a zero-copy cascading `std::inplace_merge` to stitch the datasets back together.
5. It output the baseline and threaded execution duration.

## How to Test Your Own Hardware
You can explicitly configure the test parameters directly inside the code
1. Open `profiler.cpp`.
2. Locate the **USER CONFIGURATION ZONE** at the very top of `main()`.
3. Modify `VECTOR_SIZE` and `NUM_THREADS` to any parameters you want to test.

##  Compilation and Execution
```bash
g++ -O3 -std=c++11 profiler.cpp -o profiler
./profiler
```
