# Real Time Operating System

A real-time operating system developed in C for the ARM Cortex-M4 microcontroller.

[Design document](https://github.com/ShreyasPeddi/Real-Time-Operating-System/blob/master/ECE350_W25_Lab_Manual.pdf)

## Features

- 🧵 **Multitasking Kernel**

  - Cooperative context switching
  - Preemptive scheduling via timer interrupts
  - **Round-robin (RR)** and **Earliest Deadline First (EDF)** scheduling

- 📋 **Task Management**

  - Dynamic task creation
  - Dynamic stack allocation
  - Task sleeping and deadline modification
  - Null task handling and task exit management

- 🧠 **Memory Management**

  - Custom heap allocator using **Binary Buddy** algorithm
  - Support for dynamic memory allocation and deallocation
  - Coalescing of freed blocks
  - External fragmentation analysis

## License

Please do not copy or distribute without permission.
