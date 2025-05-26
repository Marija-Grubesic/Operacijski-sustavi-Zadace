# Operacijski sustavi — Zadaće 1, 2 i 3

Rješenja zadataka iz kolegija **Operacijski sustavi**, rađena u **C jeziku** na **Debian 12 (VMware Workstation)**.

---

## Zadatak 1 — Dugotrajni posao i signali

Program svakih 5 sekundi računa kvadrat broja i zapisuje ga u `obrada.txt`.  
Koristi `status.txt` za nastavak izvođenja i reagira na signale:

- `SIGUSR1`: ispis broja
- `SIGTERM`: spremi stanje
- `SIGINT`: prekid bez spremanja

---

## Zadatak 2 — Višezadaćnost (Dekker)

Dva procesa koriste **Dekkerov algoritam** za siguran ulazak u kritični odsječak.  
Ispisuju se ulasci u obliku:  
`Proces <id>: (<id>, <korak>, <petlja>)`

---

## Zadatak 3 — Višedretvenost (Lamport)

Više dretvi paralelno pokušava rezervirati stolove.  
Kritični odsječak zaštićen je **Lamportovim algoritmom**.  
Stolovi se ispisuju kao niz: `-231-` (brojevi označavaju tko je rezervirao koji stol).

---
