#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Define the number of readers and writers
#define NUM_READERS 5
#define NUM_WRITERS 5

// Semaphores and mutex
sem_t rw_mutex;
sem_t mutex;
int read_count = 0;

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        // Entry section
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex);
        }
        sem_post(&mutex);

        // Critical section
        printf("Reader %d is reading\n", reader_id);
        sleep(2); // Simulate reading
        printf("Reader %d finished reading\n", reader_id);

        // Exit section
        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex);
        }
        sem_post(&mutex);

        sleep(5); // Simulate time between reads
    }
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        // Entry section
        sem_wait(&rw_mutex);

        // Critical section
        printf("Writer %d is writing\n", writer_id);
        sleep(3); // Simulate writing
        printf("Writer %d finished writing\n", writer_id);

        // Exit section
        sem_post(&rw_mutex);

        sleep(6); // Simulate time between writes
    }
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    // Initialize semaphores
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Wait for threads to finish (they won't, in this example)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Clean up
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    return 0;
}
