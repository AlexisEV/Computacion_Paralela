#include <iostream>
#include <vector>
#include <chrono>

const int MAX = 10000;

int main() {
    
    std::vector<std::vector<double>> A(MAX, std::vector<double>(MAX, 1.0));
    std::vector<double> x(MAX, 1.0);
    std::vector<double> y(MAX, 0.0);

    std::cout << "ejecutando bucle 1 (c++) - filas/columnas..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "Tiempo: " << duration.count() << " segundos" << std::endl;

    return 0;
}