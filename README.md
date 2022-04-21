# Synchronized_Threads
## POSIX_Threads
>POSIX Threads provide multiple flows of execution within a process. The threads have their own stacks but share the global data and the heap. So the global variables are visible to multiple threads. Also, the threads need to synchronize their actions so that they jointly realize the overall objectives of the process they belong to. The core problems of concurrent programming, mutual exclusion and synchronization are relevant for threads just like these problems are relevant for multi-process systems.

## Running Programm:
```bash
# Clone the repository
$ git clone https://github.com/MightyArty/Synchronized_Threads.git
# Go into the repository
$ cd Synchronized_Threads
# Open the terminal on Linux
$ Run "make run"
$ Run Server "./server"
$ Run Iclient "./iclient"
```

## Running Test:

```bash
# Clone the repository
$ git clone https://github.com/MightyArty/Synchronized_Threads.git
# Go into the repository
$ cd Synchronized_Threads
# Open the terminal on Linux
$ Run "make run"
$ Run Test "./test"
```

## Cleaning files:
```bash
$ Run "make clean"
```
