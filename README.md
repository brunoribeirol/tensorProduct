<p align="center">
  <img
    src="https://img.shields.io/badge/Status-Finished%20-green?style=flat-square"
    alt="Status"
  />
</p>

<p align="center">
  <img
    src="https://img.shields.io/github/repo-size/brunoribeirol/tensorProduct?style=flat"
    alt="Repository Size"
  />
  <img
    src="https://img.shields.io/github/languages/count/brunoribeirol/tensorProduct?style=flat&logo=python"
    alt="Language Count"
  />
  <img
    src="https://img.shields.io/github/commit-activity/t/brunoribeirol/tensorProduct?style=flat&logo=github"
    alt="Commit Activity"
  />
  <a href="LICENSE.md"
    ><img
      src="https://img.shields.io/github/license/brunoribeirol/tensorProduct"
      alt="License"
  /></a>
</p>

# Tensor Product of Matrices

## 👀 Overview

This project implements a multithreaded program in C to compute the tensor product of two matrices. The program takes the names of two files containing matrices as command-line arguments, calculates their tensor product, and writes the result to a file named `tensor.out`. The implementation ensures efficient computation using at least two parallel threads.

## 📝 Implementation Details

1. Ensure you are on a Linux, Unix, or macOS system.
2. Ensure you're using minimum of 2 threads for processing the tensor product (multithreading)

## 🛠️ Build Instructions

- Compile the program using the command:
```bash
make
```
      
## ⚙️ Execution

- Run the compiled executable with the input file:
```bash
./tensor [namefile1] [namefile2]
```
- namefile1: Name of the first input file containing the first matrix
- namefile2: Name of the second input file containing the second matrix

## 📥 Example Input

- `matrix1.txt`
```bash
1 2
3 4
1 0
```

- `matrix2.txt`
```bash
0 5 2
6 7 3
```

## 📤 Output File

The output file should be names as `tensor.out`. The format follows strict guidelines as provided in the assignment.
```bash
 0  5 2  0 10  4
 6  7 3 12 14  6
 0 15 6  0 20  8
18 21 9 24 28 12
 0  5 2  0  0  0
 6  7 3  0  0  0
```
