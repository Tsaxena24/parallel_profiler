#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <ctime>

int main() {
// ______________________________________________________________________
//
//    USER CONFIGURATION ZONE: CHANGE THESE TO BENCHMARK YOUR CPU
// ______________________________________________________________________

    const size_t VECTOR_SIZE = 2000000; // Try 500 (small) vs 2,000,000 (large)
    const size_t NUM_THREADS = 4;       // Try matching your physical CPU core count
    std::cout<< "Time taken for vector size " << VECTOR_SIZE << " by: \n";
// _______________________________________________________________________

// Guard against 0 threads
    const size_t final_threads = (NUM_THREADS == 0) ? 1 : NUM_THREADS;

//Array Generation 
    
    std::vector<int> original_vector(VECTOR_SIZE);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, VECTOR_SIZE);

    std::generate(original_vector.begin(), original_vector.end(), [&]() {
        return distrib(gen);
    });

    std::vector<int> parallel_vector = original_vector;

//Single-Threaded Sort

    clock_t start_baseline = clock();
    std::sort(original_vector.begin(), original_vector.end());
    clock_t end_baseline = clock();
    double baseline_time = double(end_baseline - start_baseline) / CLOCKS_PER_SEC;

    std::cout << " Single threaded sort : " << baseline_time << " seconds\n";

//Concurrent execution
    clock_t start_parallel = clock();
    size_t chunk_size = VECTOR_SIZE / NUM_THREADS;
    std::vector<std::thread> workers;
    workers.reserve(NUM_THREADS);

    for (size_t i = 0; i < NUM_THREADS; ++i) {
        // Calculate bounds for this specific thread chunk
        auto chunk_start = parallel_vector.begin() + (i * chunk_size);
        auto chunk_end = (i == NUM_THREADS - 1) ? parallel_vector.end() : chunk_start + chunk_size;

        // Spawn thread and immediately store it in our tracking vector
        // Lambda captures the iterator range and executes std::sort in parallel
        workers.push_back(std::thread([chunk_start, chunk_end]() {
            std::sort(chunk_start, chunk_end);
        }));
    }

//Thread join
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

//In place merge
    size_t current_sorted_end = chunk_size;

    for (size_t i = 1; i < NUM_THREADS; ++i) {
        size_t next_chunk_end = (i == NUM_THREADS - 1) ? VECTOR_SIZE : (i + 1) * chunk_size;

        std::inplace_merge(
            parallel_vector.begin(), 
            parallel_vector.begin() + current_sorted_end, 
            parallel_vector.begin() + next_chunk_end
        );
        current_sorted_end = next_chunk_end;
    }

// Concurrent execution Output
    clock_t end_parallel = clock();
    double parallel_time = double(end_parallel - start_parallel) / CLOCKS_PER_SEC;
    std::cout << " Multi threaded( " << NUM_THREADS << " threads ) sort : " << parallel_time << " seconds\n";
}