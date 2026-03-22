package main

import (
	"fmt"
	"time"
)

func multiplicarBloques(N int) {
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
	//tam bloque
	blockSize := 64

	start := time.Now()

	// bucles externos
	for jj := 0; jj < N; jj += blockSize {
		for kk := 0; kk < N; kk += blockSize {
			for ii := 0; ii < N; ii += blockSize {

				// bucles intrnos: dentro del bloque actual
				for i := ii; i < min(ii+blockSize, N); i++ {
					for j := jj; j < min(jj+blockSize, N); j++ {
						for k := kk; k < min(kk+blockSize, N); k++ {
							C[i][j] += A[i][k] * B[k][j]
						}
					}
				}

			}
		}
	}

	elapsed := time.Since(start)
	fmt.Printf("matriz %dx%d - tiempo: %f segundos\n", N, N, elapsed.Seconds())
}

func main() {
	fmt.Println("mult por bloques en go")

	tamanos := []int{1000}

	for _, N := range tamanos {
		multiplicarBloques(N)
	}
}
