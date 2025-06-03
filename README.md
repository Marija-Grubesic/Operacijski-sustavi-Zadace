# Operating Systems

This repository contains solutions to various assignments completed during the Operating Systems course, demonstrating proficiency in process management, synchronization, and inter-process communication using C programming on a Linux environment.

---

## Task 1 — Long-running Process and Signals

The program calculates the square of a number every 5 seconds and writes the result to `obrada.txt`.  
It uses `status.txt` to resume execution and responds to the following signals:

- `SIGUSR1`: print the current number
- `SIGTERM`: save the current state
- `SIGINT`: terminate without saving

---

## Task 2 — Multitasking (Dekker)

Two processes use **Dekker's algorithm** to safely enter the critical section.  
Entries are printed in the following format:  
`Process <id>: (<id>, <step>, <loop>)`

---

## Task 3 — Multithreading (Lamport)

Multiple threads attempt to reserve tables in parallel.  
The critical section is protected using **Lamport’s algorithm**.  
Tables are displayed as a string like: `-231-` (numbers indicate which thread reserved which table).

---

## Task 4 — Semaphores and Carousel

A carousel simulation using **System V semaphores**.  
Visitors wait for a free seat, the ride starts when everyone is ready, and then all leave.  
Synchronization is handled using 3 semaphores: `seats`, `ready`, `ride_done`.

---

## Task 5 — Dining Philosophers Problem (Monitor)

Five philosophers share forks to eat.  
A **monitor** (mutex + condition variables) is used for safe resource sharing.  
A philosopher may eat only if both forks are free. The current state is printed as:  
`Philosopher state: O X o o O` (`O` – thinking, `o` – waiting, `X` – eating)

---
## Technologies Used

- C (POSIX, pthreads, signals)  
- Debian 12 / Linux  
- VMware Workstation
