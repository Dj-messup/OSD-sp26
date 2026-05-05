# Threads, Critical Sections, and Mutexes

This program demonstrates the problem of race conditions when multiple threads access shared data.

## Concept

The program creates multiple POSIX threads. Each thread increments two shared counters:

1. `unsafe_counter`
2. `safe_counter`

The `unsafe_counter` is incremented without a mutex. This means multiple threads can enter the critical section at the same time, causing a race condition.

The `safe_counter` is incremented inside a mutex-protected critical section. The mutex allows only one thread to modify the shared variable at a time.

## How to Build

```bash
make
