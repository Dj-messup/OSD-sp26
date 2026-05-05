#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 100000

/*
 * These counters are shared by all threads.
 * Because multiple threads access them, the increment operation becomes
 * a critical section.
 */
int unsafe_counter = 0;
int safe_counter = 0;

/*
 * This mutex protects the safe_counter critical section.
 */
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread_id;
} ThreadInfo;

void *thread_work(void *arg) {
    ThreadInfo *info = (ThreadInfo *)arg;

    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        /*
         * Critical section without a mutex.
         * This has a race condition because multiple threads can read,
         * modify, and write unsafe_counter at the same time.
         */
        int temp = unsafe_counter;
        temp = temp + 1;

        /*
         * Yielding sometimes makes thread interleaving more visible.
         * This helps show the race condition during the demo.
         */
        if (i % 1000 == 0) {
            sched_yield();
        }

        unsafe_counter = temp;

        /*
         * Critical section with a mutex.
         * Only one thread can modify safe_counter at a time.
         */
        pthread_mutex_lock(&counter_mutex);
        safe_counter = safe_counter + 1;
        pthread_mutex_unlock(&counter_mutex);
    }

    printf("Thread %d finished.\n", info->thread_id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    ThreadInfo thread_info[NUM_THREADS];

    int expected_value = NUM_THREADS * INCREMENTS_PER_THREAD;

    printf("Starting thread demo.\n");
    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Increments per thread: %d\n", INCREMENTS_PER_THREAD);
    printf("Expected final value: %d\n\n", expected_value);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_info[i].thread_id = i;

        if (pthread_create(&threads[i], NULL, thread_work, &thread_info[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nFinal results:\n");
    printf("Unsafe counter without mutex: %d\n", unsafe_counter);
    printf("Safe counter with mutex:     %d\n", safe_counter);
    printf("Expected value:              %d\n", expected_value);

    pthread_mutex_destroy(&counter_mutex);

    return 0;
}
