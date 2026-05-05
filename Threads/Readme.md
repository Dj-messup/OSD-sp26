# Threads, Critical Sections, and Mutexes

This program demonstrates threads, critical sections, race conditions, and mutexes using POSIX threads in C.

The program creates four threads. Each thread increments two shared counters 100,000 times.

The unsafe counter is changed without a mutex. This creates a race condition because multiple threads can read, modify, and write the shared value at the same time. Because of that, the final unsafe value is usually lower than the expected value.

The safe counter is changed inside a mutex-protected critical section. The mutex allows only one thread to update the counter at a time, so the final value matches the expected value.

## Build

```bash
make
