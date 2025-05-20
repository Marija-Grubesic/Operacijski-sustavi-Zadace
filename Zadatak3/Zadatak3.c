#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define MAX_STOLOVA 100
#define MAX_DRETVI 100

int *stolovi;            // 0 = slobodan, >0 = broj dretve
int broj_stolova;
int broj_dretvi;

int ULAZ[MAX_DRETVI];
int BROJ[MAX_DRETVI];

pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

int svi_zauzeti() {
    for (int i = 0; i < broj_stolova; i++) {
        if (stolovi[i] == 0) return 0;
    }
    return 1;
}

int max_broj() {
    int max = 0;
    for (int i = 0; i < broj_dretvi; i++) {
        if (BROJ[i] > max) max = BROJ[i];
    }
    return max;
}

void udji_u_kriticni(int i) {
    ULAZ[i] = 1;
    BROJ[i] = max_broj() + 1;
    ULAZ[i] = 0;

    for (int j = 0; j < broj_dretvi; j++) {
        while (ULAZ[j]);
        while (BROJ[j] != 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i)));
    }
}

void izadji_iz_kriticnog(int i) {
    BROJ[i] = 0;
}

void ispisi_stanje() {
    for (int i = 0; i < broj_stolova; i++) {
        if (stolovi[i] == 0)
            printf("-");
        else
            printf("%d", stolovi[i]);
    }
    printf("\n");
}

void* dretva_funkcija(void* arg) {
    int id = *(int*)arg;
    free(arg);
    srand(time(NULL) + id);

    int rezervirano = 0;

    while (!svi_zauzeti() && !rezervirano) {
        sleep(1);

        int stol = rand() % broj_stolova;
        pthread_mutex_lock(&print_lock);
        printf("Dretva %d: odabirem stol %d\n", id + 1, stol + 1);
        pthread_mutex_unlock(&print_lock);

        udji_u_kriticni(id);

        if (stolovi[stol] == 0) {
            stolovi[stol] = id + 1;
            rezervirano = 1;
            pthread_mutex_lock(&print_lock);
            printf("Dretva %d: rezerviram stol %d, stanje: ", id + 1, stol + 1);
            ispisi_stanje();
            pthread_mutex_unlock(&print_lock);
        } else {
            pthread_mutex_lock(&print_lock);
            printf("Dretva %d: neuspjela rezervacija stola %d, stanje: ", id + 1, stol + 1);
            ispisi_stanje();
            pthread_mutex_unlock(&print_lock);
        }

        izadji_iz_kriticnog(id);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Upotreba: %s broj_dretvi broj_stolova\n", argv[0]);
        return 1;
    }

    broj_dretvi = atoi(argv[1]);
    broj_stolova = atoi(argv[2]);

    if (broj_dretvi > MAX_DRETVI || broj_stolova > MAX_STOLOVA) {
        printf("Prevelik broj dretvi ili stolova.\n");
        return 1;
    }

    stolovi = calloc(broj_stolova, sizeof(int));

    pthread_t dretve[broj_dretvi];

    for (int i = 0; i < broj_dretvi; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&dretve[i], NULL, dretva_funkcija, id);
    }

    for (int i = 0; i < broj_dretvi; i++) {
        pthread_join(dretve[i], NULL);
    }

    free(stolovi);
    return 0;
}
