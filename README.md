# CS333 Operating System - xv6 lab

This repository contains my implementations for the CS333 Operating Systems course, built on top of the MIT xv6 RISC-V operating system. The project is divided into utility enhancements, new system calls, and a custom memory mapping mechanism.

## 🚀 Features Implemented

### Project 1: Utilities & System Calls
*   **Enhanced `wc` Command**: Modified the word-count utility to support specific flags: `-l` (lines), `-w` (words), and `-c` (characters). Fully supports multiple file arguments and standard input (stdin).
*   **Enhanced `cat` Command**: Added a `-n` option to print output with prefixed line numbers. Works with multiple files and piped stdin.
*   **Process Status (`ps`)**: Implemented a new `getprocs()` system call and a user-level `ps` command. It lists all currently running processes, displaying their PID, State, Size, and Name.
*   **System Call Tracing**: Added a `trace` system call that takes an integer bitmask to selectively log system calls as they return (logging PID, syscall name, and return value).

### Project 2: Memory Mapping (`mmap`)
*   **Shared Memory Mechanism**: Implemented simplified `mmap()` and `munmap()` system calls to allow processes to map shared physical pages into their virtual address space.
*   **Process Lifecycle Management**: Modified `fork()` (via `uvmcopy`) to share mapped pages between parent and child processes rather than duplicating them. Modified exit routines (via `uvmunmap`) to clean up memory properly.
*   **Reference Counting**: Safely tracks how many processes are sharing a memory region, only freeing the physical page when the reference count reaches zero.
*   🌟 **Advanced - Multiple Regions**: Extended the baseline requirements to support **multiple shared memory regions** at dynamically determined addresses, rather than being limited to a single fixed memory address.
