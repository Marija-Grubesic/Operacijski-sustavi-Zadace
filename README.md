# Operacijski sustavi — Zadaće 1 i 2

Rješenja zadataka iz kolegija **Operacijski sustavi**, rađena u jeziku **C** na **Debian 12** unutar **VMware Workstation**.

---

## Zadatak 1 — Dugotrajni posao i signali

Program svakih 5 sekundi izračunava kvadrat broja i zapisuje ga u `obrada.txt`, dok `status.txt` pamti stanje izvođenja za nastavak.

### Signali:
- `SIGUSR1`: ispis trenutnog broja
- `SIGTERM`: spremi stanje i izađi
- `SIGINT`: prekid bez spremanja

---
## Zadatak 2 — Višezadaćnost (Dekkerov algoritam)

Dva procesa (0 i 1) izmjenično ulaze u kritični odsječak pomoću Dekkerovog algoritma i zajedničke memorije (shmget, shmat).

Svaki proces 5 puta ulazi u kritični odsječak i ispisuje:

Proces `<id>`: `(<id>, <korak>, <petlja>)`
