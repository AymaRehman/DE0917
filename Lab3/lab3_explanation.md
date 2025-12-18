# Lab 3: Concurrent Chunk Sorting in Go


> This is a README style document that explains:
> - How to run each mode
> - Any design decisions


## Student Information
- Name: Ayma Rehman
- Student ID: 241ADB165
- Course: DE0917 – Programming Languages

---

## Description
`gosort` is a Go command-line program that sorts integers using concurrent chunk-based sorting.

The program follows these steps:
1. Split input numbers into chunks
2. Sort each chunk concurrently using goroutines
3. Merge the sorted chunks into one globally sorted result

Chunking rules:
- Minimum number of chunks: 4
- Otherwise: ceil(sqrt(n))
- Chunks differ in size by at most one element

---

## How to Run

### Build
```
go build
```
### Mode 1: Random Numbers (-r)
Generates and sorts random integers.
```
go run . -r N
```
Notes:
- N must be an integer
- N must be at least 10
- Random number range: 0–999

### Mode 2: Input File (-i)
Sorts integers read from a text file.
```
go run . -i input.txt
```
Input file rules:
- One integer per line
- Empty lines are ignored
- Invalid lines cause an error
- At least 10 valid integers required

### Mode 3: Directory Mode (-d)
Sorts all `.txt` files in a directory independently.
```
go run . -d incoming
```
Behavior:
- Only `.txt` files are processed
- Each file is sorted independently using concurrent chunk sorting
- No console output for individual files
- A new output directory is created automatically

Output directory format:
```
incoming_sorted_Ayma_Rehman_241ADB165
```
Sorted files:
- Keep original filenames
- Contain sorted integers (one per line)

### Design Notes
- Sorting is performed concurrently using goroutines and sync.WaitGroup
- `sort.Ints` is used for chunk sorting
- A manual k-way merge is implemented to merge sorted chunks
- No flattening and re-sorting is performed
- All error cases are handled cleanly

