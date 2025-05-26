#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX 100

int *ULAZ;
int *BROJ;
int *stolovi;
int broj_stolova, broj_dretvi;

pthread_t *dretve;

void u_kriticni(int i) {
    ULAZ[i] = 1;
    int max = 0;
    for (int j = 0; j < broj_dretvi; j++) {
        if (BROJ[j] > max) max = BROJ[j];
    }
    BROJ[i] = max + 1;
    ULAZ[i] = 0;

    for (int j = 0; j < broj_dretvi; j++) {
        while (ULAZ[j]);
        while (BROJ[j] != 0 &&
              (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i)));
    }
}

void iz_kriticnog(int i) {
    BROJ[i] = 0;
}

void ispis_stolova() {
    for (int i = 0; i < broj_stolova; i++) {
        if (stolovi[i] == 0) printf("-");
        else printf("%d", stolovi[i]);
    }
    printf("\n");
}

void* dretva_rezervacija(void* arg) {
    int id = *(int*)arg;
    free(arg);
    srand(time(NULL) + id * 37);

    while (1) {
        sleep(1);
        int odabrani = rand() % broj_stolova;
        printf("Dretva %d: odabirem stol %d\n", id, odabrani + 1);

        u_kriticni(id);
        if (stolovi[odabrani] == 0) {
            stolovi[odabrani] = id;
            printf("Dretva %d: rezerviram stol %d, stanje: ", id, odabrani + 1);
        } else {
            printf("Dretva %d: neuspjela rezervacija stola %d, stanje: ", id, odabrani + 1);
        }
        ispis_stolova();
        iz_kriticnog(id);

        // prekini dretvu ako su svi stolovi zauzeti
        int slobodno = 0;
        for (int i = 0; i < broj_stolova; i++)
            if (stolovi[i] == 0)
                slobodno = 1;

        if (!slobodno) break;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Upotreba: %s <broj_dretvi> <broj_stolova>\n", argv[0]);
        return 1;
    }

    broj_dretvi = atoi(argv[1]);
    broj_stolova = atoi(argv[2]);

    ULAZ = calloc(broj_dretvi, sizeof(int));
    BROJ = calloc(broj_dretvi, sizeof(int));
    stolovi = calloc(broj_stolova, sizeof(int));
    dretve = malloc(broj_dretvi * sizeof(pthread_t));

    for (int i = 0; i < broj_dretvi; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&dretve[i], NULL, dretva_rezervacija, id);
    }

    for (int i = 0; i < broj_dretvi; i++) {
        pthread_join(dretve[i], NULL);
    }

    free(ULAZ);
    free(BROJ);
    free(stolovi);
    free(dretve);
    return 0;
}
