#include <iostream>
#include <vector>
#include <chrono>

void multiplicarMatrices(int N) {
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 1.0));
    std::vector<std::vector<double>> B(N, std::vector<double>(N, 2.0));
    std::vector<std::vector<double>> C(N, std::vector<double>(N, 0.0));

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "matriz " << N << "x" << N << " - tiempo: " << duration.count() << " segundos\n";
}

int main() {
    std::cout << "mult clasica en c++\n";

    std::vector<int> tams = {1000};
    
    for(int N : tams) {
        multiplicarMatrices(N);
    }

    return 0;
}