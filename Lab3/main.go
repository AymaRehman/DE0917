// Full Name: Ayma Rehman
// Student ID: 241ADB165
// Course: DE0917 - Programming Languages

package main

import "fmt"

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

