#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define N 10 // broj mjesta i broj posjetitelja

int semid;

enum { MJESTA = 0, SPREMNI = 1, VOZNJA_GOTOVA = 2 };

// P (wait) operacija
void P(int semnum) {
    struct sembuf op = {semnum, -1, 0};
    semop(semid, &op, 1);
}

// V (signal) operacija
void V(int semnum) {
    struct sembuf op = {semnum, +1, 0};
    semop(semid, &op, 1);
}

void posjetitelj(int id) {
    P(MJESTA); // čeka slobodno mjesto
    printf("Posjetitelj %d: sjeda na vrtuljak.\n", id);

    V(SPREMNI); // označava da je spreman

    P(VOZNJA_GOTOVA); // čeka kraj vožnje
    printf("Posjetitelj %d: silazi s vrtuljka.\n", id);

    V(MJESTA); // oslobađa mjesto

    exit(0);
}

void vrtuljak() {
    while (1) {
        // čekaj da svi sjednu
        for (int i = 0; i < N; i++)
            P(SPREMNI);

        printf("Vrtuljak: pokrećem vožnju!\n");
        sleep(3); // vožnja traje

        printf("Vrtuljak: zaustavljam se.\n");

        // pusti sve da siđu
        for (int i = 0; i < N; i++)
            V(VOZNJA_GOTOVA);
    }
}

void cleanup(int sig) {
    semctl(semid, 0, IPC_RMID, 0);
    printf("\nSemafori uklonjeni. Izlazim.\n");
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    // Stvori skup od 3 semafora
    semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0600);

    // Inicijalizacija semafora
    semctl(semid, MJESTA, SETVAL, N);
    semctl(semid, SPREMNI, SETVAL, 0);
    semctl(semid, VOZNJA_GOTOVA, SETVAL, 0);

    // Pokreni vrtuljak kao zaseban proces
    if (fork() == 0) {
        vrtuljak();
        exit(0);
    }

    // Pokreni posjetitelje
    for (int i = 0; i < N; i++) {
        if (fork() == 0) {
            posjetitelj(i + 1);
        }
        sleep(1); // razmak između dolazaka
    }

    // Čekaj sve posjetitelje
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Kad svi posjetitelji završe, prekini vrtuljak
    kill(0, SIGINT);

    return 0;
}
