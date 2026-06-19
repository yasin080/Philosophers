# philosophers

A multithreaded C program that solves the classic **Dining Philosophers** concurrency problem using POSIX threads and mutexes.

## The Problem

Five philosophers sit around a circular table. Between each pair of philosophers lies a single fork. To eat, a philosopher must pick up both forks — the one to their left and the one to their right. After eating, they put the forks down and start thinking. If every philosopher picks up their left fork at the same time, they all wait forever for the right fork — this is a **deadlock**. If a philosopher goes too long without eating, they **starve and die**.

The challenge: coordinate N philosophers sharing N forks using threads and mutual exclusion so that no one starves and no one deadlocks.

## Solution Approach

- **One thread per philosopher** — each philosopher runs an eat → sleep → think loop independently.
- **One mutex per fork** — a philosopher must lock both adjacent forks before eating, preventing two neighbors from sharing the same fork.
- **Asymmetric fork assignment** — even-numbered philosophers pick up the left fork first, odd-numbered philosophers pick up the right fork first. This breaks the circular wait condition and prevents deadlocks.
- **Monitor thread** — a separate thread continuously checks if any philosopher has exceeded `time_to_die` since their last meal. If so, it flags the simulation as ended.
- **Thread-safe shared state** — all reads and writes to shared variables (meal counts, last meal time, simulation status) go through mutex-protected getter/setter functions.
- **Precise timing** — a hybrid `usleep` + spinlock scheduler keeps timing accurate to ~1ms, checking for simulation end during waits.
- **Staggered start** — philosophers don't all start simultaneously. Even/odd staggering reduces initial contention for forks.
- **Single philosopher edge case** — handled separately: with only one philosopher and one fork, they take the fork and wait until they die.

## Usage

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

All times in milliseconds. The optional 6th argument makes the simulation stop once every philosopher has eaten at least that many meals.

## Examples

./philo 1 800 200 200          # One philosopher, dies at 800ms
./philo 5 800 200 200          # No one dies
./philo 4 400 200 200          # Philosopher dies at 400ms
./philo 4 410 200 200          # No one dies (410 > 2*200)
./philo 5 800 200 200 7        # Stops after 7 meals each

## Build

make            # Compile
make clean      # Remove object files
make fclean     # Remove objects and binary
make re         # Full rebuild

Compiled with `cc -Wall -Wextra -Werror -pthread`.

## Architecture

File                | Purpose
--------------------|----------------------------------------------
main.c              | Entry point, argument validation
parsing.c           | Input parsing and validation
init.c              | Data initialization and fork assignment
dinner.c            | Philosopher simulation (eat/sleep/think)
monitor.c           | Death detection thread
write_status.c      | Mutex-protected status output
getters_setters.c   | Thread-safe accessors for shared state
sync_utils.c        | Thread synchronization utilities
security.c          | Safe wrappers for malloc, mutex, pthread
utils.c             | Time, sleep, cleanup utilities
philo.h             | Header: types, structs, prototypes

Author: ybahri (42)
