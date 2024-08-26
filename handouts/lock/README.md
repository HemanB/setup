**Objectives**
- Learning about concurrency through the use of threads
- Familiarize yourself with mutual exclusion and locks

---

**Overview**

This is your second lab learning about concurrency through the use
of posix threads.  This lab focuses on mutual exclusion and ensuring correct
operation by using locks. As with the previous lab, use the capital P routines defined in [mythreads.h](mythreads.h)
for correct autograding (e.g., `Pthread_create` instead of `pthread_create`).

There are two programs for you complete in this lab, **counter.c** and **array.c**.

---

**counter.c** 

This program is is a buggy program that attempts to have two threads each increment shared counter 10 million times. If it worked properly, we would expect the counter to have a final value of 20 million.

Build using `make` and run the compiled program `./counter` a few times to see how it behaves without any changes.
Notice the incorrect
result and how it generally produces a different result for each execution.

Your job is to add appropriate synchronization from the thread library to the routine/function `thread_count()` so that correct results are obtained as well as initialize the lock in the `main()` function.

An example wrong output is shown below:
```
A: begin
pid 34715 tid 34716
B: begin
pid 34715 tid 34717
34717 done
34716 done
main: result 10259435
```
Notice how the last line of the output is 10259435 when the counter should be 20000000

**Hints**:
- You will need to share a lock between all threads. The Pthread library function for initializing a lock, `Pthread_mutex_init()`, requires a pointer to a `pthread_mutex_t` 
struct to store information about the lock (you can just use NULL for the second argument). The struct should have the correct scope so that all threads
share the same lock struct.
- You should initialize the lock in the main function (there is a code comment in the file)
- You can just put `NULL` as the second argument to `Pthread_mutex_init()`.
- See chapter 29.1 of OSTEP (linked from the course calendar) for a walkthrough of synchronized counters. You don't need to worry about efficiency--feel free to implement the slow solution.

---

**array.c**

This is a program that updates entries in an array. You need to fill in
the function `array_update` and complete `main` to spawn the threads.  `main` should spawn
**four threads** arunning `array_update` and **wait** for all of them to complete. There are no arguments used within
`array_update` so you can pass `NULL` for your argument pointer.

Within `array_update` you should repeat the following 10000 times:
1. obtain an index by calling `get_index`.
2. increment the corresponding array entry for that index.

Your program should be correct **and** have reasonably good performance, which means
don't use a single big lock! We check for that.

**Hint**: When is it okay for two threads to write to the array at the same time,
and when is it not okay? What is the smallest/most specific case in which a race condition
could occur and invalidate the program's output? Make a lock for each of these
race conditions. (You should have SIZE number of locks)

Sample output:
```
pid 2243014 tid 2243015
pid 2243014 tid 2243016
pid 2243014 tid 2243017
pid 2243014 tid 2243018
2243015 done
2243016 done
2243018 done
2243017 done
0: 26
1: 32
  .
  .
  .
1248: 28
1249: 31
main: result 40000
```

**Local Testing and Submission**

to run the test harness use the following command
```bash
python3 test_kit.py ALL
```
submit your completed counter.c and array.c to gradescope
