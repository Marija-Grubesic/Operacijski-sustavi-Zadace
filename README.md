# Operacijski sustavi

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

## Zadatak 4 — Semafori i vrtuljak

Simulacija vrtuljka pomoću **System V semafora**.  
Posjetitelji čekaju slobodno mjesto, vožnja se pokreće kad su svi spremni, a zatim svi silaze.  
Sinkronizacija se provodi pomoću 3 semafora: `mjesta`, `spremni`, `voznja_gotova`.

---

## Zadatak 5 — Problem pet filozofa (monitor)

Pet filozofa dijeli vilice za jelo.  
Korišten je **monitor** (mutex + uvjetne varijable) za sigurno dijeljenje resursa.  
Filozofi mogu jesti samo ako su obje vilice slobodne; ispisuje se trenutno stanje:  
`Stanje filozofa: O X o o O` (`O` – razmišlja, `o` – čeka, `X` – jede)

---
