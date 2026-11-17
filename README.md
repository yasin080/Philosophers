# philosophers

My solution to the Dining Philosophers problem for 42, in C with POSIX
threads and mutexes.

N philosophers sit around a table with one fork between each pair. To eat, a
philosopher needs both forks next to them. If one of them goes `time_to_die`
milliseconds without starting a meal, they die and the simulation stops. The
program has to avoid deadlocks and print every state change in order.

## Usage

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Times are in milliseconds. With the optional last argument the simulation ends
once every philosopher has eaten at least that many times. Every argument must
be a positive integer (the last one can be 0, which ends immediately).

```
./philo 1 800 200 200          # one philosopher, dies at 800 ms
./philo 5 800 200 200          # nobody dies
./philo 4 400 200 200          # a philosopher dies at 400 ms
./philo 4 410 200 200          # nobody dies (410 > 2 * 200)
./philo 5 800 200 200 7        # stops after 7 meals each
```

## How it works

- One thread per philosopher and one mutex per fork.
- Odd philosophers take their left fork first, even ones their right fork.
  With that order the circular wait cannot happen, so there is no deadlock.
- The main thread is the monitor: it checks every philosopher's last meal
  time (protected by a per-philosopher mutex) and stops the simulation on a
  death or when everyone has eaten enough.
- A shared `stop` flag, also behind a mutex, is checked by every thread.
  "died" is printed while holding the print mutex and raising the flag at the
  same time, so nothing is printed after it.
- All threads wait for a common start time. Even philosophers start half a
  meal late, and with an odd number of philosophers thinking takes a bit of
  time so the hungriest neighbour gets the forks first.
- Sleeping is done in small `usleep` steps so a philosopher notices the end of
  the simulation quickly.

Checked with `-fsanitize=thread` (no data races) and with 1 to 200
philosophers.

## Build

Compilar el proyecto:

```bash
make
```

Targets disponibles:

| Target | Descripción |
|--------|-------------|
| `make` | Compila el proyecto (equivale a `make all`) |
| `make clean` | Elimina los archivos objeto y el directorio `obj/` |
| `make fclean` | Ejecuta `clean` y elimina el binario `philo` |
| `make re` | Compilación completa: `fclean` + `make all` |

Compilado con `cc -Wall -Wextra -Werror -pthread`.

## Files

| File | Purpose |
|------|---------|
| `main.c` | starts the threads, runs the monitor, joins and cleans up |
| `parse.c` | argument validation |
| `init.c` | mutexes, philosophers and fork order |
| `routine.c` | eat / sleep / think loop |
| `monitor.c` | death and "everyone ate enough" checks |
| `print.c` | stop flag and state messages |
| `time.c` | time in ms and sleeping helpers |
| `philo.h` | structs and prototypes |

## What I'd improve

- The bonus version with processes and semaphores.
- Replace the monitor's polling with a smarter wake-up, it currently checks
  every 0.5 ms.

Author: ybahri (42)
