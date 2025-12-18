# DE0917 – Lab 3 Report  
**Concurrent Chunk Sorting in Go (gosort)**

**Author:** Ayma Rehman  
**Student ID:** 241ADB165  

---

## Aim of the Assignment

The goal of this assignment was to implement a Go command-line program called `gosort` that sorts integers using concurrent chunk-based sorting.

The core objectives were to:
- Split input integers into multiple chunks based on defined rules
- Sort each chunk concurrently using goroutines
- Merge the sorted chunks into one globally sorted result
- Support multiple input modes via command-line arguments
- Apply correct concurrency, chunking, merging, and error handling

---

## Overview of Implementation

The program follows the required **Split → Sort → Merge** workflow:

1. Input integers are divided into chunks  
2. Each chunk is sorted concurrently using a goroutine  
3. A manual k-way merge algorithm is used to merge sorted chunks  
4. The program supports three execution modes:
   - Random number generation (`-r`)
   - Input file processing (`-i`)
   - Directory processing (`-d`)

Concurrency is implemented using `sync.WaitGroup`, and sorting is performed with `sort.Ints`.

---

## Features Implemented (Rubric Self-Assessment)

The table below maps implemented features directly to the grading rubric.

| Grade | Requirement | Status |
|------|-------------|--------|
| 7 | Correct concurrent chunk sorting logic | ✅ |
| 8 | Random numbers mode (`-r`) with full output | ✅ |
| 9 | Input file mode (`-i`) with validation | ✅ |
| 10 | Directory mode (`-d`) with batch processing | ✅ |

## Design Decisions

- **Concurrency:** Implemented using goroutines and `sync.WaitGroup` to ensure real parallel execution.
- **Sorting:** Used Go’s standard `sort.Ints` for clarity and reliability.
- **Merging:** Implemented a manual k-way merge to comply with assignment rules.
- **Error Handling:** All invalid inputs and incorrect usage scenarios are handled with clear error messages.
- **CLI Design:** Enforced exactly one mode (`-r`, `-i`, or `-d`) per execution.

---

## Resources

The following resources were consulted for guidance during development:

- Go Official Documentation    
https://pkg.go.dev/sort  
https://pkg.go.dev/sync  
https://pkg.go.dev/flag  
- Go by Example – Goroutines and WaitGroups  
https://gobyexample.com/
- Course lectures, labs, and assignment specification

> This project is submitted as part of a university assignment. All code is authored by Ayma Rehman.