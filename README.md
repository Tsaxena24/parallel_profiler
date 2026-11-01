# Pure C++ CPU Thread Cores & Memory Layout Profiler

A production-grade benchmarking utility written in pure C++ to experimentally discover the limits of parallel data processing, thread management overhead, and cache structures on any CPU architecture.

## How it Works
The program runs a comparative scientific experiment on your hardware:
1. It generates a massive array of randomized integers.
2. It benchmarks a single-threaded baseline sort using `std::sort` (Introsort) and records the execution duration.
3. It resets the array, calculates clean pointer slices based on your custom configuration, and forks the data chunks across multiple parallel `std::thread` workers.
4. Each thread records its own high-precision execution duration.
5. The main thread synchronizes the workers via a `.join()` barrier and executes a zero-copy cascading `std::inplace_merge` to stitch the datasets back together.

## How to Test Your Own Hardware
You can explicitly configure the test parameters directly inside the code! 
1. Open `main.cpp`.
2. Locate the **USER CONFIGURATION ZONE** at the very top of `main()`.
3. Modify `VECTOR_SIZE` and `NUM_THREADS` to any parameters you want to stress-test.

##  Compilation and Execution
To unleash true bare-metal performance, make sure to compile with maximum compiler optimizations (`-O3` flag):

```bash
g++ -O3 -std=c++11 main.cpp -o profiler
./profiler
```

##  Experiments to Try
* **Thread Creation Overhead:** Set `VECTOR_SIZE = 500` and `NUM_THREADS = 8`. Notice how the parallel run is significantly slower than the single thread due to OS context-switching and thread creation costs.
* **The Sweet Spot:** Set `VECTOR_SIZE = 2000000` and set `NUM_THREADS` to match your CPU's actual physical cores. Watch the multi-threaded ingestion speed skyrocket.
* **Core Oversaturation:** Set `NUM_THREADS = 64` on a large vector array. Notice where the performance scaling plateaus or degrades as the CPU wastes clock cycles swapping registers.