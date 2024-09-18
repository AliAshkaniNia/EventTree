# Event Tree

This repository contains a solution to a coding challenge. The problem description can be found at [problem.md](./docs/problem.md). You can find a report on the progress of the project at [report.md](./docs/report.md).

# Building the Project 
To build the project, execute the following commands in your terminal:

```bash
mkdir build 
cd build 
cmake ..
cmake --build . 
```

## Dependencies 

This project has a number of dependencies on other libraries, but you do not have to install them manually. If previously installed, they will be used, otherwise CMake will try to automatically fetch and compile them for you (via `FetchContent`), so you do not have to install anything.

Here are the libraries used in this project:

- [GoogleTest](https://github.com/google/googletest): Used for unit testing 
- [eventpp](https://github.com/wqking/eventpp/): A production-ready event handling library
- [TBB](https://github.com/oneapi-src/oneTBB): Used for `concurrent_unordered_map`
- [concurrentqueue](https://github.com/cameron314/concurrentqueue): Used as a lock-free queue provider
- [benchmark](https://github.com/google/benchmark): Used to implement benchmarks

A C++20 compiler is required to compile this project, as it utilizes C++20 features such as concepts.

This program was tested with CMake version `3.28.3` and Clang version `18.1.8` on Ubuntu 24.04 operating system. 


## Execution

This program creates several executables. You can run them as follows:

### Demo
`EventTree` is a demo program simulating a solution to the [problem](./docs/problem.md). You can run it with the following command: 
```bash
./EventTree
```

### Tests 
Testcases to test `SpecialEventQueue`, the implemented event queue library, can be executed with this command: 
```bash
./SpecialQueueTest
```

### Benchmarks
There are 3 benchmarks in this project. You can run them with:

```bash
./bm_dispatch
./bm_enqueue
./bm_fairness
```

### Static Analysis 
`clang-tidy` is used as the static analysis tool. You can run it by using this command:
```bash 
cmake --build . --target tidy
```
`clang-format` is used as the formatter in this project. The project is configured to invoke this program every time you issue a build command, so there's no need to call it explicitly.


