#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>
#include <format>
#include "kaizen.h"
#include <mutex>

extern "C" const char* __tsan_default_options() {
  return "verbosity=1";
}

int mutex_counter = 0;
std::mutex mutex_counter_mutex;

std::pair<int, int> process_args(int argc, char* argv[]) {
    zen::cmd_args args(argv, argc);
    auto threadCount_options = args.get_options("--threads");
    auto iter_options = args.get_options("--iterations");

    int threads = 3, iters = 1000000; // Default values
    if (threadCount_options.empty() || iter_options.empty()) {
        zen::print("Error: Missing --threads or --iterations, using defaults: threads=3, iterations=1000000\n");
    } else {
        threads = std::stoi(threadCount_options[0]);
        iters = std::stoi(iter_options[0]);
    }
    return {threads, iters};
}

int main(int argc, char* argv[]) {
    auto [thread_count, iterations] = process_args(argc, argv);

    // Define counters
    int non_atomic_counter = 0; // Non-atomic, prone to race conditions
    std::atomic<int> seq_counter{0};
    std::atomic<int> relaxed_counter{0};
    std::atomic<int> release_counter{0};
    std::atomic<int> acquire_counter{0};

    std::vector<std::thread> threads;
    std::atomic<bool> running{true}; // Flag to control sampling thread

    // Sampling thread to periodically print counter values
    std::thread sampler([&]() {
        while (running.load()) {
            zen::print(std::format("\nSampled Values:\n"));
            zen::print(std::format("| {:<17} | {:>7}|\n","Non-Atomic:", non_atomic_counter));
            zen::print(std::format("| {:<17} | {:>7}|\n","SeqCst:", seq_counter.load()));
            zen::print(std::format("| {:<17} | {:>7}|\n","Relaxed:", relaxed_counter.load()));
            zen::print(std::format("| {:<17} | {:>7}|\n","Release:", release_counter.load()));
            zen::print(std::format("| {:<17} | {:>7}|\n","Acquire:", acquire_counter.load()));
            zen::print(std::format("| {:<17} | {:>7}|\n","Mutex:", mutex_counter));
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Sample every 50ms
        }
    });

    // Launch threads for all counters concurrently
    // Non-atomic counter
    for (int t = 0; t < thread_count; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < iterations; ++i) {
                non_atomic_counter++; // Race condition here

                std::this_thread::yield();
            }
        });
    }

    // Sequential Consistent (memory_order_seq_cst)
    for (int t = 0; t < thread_count; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < iterations; ++i) {
                seq_counter.fetch_add(1, std::memory_order_seq_cst);
            }
        });
    }


    // Mutex-protected counter
    for (int t = 0; t < thread_count; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < iterations; ++i) {
                {
                    std::lock_guard<std::mutex> lock(mutex_counter_mutex);
                    mutex_counter++;
                }
                std::this_thread::yield();
            }
        });
    }


    // Relaxed (memory_order_relaxed)
    for (int t = 0; t < thread_count; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < iterations; ++i) {
                relaxed_counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // Release (memory_order_release)
    for (int t = 0; t < thread_count; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < iterations; ++i) {
                release_counter.fetch_add(1, std::memory_order_release);
            }
        });
    }

    // Acquire (memory_order_acquire)
    for (int t = 0; t < thread_count; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < iterations; ++i) {
                acquire_counter.fetch_add(1, std::memory_order_acquire);
            }
        });
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Stop the sampler
    running.store(false);
    sampler.join();

    // Expected final value
    int expected = thread_count * iterations;

    // Print final results
    zen::print(std::format("\nFinal Counter Values:\n"));
    zen::print(std::format("| {:<17} | {:>7}|\n","Non-Atomic:", non_atomic_counter));
    zen::print(std::format("| {:<17} | {:>7}|\n","SeqCst:", seq_counter.load()));
    zen::print(std::format("| {:<17} | {:>7}|\n","Relaxed:", relaxed_counter.load()));
    zen::print(std::format("| {:<17} | {:>7}|\n","Release:", release_counter.load()));
    zen::print(std::format("| {:<17} | {:>7}|\n","Acquire:", acquire_counter.load()));
    zen::print(std::format("| {:<17} | {:>7}|\n","Mutex:", mutex_counter));
    zen::print(std::format("| {:<17} | {:>7}|\n","Expected:", expected));
    return 0;
}