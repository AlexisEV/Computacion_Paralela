package main

import (
	"fmt"
	"time"
)

func multiplicarMatrices(N int) {

	A := make([][]float64, N)
	B := make([][]float64, N)
	C := make([][]float64, N)
	for i := 0; i < N; i++ {
		A[i] = make([]float64, N)
		B[i] = make([]float64, N)
		C[i] = make([]float64, N)
		for j := 0; j < N; j++ {
			A[i][j] = 1.0
			B[i][j] = 2.0
			C[i][j] = 0.0
		}
	}

	start := time.Now()

	for i := 0; i < N; i++ {
		for j := 0; j < N; j++ {
			for k := 0; k < N; k++ {
				C[i][j] += A[i][k] * B[k][j]
			}
		}
	}

	elapsed := time.Since(start)
	fmt.Printf("matriz %dx%d - tiempo: %f segundos\n", N, N, elapsed.Seconds())
}

func main() {
	fmt.Println("mult clasica en Go")

	tams := []int{1000}

	for _, N := range tams {
		multiplicarMatrices(N)
	}
}
