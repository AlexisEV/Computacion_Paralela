package main

import (
	"fmt"
	"time"
)

const MAX = 10000

func main() {
	// Inicialización idéntica
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

	fmt.Println("ejecutando bucle 2 (go) - columnas/filas...")

	start := time.Now()

	for j := 0; j < MAX; j++ {
		for i := 0; i < MAX; i++ {
			y[i] += A[i][j] * x[j]
		}
	}

	elapsed := time.Since(start)
	fmt.Printf("Tiempo: %f segundos\n", elapsed.Seconds())
}
