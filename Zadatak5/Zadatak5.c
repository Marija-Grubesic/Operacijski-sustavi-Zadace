#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5  // broj filozofa

// stanja filozofa: 'O' - razmišlja, 'o' - čeka vilice, 'X' - jede
char filozof[N];

// vilice: 1 - slobodna, 0 - zauzeta
int vilica[N] = {1, 1, 1, 1, 1};

pthread_mutex_t monitor = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t red[N];

void ispis_stanja() {
    printf("Stanje filozofa: ");
    for (int i = 0; i < N; i++) {
        printf("%c ", filozof[i]);
    }
    printf("\n");
}

void misliti(int id) {
    pthread_mutex_lock(&monitor);
    filozof[id] = 'O';
    ispis_stanja();
    pthread_mutex_unlock(&monitor);
    sleep(3); // simulacija razmišljanja
}

void jesti(int id) {
    pthread_mutex_lock(&monitor);
    filozof[id] = 'o'; // čeka vilice
    ispis_stanja();

    while (vilica[id] == 0 || vilica[(id + 1) % N] == 0) {
        pthread_cond_wait(&red[id], &monitor);
    }

    vilica[id] = vilica[(id + 1) % N] = 0; // uzmi obje vilice
    filozof[id] = 'X'; // jede
    ispis_stanja();

    pthread_mutex_unlock(&monitor);
    sleep(2); // simulacija jedenja
}

void pusti_vilice(int id) {
    pthread_mutex_lock(&monitor);
    filozof[id] = 'O';

    vilica[id] = vilica[(id + 1) % N] = 1;

    // probudi susjede ako mogu sada jesti
    pthread_cond_signal(&red[(id + N - 1) % N]);
    pthread_cond_signal(&red[(id + 1) % N]);

    ispis_stanja();
    pthread_mutex_unlock(&monitor);
}

void* filozof_radi(void* arg) {
    int id = *(int*)arg;
    free(arg);

    while (1) {
        misliti(id);
        jesti(id);
        pusti_vilice(id);
    }

    return NULL;
}

int main() {
    pthread_t dretve[N];

    for (int i = 0; i < N; i++) {
        filozof[i] = 'O'; // svi počinju kao mislioci
        pthread_cond_init(&red[i], NULL);
    }

    ispis_stanja();

    for (int i = 0; i < N; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&dretve[i], NULL, filozof_radi, id);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(dretve[i], NULL);
    }

    return 0;
}
