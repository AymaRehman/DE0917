// Full Name: Ayma Rehman
// Student ID: 241ADB165
// Course: DE0917 - Programming Languages

package main

import (
	"fmt"
	"sort"
	"sync"
)


func main() {
	fmt.Println("=================================")
	fmt.Println("Gosort – concurrent chunk sorting")
	fmt.Println("=================================")
}

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
