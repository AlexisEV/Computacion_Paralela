package main

import (
	"fmt"
	"time"
)

const MAX = 10000

func main() {
	A := make([][]float64, MAX)
	for i := range A {
		A[i] = make([]float64, MAX)
		for j := range A[i] {
			A[i][j] = 1.0
		}
	}
	x := make([]float64, MAX)
	y := make([]float64, MAX)
	for i := 0; i < MAX; i++ {
		x[i] = 1.0
		y[i] = 0.0
	}

	fmt.Println("ejecutando bucle 1 (go) - filas/columnas...")

	start := time.Now()

	for i := 0; i < MAX; i++ {
		for j := 0; j < MAX; j++ {
			y[i] += A[i][j] * x[j]
		}
	}

	elapsed := time.Since(start)
	fmt.Printf("Tiempo: %f segundos\n", elapsed.Seconds())
}
