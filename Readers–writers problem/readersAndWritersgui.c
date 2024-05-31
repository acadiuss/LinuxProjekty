#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <curses.h>

// Definiowanie liczby czytelników i pisarzy
#define NUM_READERS 5
#define NUM_WRITERS 5

// Semafory i mutex
sem_t rw_mutex;
sem_t mutex;
pthread_mutex_t curses_mutex;
int read_count = 0;

// Okno curses
WINDOW *win;
 
// Funkcja czyszczenia linii w oknie curses
void clear_line(int y) {
    pthread_mutex_lock(&curses_mutex);
    mvwprintw(win, y, 0, "                               ");
    wrefresh(win);
    pthread_mutex_unlock(&curses_mutex);
}

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        // Sekcja wejściowa
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex);
        }
        sem_post(&mutex);

        // Sekcja krytyczna
        pthread_mutex_lock(&curses_mutex);
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, reader_id, 0, "Reader %d is reading      ", reader_id);
        wattroff(win, COLOR_PAIR(1));
        wrefresh(win);
        pthread_mutex_unlock(&curses_mutex);
        
        sleep(2); // Symulacja czytania
        
        clear_line(reader_id);
        pthread_mutex_lock(&curses_mutex);
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, reader_id, 0, "Reader %d finished reading ", reader_id);
        wattroff(win, COLOR_PAIR(2));
        wrefresh(win);
        pthread_mutex_unlock(&curses_mutex);

        // Sekcja wyjściowa
        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex);
        }
        sem_post(&mutex);

        sleep(5); // Symulacja czasu między operacjami czytania
    }
    return NULL;
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        // Sekcja wejściowa
        sem_wait(&rw_mutex);

        // Sekcja krytyczna
        pthread_mutex_lock(&curses_mutex);
        wattron(win, COLOR_PAIR(3));
        mvwprintw(win, NUM_READERS + writer_id, 0, "Writer %d is writing      ", writer_id);
        wattroff(win, COLOR_PAIR(3));
        wrefresh(win);
        pthread_mutex_unlock(&curses_mutex);
        
        sleep(3); // Symulacja pisania
        
        clear_line(NUM_READERS + writer_id);
        pthread_mutex_lock(&curses_mutex);
        wattron(win, COLOR_PAIR(4));
        mvwprintw(win, NUM_READERS + writer_id, 0, "Writer %d finished writing ", writer_id);
        wattroff(win, COLOR_PAIR(4));
        wrefresh(win);
        pthread_mutex_unlock(&curses_mutex);

        // Sekcja wyjściowa
        sem_post(&rw_mutex);

        sleep(6); // Symulacja czasu między operacjami pisania
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];


    // Inicjalizacja curses
    initscr();
    cbreak();
    noecho();
    clear(); // Wyczyszczenie ekranu przed utworzeniem okna
    start_color(); // Inicjalizacja kolorów
    win = newwin(NUM_READERS + NUM_WRITERS, 40, 0, 0);
    refresh();

    // Definiowanie par kolorów
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Kolor dla czytającego
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Kolor dla kończącego czytanie
    init_pair(3, COLOR_BLUE, COLOR_BLACK); // Kolor dla piszącego
    init_pair(4, COLOR_RED, COLOR_BLACK); // Kolor dla kończącego pisanie

    // Inicjalizacja semaforów
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    pthread_mutex_init(&curses_mutex, NULL);

   // Tworzenie wątków czytelników
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Tworzenie wątków pisarzy
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Oczekiwanie na zakończenie wątków (w tym przykładzie nie nastąpi)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Czyszczenie zasobóws
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    pthread_mutex_destroy(&curses_mutex);
    endwin(); // Zakończenie pracy z curses

    return 0;
}
