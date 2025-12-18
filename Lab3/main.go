// Full Name: Ayma Rehman
// Student ID: 241ADB165
// Course: DE0917 - Programming Languages

package main

import (
	"bufio"
	"flag"
	"fmt"
	"math/rand"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
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

func generateRandomNumbers(n int) []int {
	rand.Seed(time.Now().UnixNano())

	data := make([]int, n)
	for i := 0; i < n; i++ {
		data[i] = rand.Intn(1000) // range: 0–999
	}
	return data
}

func readInputFile(filename string) ([]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, fmt.Errorf("cannot open file %s", filename)
	}
	defer file.Close()

	var data []int
	scanner := bufio.NewScanner(file)
	lineNumber := 0

	for scanner.Scan() {
		lineNumber++
		line := strings.TrimSpace(scanner.Text())

		if line == "" {
			continue
		}

		value, err := strconv.Atoi(line)
		if err != nil {
			return nil, fmt.Errorf("invalid integer on line %d", lineNumber)
		}

		data = append(data, value)
	}

	if err := scanner.Err(); err != nil {
		return nil, err
	}

	return data, nil
}

func writeOutputFile(filename string, data []int) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	for _, v := range data {
		fmt.Fprintln(writer, v)
	}
	return writer.Flush()
}

func processDirectory(dir string) error {
	info, err := os.Stat(dir)
	if err != nil || !info.IsDir() {
		return fmt.Errorf("invalid directory")
	}

	outputDir := dir + "_sorted_Ayma_Rehman_241ADB165"
	if err := os.MkdirAll(outputDir, 0755); err != nil {
		return err
	}

	entries, err := os.ReadDir(dir)
	if err != nil {
		return err
	}

	for _, entry := range entries {
		if entry.IsDir() || filepath.Ext(entry.Name()) != ".txt" {
			continue
		}

		inputPath := filepath.Join(dir, entry.Name())
		data, err := readInputFile(inputPath)
		if err != nil || len(data) < 10 {
			continue
		}

		chunks := splitIntoChunks(data, calculateChunkCount(len(data)))
		sortChunksConcurrently(chunks)
		sorted := mergeSortedChunks(chunks)

		outputPath := filepath.Join(outputDir, entry.Name())
		if err := writeOutputFile(outputPath, sorted); err != nil {
			return err
		}
	}

	return nil
}

func main() {
	randCount := flag.Int("r", -1, "number of random integers to sort (must be >= 10)")
	inputFile := flag.String("i", "", "input file containing integers")
	dir := flag.String("d", "", "directory containing .txt files")
	flag.Parse()

	if flag.NArg() != 0 {
		fmt.Println("Error: invalid extra arguments")
		os.Exit(1)
	}

	modes := 0
	if *randCount != -1 {
		modes++
	}
	if *inputFile != "" {
		modes++
	}
	if *dir != "" {
		modes++
	}

	if modes != 1 {
		fmt.Println("Error: specify exactly one mode: -r, -i, or -d")
		os.Exit(1)
	}

	if *dir != "" {
		if err := processDirectory(*dir); err != nil {
			fmt.Println("Error:", err)
			os.Exit(1)
		}
		return
	}

	var data []int
	var err error

	if *randCount != -1 {
		if *randCount < 10 {
			fmt.Println("Error: N must be at least 10")
			os.Exit(1)
		}
		data = generateRandomNumbers(*randCount)
	} else {
		data, err = readInputFile(*inputFile)
		if err != nil || len(data) < 10 {
			fmt.Println("Error: invalid input file")
			os.Exit(1)
		}
	}

	fmt.Println("Original numbers:")
	fmt.Println(data)

	chunks := splitIntoChunks(data, calculateChunkCount(len(data)))

	fmt.Println("\nChunks before sorting:")
	for i, c := range chunks {
		fmt.Printf("Chunk %d: %v\n", i, c)
	}

	sortChunksConcurrently(chunks)

	fmt.Println("\nChunks after sorting:")
	for i, c := range chunks {
		fmt.Printf("Chunk %d: %v\n", i, c)
	}

	fmt.Println("\nFinal merged sorted result:")
	fmt.Println(mergeSortedChunks(chunks))
}
