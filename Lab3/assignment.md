# Go Programming Assignment: Concurrent Chunk Sorting (`gosort`)

NO DEFENSE NEEDED - just submit this assignment!

## Goal
Implement a Go command-line program called `gosort` that sorts integer numbers **concurrently** using **goroutines**.
The core idea is simple and non-negotiable:

> Split → Sort concurrently → Merge  
  
You will divide the input numbers into chunks, sort each chunk in its own goroutine, then merge the results into one globally sorted slice.

## Core Rules (Apply to all modes)
## Chunking Rules
Let `n` be the number of integers to sort.

1. **Minimum number of chunks:** `4`

2. Otherwise, number of chunks is:
```
ceil(sqrt(n))
```
3. Chunks must be **roughly equal in size** (difference of at most 1 element).

4. **Each chunk must be sorted in its own goroutine.**

5. You may use **any sorting algorithm**(`sort.Ints` is allowed).

6. You must **merge sorted chunks** yourself (do not flatten and re-sort).

## Program Name
```
gosort
```

## Command-Line Modes
### Mode 1: Random Numbers (`-r`)
**Grade: 8**

### Usage
```
gosort -r N
```
### Behavior
- `N` is the number of random integers to generate.

- `N` must be:

    - An integer

    - `N >= 10`

- Generate `N` random integers (range is your choice, but document it).

- Split the numbers into chunks using the rules above.

- Sort each chunk concurrently.

- Merge chunks into one sorted slice.

### Console Output (Required)
1. Original numbers (unsorted)

2. Chunks before sorting

3. Chunks after sorting

4. Final merged sorted result

All steps must be clearly visible.

### Error Handling
If:

- `N` < 10

- `N` is not an integer

- `-r` is used incorrectly

→ Print a **clear error message** and exit.

### Mode 2: Input File (`-i`)
**Grade: 9**

### Usage
```
gosort -i input.txt
```
### Input File Format
- Plain text file
- One integer per line
- Empty lines may be ignored
- Invalid lines must produce an error

### Behavior
- Read integers from the file in given order
- Apply the same chunking, concurrent sorting, and merging logic
- Sorting must be done only via goroutines

### Console Output (Same as `-r`)
1. Original numbers
2. Chunks before sorting
3. Chunks after sorting
4. Final sorted result

### Error Handling
- File not found
- Invalid integers
- Fewer than 10 valid numbers

### Mode 3: Directory Mode (`-d`)
#### Grade: 10

### Usage
```
gosort -d incoming
```
### Behavior
- `incoming` is a directory containing `.txt` files
- Each `.txt` file:
    - One integer per line
    - Sorted independently
- Apply **the same chunking and concurrent sorting logic** to each file
- **No console output** for individual files

### Output Directory
Create a **new sibling directory** with the following exact naming pattern:
```
incoming_sorted_<firstname>_<surname>_<studentID>
```
### Example
```
incoming_sorted_alice_smith_RDB00001
```
- Directory must be created automatically
- Sorted files must:
    - Keep the **original filenames**
    - Contain **only sorted integers**, one per line

### Constraints
- Only `.txt` files are processed
- Other file types are ignored
- Sorting must still be concurrent internally

## Grading Summary
| Grade | Requirements                               |
|-------|--------------------------------------------|
| 7     | Correct concurrent chunk sorting logic (partial implementation) |
| **8**     | `-r` mode fully working                     |
| **9**     | `-r` **and** `-i` modes                           |
| **10**    | All three modes, including directory processing |

## Technical Requirements
- Language: **Go**
- Must use:
    - Goroutines
    - Channels or `sync.WaitGroup`
- Code must:
    - Compile without warnings
    - Handle errors cleanly
    - Be readable and structured
- Hard-coded limits or fake concurrency **→ automatic downgrade**

## Submission Rules
- Submit source code only
- Program must be runnable as:
```
go run .
```
or
```
go build && ./gosort
```
- Include a short `README.md` explaining: 
    - How to run each mode
    - Any design decisions

## Notes (Read Carefully)
- This is **not** about inventing a clever sorting algorithm.
- This **is** about:
    - Correct concurrency
    - Correct chunking
    - Clean merging
    - Proper CLI behavior
- If concurrency is superficial or fake, it will be noticed.

### GRADING and Submission

If you submit on 18th by 23:59 you get FULL CREDIT.  
If you submit on 20th by 23:59 you get -1 grade so instead of 9 (implemented both -r and -i) you would get 8.   
If you submit on 22nd by 23:59 you get -2 grade so instead of 9 you would get 7.  
If your name, surname and student ID is not in first lines of comments - you get -1 GRADE!!!  

