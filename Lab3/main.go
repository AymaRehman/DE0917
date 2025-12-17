// Full Name: Ayma Rehman
// Student ID: 241ADB165
// Course: DE0917 - Programming Languages

package main

import (
	"flag"
	"fmt"
	"math/rand"
	"os"
	"sort"
	"sync"
	"time"
)


func calculateChunkCount(n int) int {
	if n <= 0 {
		return 0
	}

	minChunks := 4
	chunks := intCeilSqrt(n)

	if chunks < minChunks {
		return minChunks
	}
	return chunks
}

func intCeilSqrt(n int) int {
	x := 1
	for x*x < n {
		x++
	}
	return x
}

func splitIntoChunks(data []int, chunks int) [][]int {
	result := make([][]int, 0, chunks)

	n := len(data)
	baseSize := n / chunks
	remainder := n % chunks

	start := 0
	for i := 0; i < chunks; i++ {
		size := baseSize
		if i < remainder {
			size++
		}

		end := start + size
		if end > n {
			end = n
		}

		result = append(result, data[start:end])
		start = end
	}

	return result
}

func sortChunksConcurrently(chunks [][]int) {
	var wg sync.WaitGroup

	for i := range chunks {
		wg.Add(1)

		go func(idx int) {
			defer wg.Done()
			sort.Ints(chunks[idx])
		}(i)
	}

	wg.Wait()
}

func mergeSortedChunks(chunks [][]int) []int {
	totalSize := 0
	for _, chunk := range chunks {
		totalSize += len(chunk)
	}

	result := make([]int, 0, totalSize)
	indices := make([]int, len(chunks))

	for {
		minValue := 0
		minChunk := -1

		for i := range chunks {
			if indices[i] >= len(chunks[i]) {
				continue
			}

			val := chunks[i][indices[i]]
			if minChunk == -1 || val < minValue {
				minValue = val
				minChunk = i
			}
		}

		if minChunk == -1 {
			break
		}

		result = append(result, minValue)
		indices[minChunk]++
	}

	return result
}

func main() {
	randCount := flag.Int("r", -1, "number of random integers to sort (must be >= 10)")
	flag.Parse()

	if *randCount == -1 {
		fmt.Println("Error: -r flag is required")
		fmt.Println("Usage: gosort -r N")
		os.Exit(1)
	}

	if *randCount < 10 {
		fmt.Println("Error: N must be at least 10")
		os.Exit(1)
	}

	data := generateRandomNumbers(*randCount)

	fmt.Println("Original numbers:")
	fmt.Println(data)

	chunkCount := calculateChunkCount(len(data))
	chunks := splitIntoChunks(data, chunkCount)

	fmt.Println("\nChunks before sorting:")
	for i, c := range chunks {
		fmt.Printf("Chunk %d: %v\n", i, c)
	}

	sortChunksConcurrently(chunks)

	fmt.Println("\nChunks after sorting:")
	for i, c := range chunks {
		fmt.Printf("Chunk %d: %v\n", i, c)
	}

	merged := mergeSortedChunks(chunks)

	fmt.Println("\nFinal merged sorted result:")
	fmt.Println(merged)
}

func generateRandomNumbers(n int) []int {
	rand.Seed(time.Now().UnixNano())

	data := make([]int, n)
	for i := 0; i < n; i++ {
		data[i] = rand.Intn(1000) // range: 0–999
	}
	return data
}
