# 42-philosophers

17 mai 9h30

## Index
- [Data Race](## Data Race)


## Data Race

```
C11 introduced support for multiple threads of execution, which affords the possibility of data races. A program contains a data race if an object in it is accessed1 by two different threads, where at least one of the accesses is non-atomic, at least one modifies the object, and program semantics fail to ensure that the two accesses cannot overlap temporally.2 Note well that actual concurrency of the accesses involved is not a condition for a data race; data races cover a broader class of issues arising from (allowed) inconsistencies in different threads' views of memory.

Consider this example:

#include <threads.h>

int a = 0;

int Function( void* ignore )
{
    a = 1;

    return 0;
}

int main( void )
{
    thrd_t id;
    thrd_create( &id , Function , NULL );

    int b = a;

    thrd_join( id , NULL );
}
The main thread calls thrd_create to start a new thread running function Function. The second thread modifies a, and the main thread reads a. Neither of those access is atomic, and the two threads do nothing either individually or jointly to ensure that they do not overlap, so there is a data race.

Among the ways this program could avoid the data race are

the main thread could perform its read of a before starting the other thread;
the main thread could perform its read of a after ensuring via thrd_join that the other has terminated;
the threads could synchronize their accesses via a mutex, each one locking that mutex before accessing a and unlocking it afterward.
As the mutex option demonstrates, avoiding a data race does not require ensuring a specific order of operations, such as the child thread modifying a before the main thread reads it; it is sufficient (for avoiding a data race) to ensure that for a given execution, one access will happen before the other.
```
- https://riptutorial.com/c/example/2622/data-race


## Testers

- [Socrates by nesvoboda](https://github.com/nesvoboda/socrates)
- [Philophers visualizer](https://nafuka11.github.io/philosophers-visualizer/)

References:
- [Concurrency (computer science)](https://en.wikipedia.org/wiki/Concurrency_(computer_science))
- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Deadlock]()https://en.wikipedia.org/wiki/Deadlock
- [Mutex -> Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)
- [Mutex vulgarisation](https://stackoverflow.com/questions/34524/what-is-a-mutex)
- [pthread_mutex_init() manual](https://linux.die.net/man/3/pthread_mutex_init)
- [pthread_create() manual](https://linux.die.net/man/3/pthread_create)
- [pthread_join() manual](https://linux.die.net/man/3/pthread_join)
- [timeoftheday()](https://linuxhint.com/gettimeofday_c_language/)
- [Tester](https://github.com/newlinuxbot/Philosphers-42Project-Tester/blob/master/start.sh)
- [Awesome README with kitty kat and litter. miaow](https://github.com/lavrenovamaria/42-philosophers)
- [EXIT_SUCCESS, EXIT_FAILURE](https://en.cppreference.com/w/c/program/EXIT_status)