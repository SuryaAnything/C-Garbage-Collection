# Memory Management in C: Shared Pointers with Garbage Collection

## Overview
This project implements shared pointer functionality with garbage collection in C. It provides a mechanism for managing memory allocation and deallocation while ensuring proper reference counting and garbage collection to prevent memory leaks.

## Files
- **sharedptrfwd.h**: Header file containing declarations for shared pointer functionality and synchronization mechanisms.
- **sharedptrfwd.c**: Implementation of shared pointer functionality including memory allocation, reference counting, and garbage collection.
- **main.c**: Main file containing example usage and testing of the shared pointer functionality.

## Usage
To use the shared pointer functionality in your project:
1. Include the `sharedptrfwd.h` header file in your source files.
2. Use the provided functions for memory allocation (`sync_with_dynamic_allocator()`), loading allocator (`allocator_load()`), moving references (`move_ref()`), and other operations as needed.

## Implementation Details
- **Memory Allocation**: Memory is allocated using `sync_with_dynamic_allocator()` function, which synchronizes memory allocation with proper reference counting.
- **Reference Counting**: Reference counting is maintained for each allocated memory block to track the number of references.
- **Garbage Collection**: Garbage collection is triggered periodically to reclaim memory blocks with zero references, preventing memory leaks.
- **Logging**: Events and system statistics are logged to a file (`last_test.log`) for debugging and monitoring purposes.

## Compilation and Execution
Compile the project using a C compiler such as GCC or clang:
```bash
clang main.c sharedptrfwd.c -o driver -lpthread
./driver.exe
```



## Dependencies
- `pthread.h`: For thread synchronization.
- `windows.h`: For Windows-specific functionality (optional, used for system stats).
- `time.h`: For time-related functions.
- `stdbool.h`, `stdarg.h`, `string.h`, `unistd.h`, `malloc.h`: Standard C libraries.

