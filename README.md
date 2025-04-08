# Thread-Safe_Counter



## Overview

This program demonstrates and compares the behavior of different memory ordering models for atomic operations in a multi-threaded environment, alongside a non-atomic counter for reference. It showcases how race conditions affect a non-atomic counter and how various memory ordering constraints impact performance and correctness.

## Features
- Compares a non-atomic integer counter with atomic counters using different memory ordering models:
  - `memory_order_seq_cst` (Sequential Consistency)
  - `memory_order_relaxed` (Relaxed)
  - `memory_order_release` (Release)
  - `memory_order_acquire` (Acquire)
- Configurable number of threads and iterations via command-line arguments.
- Real-time sampling of counter values every 200ms during execution.
- Final comparison of counter values against the expected result.

## Prerequisites
- A C++20 (or later)  (for `std::format` and modern features) compatible compiler (e.g., GCC, Clang, MSVC).
- **Custom headers**:
  - [`kaizen.h`](https://github.com/heinsaar/kaizen) (provides `zen::print`, `zen::color`, `zen::cmd_args`, and other utilities)

## Build Instructions

1. Clone the repository:
   ```
   git clone https://github.com/username/Thread-Safe_Counter
   ```
2. Navigate to the repository:
   ```
   cd Thread-Safe_Counter
   ```
3. Generate build files:
   ```
   cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
   ```
4. Build the project:
   ```
   cmake --build build --config Release
   ```
5. Run the executable from the build directory:
   ```
   ./build/Thread-Safe_Counter

   ```

## Usage

Run the program with optional arguments to customize the experiment parameters:

```
./Instruction_Reordering_Experiment --threads [num] --iterations [num]  as in int
```
- `--iterations`: Number of summation iterations per trial (default:1,000).
- `--threads`: Number of summation iterations per trial (default: 3).

# How It Works
- Argument Parsing: The program parses --threads and --iterations from the command line using - zen::cmd_args. Defaults are used if arguments are missing.
- Counters:
    - non_atomic_counter: A regular integer prone to race conditions.
    - seq_counter: Atomic with sequential consistency (strictest memory ordering).
    - relaxed_counter: Atomic with relaxed memory ordering (least strict).
    - release_counter: Atomic with release semantics.
    - acquire_counter: Atomic with acquire semantics.
- Threads:
    - A separate set of threads is launched for each counter type.
    - Each thread increments its assigned counter for the specified number of iterations.
    - A sampling thread runs concurrently, printing all counter values every 200ms.
- Output:
    - During execution, sampled counter values are displayed periodically.
    - After all threads complete, final counter values are printed alongside the expected value (threads * iterations).

### Expected Behavior
    - Non-Atomic Counter: Due to race conditions, the final value will likely be less than the expected value, as increments can overwrite each other.
    - Atomic Counters: All atomic counters should reach the expected value, but their performance and synchronization guarantees differ:
    - SeqCst: Ensures total order across all threads (safest, potentially slowest).
    - Relaxed: No ordering guarantees between operations (fastest, least synchronized).
    - Release/Acquire: Pairwise synchronization (balanced approach).

## Example Output

Below is sample output from running the program with ` --iterations 1000000`:
```

Sampled Values:
Non-Atomic:  0
SeqCst:  1000
Relaxed:  2000
Release:  2000
Acquire:  2000

Final Counter Values:
Non-Atomic:  3000
SeqCst:  3000
Relaxed:  3000
Release:  3000
Acquire:  3000
Expected:  3000
```

v
