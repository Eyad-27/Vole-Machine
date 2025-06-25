# Vole Machine Simulator

![C++](https://img.shields.io/badge/C++-17-blue?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.30-brightgreen)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

A C++ implementation of a Vole Machine simulator based on OOP principles, developed as part of Cairo University's CS213 Object-Oriented Programming course.

## Features

- 🖥️ Full Vole Machine architecture simulation
- 📝 Supports 15+ Vole Machine instructions
- 🧠 Memory management (256 cells)
- 🔢 Register operations (16 registers)
- 📊 Visual memory display
- 🖨️ Screen output simulation
- 🚦 Step-by-step program execution
- 🔄 Full program execution modes
- ♻️ Reset functionality for CPU and memory

## Supported Instructions

| Op-code | Operation           | Example |
|--------|----------------------|---------|
| 1RXY   | Load from memory     | 10FF    |
| 2RXY   | Load immediate       | 20A3    |
| 3RXY   | Store to memory      | 35B1    |
| 4RS0   | Move register        | 40A4    |
| 5RST   | Integer addition     | 5726    |
| 6RST   | Floating-point add   | 634E    |
| BRXY   | Conditional jump     | B43C    |
| C000   | Halt execution       | C000    |

## Project Structure

├── CMakeLists.txt         # Build configuration  
├── src/  
│   ├── main.cpp           # Program entry point  
│   ├── header.h           # Class declarations  
│   └── class_implementation.cpp  # Method implementations  
├── input.txt              # Sample program  
└── test_files/            # Additional test programs

## Building & Running

### Requirements
- CMake (>= 3.30)
- C++17 compatible compiler

### Build Instructions
mkdir build && cd build cmake .. cmake --build .

### Running the Simulator
./new_vole_machine


## Usage

After launching the simulator:

**Menu:**
1- Load instructions file  
2- Execute 1 instruction  
3- Execute all instructions  
4- Display program status  
5- Reset CPU  
6- Reset Memory  
7- Clear screen  
8- Exit  

**Sample workflow:**
- Load a program file (e.g., input.txt)  
- Execute step-by-step or run entire program  
- Inspect registers/memory status  
- View screen output  

**Sample Output:**
PC: 000A
IR: 5112
Screen: Hello Vole!
Registers:
R0 = 00
R1 = 2A
...
R5 = FF
Memory:
0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
0     00 2A 1B C4 00 00 00 00 00 00 00 00 00 00 00 00
1     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
...
F     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00


## Key OOP Concepts Implemented

- Class inheritance hierarchy  
- Encapsulation of machine components  
- Polymorphic operations  
- Resource management (RAII)  
- Operator overloading  
- Abstract data types  

## Contributors

- [Your Name/Team Name]  
- [Team Member 2]  
- [Team Member 3]  

## License
Developed for Cairo University CS213 - Object Oriented Programming (2024)
