
COMP.CS.310 Rinnakkaisuus / Concurrency

Pipeline Processing with C++ Threads
This project demonstrates pipeline processing using C++ threads and synchronization mechanisms. 
The pipeline consists of four units (A, B, C, and D) connected sequentially, processing integer data.

Learning Goals:
Create multiple threads of execution in C++.
Synchronize threads using C++11 mechanisms.
Implement pipeline processing.

Functionality:
Each unit processes integer data (uint64_t) and performs specific operations.
Units communicate through a shared data structure.
Synchronization mechanisms ensure proper coordination between units.

Requirements:
C++20 (or C++11/C++17) compiler.
config.hh file defining configuration constants.
Compile with -pthread flag.

Usage:
Modify config.hh to set configuration constants.
Compile the code with g++ -pthread main.cpp -o main.
Run the executable with ./main.



