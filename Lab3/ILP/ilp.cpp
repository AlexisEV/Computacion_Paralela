#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const long long TOTAL_OPERATIONS = 1000000000LL;

int main() {
    vector<int> test_sizes = {512, 1024, 32768, 131072, 524288, 33554432};
    
    cout << "Iniciando pruebas de ILP con multiples tamanos de vector...\n";
    cout << "Operaciones totales por prueba: " << TOTAL_OPERATIONS << "\n";
    cout << "--------------------------------------------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "Tamano" 
         << setw(12) << "T. Sec" 
         << setw(12) << "T. ILP2" << setw(12) << "Mejora 2"
         << setw(12) << "T. ILP4" << setw(12) << "Mejora 4"
         << setw(12) << "T. ILP8" << setw(12) << "Mejora 8"
         << setw(12) << "T. ILP16" << setw(12) << "Mejora 16" << "\n";
    cout << "--------------------------------------------------------------------------------------------------------------------\n";

    long long global_dummy_sum = 0; 

    for (int size : test_sizes) {
        vector<int> data(size, 2);
        long long iterations = TOTAL_OPERATIONS / size;

        // PRUEBA 1: secuencial
        auto start_seq = high_resolution_clock::now();
        long long sum_seq = 0;
        for (long long iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < size; ++i) {
                sum_seq += data[i]; 
            }
        }
        auto end_seq = high_resolution_clock::now();
        auto time_seq = duration_cast<milliseconds>(end_seq - start_seq).count();

        // PRUEBA 2: ILP (2 acumuladores)
        auto start_ilp2 = high_resolution_clock::now();
        long long s2_0 = 0, s2_1 = 0;
        for (long long iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < size; i += 2) {
                s2_0 += data[i]; s2_1 += data[i+1];
            }
        }
        long long sum_ilp2 = s2_0 + s2_1;
        auto end_ilp2 = high_resolution_clock::now();
        auto time_ilp2 = duration_cast<milliseconds>(end_ilp2 - start_ilp2).count();

        // PRUEBA 3: ILP (4 Acumuladores)
        auto start_ilp4 = high_resolution_clock::now();
        long long s4_0 = 0, s4_1 = 0, s4_2 = 0, s4_3 = 0;
        for (long long iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < size; i += 4) {
                s4_0 += data[i]; s4_1 += data[i+1]; s4_2 += data[i+2]; s4_3 += data[i+3];
            }
        }
        long long sum_ilp4 = s4_0 + s4_1 + s4_2 + s4_3;
        auto end_ilp4 = high_resolution_clock::now();
        auto time_ilp4 = duration_cast<milliseconds>(end_ilp4 - start_ilp4).count();

        // PRUEBA 4: ILP (8 acumuladores)
        auto start_ilp8 = high_resolution_clock::now();
        long long s8_0 = 0, s8_1 = 0, s8_2 = 0, s8_3 = 0, s8_4 = 0, s8_5 = 0, s8_6 = 0, s8_7 = 0;
        for (long long iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < size; i += 8) {
                s8_0 += data[i]; s8_1 += data[i+1]; s8_2 += data[i+2]; s8_3 += data[i+3];
                s8_4 += data[i+4]; s8_5 += data[i+5]; s8_6 += data[i+6]; s8_7 += data[i+7];
            }
        }
        long long sum_ilp8 = s8_0 + s8_1 + s8_2 + s8_3 + s8_4 + s8_5 + s8_6 + s8_7;
        auto end_ilp8 = high_resolution_clock::now();
        auto time_ilp8 = duration_cast<milliseconds>(end_ilp8 - start_ilp8).count();

        // PRUEBA 5: ILP (16 acumuladores)
        auto start_ilp16 = high_resolution_clock::now();
        long long s16_0=0, s16_1=0, s16_2=0, s16_3=0, s16_4=0, s16_5=0, s16_6=0, s16_7=0;
        long long s16_8=0, s16_9=0, s16_10=0, s16_11=0, s16_12=0, s16_13=0, s16_14=0, s16_15=0;
        for (long long iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < size; i += 16) {
                s16_0 += data[i]; s16_1 += data[i+1]; s16_2 += data[i+2]; s16_3 += data[i+3];
                s16_4 += data[i+4]; s16_5 += data[i+5]; s16_6 += data[i+6]; s16_7 += data[i+7];
                s16_8 += data[i+8]; s16_9 += data[i+9]; s16_10 += data[i+10]; s16_11 += data[i+11];
                s16_12 += data[i+12]; s16_13 += data[i+13]; s16_14 += data[i+14]; s16_15 += data[i+15];
            }
        }
        long long sum_ilp16 = s16_0+s16_1+s16_2+s16_3+s16_4+s16_5+s16_6+s16_7 +
                              s16_8+s16_9+s16_10+s16_11+s16_12+s16_13+s16_14+s16_15;
        auto end_ilp16 = high_resolution_clock::now();
        auto time_ilp16 = duration_cast<milliseconds>(end_ilp16 - start_ilp16).count();

        // suma de control
        global_dummy_sum += (sum_seq + sum_ilp2 + sum_ilp4 + sum_ilp8 + sum_ilp16);

        // calculo de mejoras
        double mej2 = 0.0, mej4 = 0.0, mej8 = 0.0, mej16 = 0.0;
        if (time_seq > 0) {
            mej2 = ((double)(time_seq - time_ilp2) / time_seq) * 100.0;
            mej4 = ((double)(time_seq - time_ilp4) / time_seq) * 100.0;
            mej8 = ((double)(time_seq - time_ilp8) / time_seq) * 100.0;
            mej16 = ((double)(time_seq - time_ilp16) / time_seq) * 100.0;
        }

        // imprimir resultados
        cout << left << setw(10) << size 
             << time_seq << setw(12 - to_string(time_seq).length()) << " ms"
             << time_ilp2 << setw(12 - to_string(time_ilp2).length()) << " ms"
             << fixed << setprecision(2) << mej2 << setw(12 - 5) << " %"
             << time_ilp4 << setw(12 - to_string(time_ilp4).length()) << " ms"
             << fixed << setprecision(2) << mej4 << setw(12 - 5) << " %"
             << time_ilp8 << setw(12 - to_string(time_ilp8).length()) << " ms"
             << fixed << setprecision(2) << mej8 << setw(12 - 5) << " %"
             << time_ilp16 << setw(12 - to_string(time_ilp16).length()) << " ms"
             << fixed << setprecision(2) << mej16 << " %\n";
    }

    cout << "--------------------------------------------------------------------------------------------------------------------\n";
    cout << "Suma de control: " << global_dummy_sum << "\n";

    return 0;
}