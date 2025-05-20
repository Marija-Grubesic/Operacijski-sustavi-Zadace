# Operacijski sustavi — Zadaće 1 i 2

Repozitorij sadrži rješenja prvih dvaju zadataka iz kolegija **Operacijski sustavi**.  
Zadaci su implementirani u programskom jeziku **C** i testirani unutar **Debian 12** virtualne mašine pokrenute u **VMware Workstation**.

---

## Zadatak 1 – Dugotrajni posao i rad sa signalima

**Opis:**

Program simulira dugotrajni proces koji svakih nekoliko sekundi izračunava kvadrat cijelog broja i pohranjuje ga u datoteku `obrada.txt`. Kako bi znao gdje je stao u prethodnom izvođenju, koristi datoteku `status.txt` za pohranu trenutnog broja.

**Značajke:**
- Svakih 5 sekundi izračunava kvadrat sljedećeg broja.
- Piše kvadrate u `obrada.txt`, a stanje čuva u `status.txt`.
- Reagira na signale:
  - `SIGUSR1` – ispisuje trenutni broj koji se obrađuje.
  - `SIGTERM` – upisuje stanje u `status.txt` i prekida rad.
  - `SIGINT` – prekida rad bez zapisivanja stanja (npr. `Ctrl+C`), čime će pri sljedećem pokretanju analizirati `obrada.txt` i nastaviti gdje je stao.

**Pokretanje:**

gcc Zadatak1.c -o program -lm
./program

## Zadatak 2 – Višezadaćnost pomoću procesa i Dekkerov algoritam

**Opis:**

U ovom zadatku simulira se rad dva procesa koji pokušavaju pristupiti zajedničkom resursu, pri čemu se koristi **Dekkerov algoritam** za međusobno isključivanje. Cilj je omogućiti da u svakom trenutku **samo jedan proces** može biti u kritičnom odsječku, kako bi se spriječila istovremena obrada dijeljenih podataka.

Za komunikaciju i koordinaciju između procesa koristi se **zajednička memorija** (`shmget`, `shmat`), a ispis stanja se koristi za vizualizaciju pristupa kritičnom odsječku.

---

**Značajke:**
- Proces 0 i Proces 1 koriste **zastavice** (`ZASTAVICA[i]`) i varijablu `PRAVO` za kontrolu ulaska u kritični odsječak.
- Svaki proces pokušava ući u kritični odsječak **5 puta**.
- Unutar kritičnog odsječka svaki proces pet puta ispisuje status u obliku:
  
  Proces `<id>`: `(<id>, <k>, <m>)`
gdje je:
  - `<id>` = ID procesa (0 ili 1)
  - `<k>` = broj ulaska u kritični odsječak (od 1 do 5)
  - `<m>` = broj unutar unutarnje petlje (od 1 do 5)

- Nakon svakog ulaska i ispisa, proces izlazi iz kritičnog odsječka i daje prednost drugom procesu.
- Program koristi `signal(SIGINT, cleanup)` kako bi oslobodio zajedničku memoriju ako korisnik prekine rad (`Ctrl + C`).

---
  
