#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define KORACI 5 // koliko puta svaki proces ulazi u kritični odsječak

int *ZASTAVICA; // pokazivač na zastavice [0] i [1]
int *PRAVO;     // pokazivač na varijablu PRAVO
int shmid1, shmid2;  // ID zajedničke memorije (dva segmenta)

// Funkcija za čišćenje memorije i izlazak iz programa (poziva se pri CTRL+C)
void cleanup(int sig) {
    printf("\n[INFO] Primljen SIGINT, brišem zajedničku memoriju i izlazim.\n");

    shmdt(ZASTAVICA);
    shmdt(PRAVO);

    shmctl(shmid1, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);

    exit(0);
}

// DEKKER: Ulazak u kritični odsječak
void udji_u_kriticni(int i, int j) {
    ZASTAVICA[i] = 1;
    while (ZASTAVICA[j]) {
        if (*PRAVO == j) {
            ZASTAVICA[i] = 0;
            while (*PRAVO == j); // čekaj dok drugi ima pravo
            ZASTAVICA[i] = 1;
        }
    }
}

// DEKKER: Izlazak iz kritičnog odsječka
void izadji_iz_kriticnog(int i, int j) {
    *PRAVO = j;         // prednost dajemo drugom procesu
    ZASTAVICA[i] = 0;   // označi da više ne želi ući
}

// Funkcija koju izvodi svaki proces (0 ili 1)
void proces(int id) {
    int drugi = 1 - id; // indeks drugog procesa

    for (int k = 1; k <= KORACI; k++) {
        udji_u_kriticni(id, drugi); // ulazak u kritični odsječak

        // simulacija kritičnog odsječka: 5 puta ispiši
        for (int m = 1; m <= KORACI; m++) {
            printf("Proces %d: (%d, %d, %d)\n", id, id, k, m);
            fflush(stdout);
            sleep(1); // usporavanje da se razlika bolje vidi
        }

        izadji_iz_kriticnog(id, drugi); // izlazak
    }

    // odspajanje zajedničke memorije u procesu
    shmdt(ZASTAVICA);
    shmdt(PRAVO);

    exit(0); // proces završava
}

int main() {
    signal(SIGINT, cleanup); // obrada signala Ctrl+C

    shmid1 = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0600);
    ZASTAVICA = shmat(shmid1, NULL, 0);

    shmid2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    PRAVO = shmat(shmid2, NULL, 0);

    // Inicijalizacija
    ZASTAVICA[0] = 0;
    ZASTAVICA[1] = 0;
    *PRAVO = 0; // proces 0 ima početnu prednost

    pid_t pid = fork(); // stvaranje djeteta (proces 1)

    if (pid == 0) {
        // Dijete – proces 1
        proces(1);
    } else if (pid > 0) {
        // Roditelj – proces 0
        proces(0);

        // čekaj završetak djeteta
        wait(NULL);
        wait(NULL);

        // odspajanje u roditelju
        shmdt(ZASTAVICA);
        shmdt(PRAVO);

        // brisanje zajedničke memorije
        cleanup(0);
    } else {
        perror("Greška kod fork()");
    }

    return 0;
}
