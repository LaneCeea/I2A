# Algorithms Implementation and Analysis

This repository contains my implementations of algorithms from "Introduction to Algorithms" (3rd edition) by Cormen, Leiserson, Rivest, and Stein (CLRS). Each algorithm is implemented in C with accompanying mathematical analysis written in LaTeX.

## Structure

Each algorithm is organized in its own directory containing:
- `main.c` - C implementation of the algorithm
- `analysis.tex` - LaTeX source for mathematical analysis (correctness and complexity)
- `analysis.pdf` - Compiled analysis document

The `utils/` folder contains helper data structures and utility functions used across different algorithms.

## Prerequisites

- **C Compiler**: GCC (GNU Compiler Collection)
- **Standard**: C11
- **Build System**: Make
- **Platform**: Linux (Windows support coming soon)
- **LaTeX**: For compiling analysis documents (optional, PDFs are included)

## Building and Running

### Compile an Algorithm

To build a specific algorithm, use:

```bash
make {directory-name}
```

The executable will be created in the `bin/` directory.

### Example: Insertion Sort

```bash
# Compile insertion sort
make 02-insertion-sort

# Run the program
./bin/02-insertion-sort
```

### Build All Algorithms

```bash
make all
```

### Clean Build Files

```bash
make clean
```

## Utils

The `utils/` directory contains shared utilities:
- Data structures
- Logging functions
- Common helper functions

Object files for utilities are compiled to `obj/` and linked automatically when building algorithms.

## Project Goals

This is a self-learning project focused on:
- Understanding algorithm design and analysis
- Implementing algorithms in clean, readable C code
- Formal mathematical analysis of correctness and complexity
- Building a comprehensive reference for classic algorithms

## Current Status

The repository is actively being expanded with new algorithm implementations. Each addition includes both working code and theoretical analysis.