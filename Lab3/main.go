// Full Name: Ayma Rehman
// Student ID: 241ADB165
// Course: DE0917 - Programming Languages

package main

import (
	"fmt"
	"sort"
	"sync"
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
	data := []int{42, 7, 19, 3, 25, 1, 9, 30, 15, 8, 12, 6}

	fmt.Println("Original data:")
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
