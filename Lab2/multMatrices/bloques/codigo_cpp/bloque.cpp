#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

void multiplicarBloques(int N) {
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 1.0));
    std::vector<std::vector<double>> B(N, std::vector<double>(N, 2.0));
    std::vector<std::vector<double>> C(N, std::vector<double>(N, 0.0));

    // tam del bloque
    int blockSize = 32; 

    auto start = std::chrono::high_resolution_clock::now();

    // bucles externos
    for (int jj = 0; jj < N; jj += blockSize) {
        for (int kk = 0; kk < N; kk += blockSize) {
            for (int ii = 0; ii < N; ii += blockSize) {
                
                // bucles internos - dentro del bloque actuall
                for (int i = ii; i < std::min(ii + blockSize, N); i++) {
                    for (int j = jj; j < std::min(jj + blockSize, N); j++) {
                        for (int k = kk; k < std::min(kk + blockSize, N); k++) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }

            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "matriz " << N << "x" << N << " - tiempo: " << duration.count() << " segundos\n";
}

int main() {
    std::cout << "mult por bloques en c++\n";
    
    std::vector<int> tamanos = {100, 500, 1000, 1500};
    
    for(int N : tamanos) {
        multiplicarBloques(N);
    }

    return 0;
}