#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

volatile sig_atomic_t broj = 0;
const char *obrada_file = "obrada.txt";
const char *status_file = "status.txt";
volatile sig_atomic_t running = 1;

// Funkcija za čitanje broja iz status.txt
int procitaj_status() {
    FILE *f = fopen(status_file, "r");
    if (!f) {
        perror("Ne mogu otvoriti status.txt");
        return -1;
    }
    int val = 0;
    fscanf(f, "%d", &val);
    fclose(f);
    return val;
}

// Funkcija za pisanje broja u status.txt
void zapisi_status(int val) {
    FILE *f = fopen(status_file, "w");
    if (!f) {
        perror("Ne mogu otvoriti status.txt za pisanje");
        return;
    }
    fprintf(f, "%d\n", val);
    fclose(f);
}

// Funkcija za čitanje zadnjeg broja iz obrada.txt
int procitaj_zadnji_iz_obrada() {
    FILE *f = fopen(obrada_file, "r");
    if (!f) {
        perror("Ne mogu otvoriti obrada.txt");
        return -1;
    }
    int zadnji = 0, trenutni;
    while (fscanf(f, "%d", &trenutni) == 1) {
        zadnji = trenutni;
    }
    fclose(f);
    return zadnji;
}

// Handler za SIGUSR1 - ispisuje trenutni broj
void sigusr1_handler(int signo) {
    printf("Trenutni broj: %d\n", broj);
}

// Handler za SIGTERM - zapisi broj u status.txt i zavrsi program
void sigterm_handler(int signo) {
    zapisi_status(broj);
    printf("Primljen SIGTERM, završavam.\n");
    running = 0;
}

// Handler za SIGINT - samo prekini program bez pisanja u status.txt
void sigint_handler(int signo) {
    printf("Primljen SIGINT, prekidam bez upisa u status.txt.\n");
    running = 0;
}

int main() {
    // Postavi handler-e
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigint_handler);

    broj = procitaj_status();

    if (broj == 0) {
        // Treba odrediti broj iz obrada.txt
        int zadnji = procitaj_zadnji_iz_obrada();
        if (zadnji <= 0) {
            // Ako nema ništa, počinjemo od 1
            broj = 1;
        } else {
            // broj = korijen zadnjeg broja (int)
            broj = (int)round(sqrt(zadnji)) - 1;
        }
    }

    // Zapiši 0 u status.txt - obrada je u tijeku
    zapisi_status(0);

    while (running) {
        broj++;
        int x = broj * broj;

        // Dodaj x u obrada.txt
        FILE *f = fopen(obrada_file, "a");
        if (!f) {
            perror("Ne mogu otvoriti obrada.txt za dodavanje");
            break;
        }
        fprintf(f, "%d\n", x);
        fclose(f);

        printf("Trenutni broj: %d, kvadrat: %d\n", broj, x);
        fflush(stdout);

        sleep(5);
    }

    return 0;
}
