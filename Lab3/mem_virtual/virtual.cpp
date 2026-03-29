#define _DEFAULT_SOURCE
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>

using namespace std;
using namespace std::chrono;

struct Metricas{
    long long tiempo_ms{};
    long fallos_menores{};
    long fallos_mayores{};
    long bloques_entrada{};
};

bool leer_uso(rusage &uso){
    return getrusage(RUSAGE_SELF, &uso) == 0;
}

size_t obtener_tamano_pagina(){
    long tp = sysconf(_SC_PAGESIZE);

    if (tp <= 0){
        return 4096;
    }

    return static_cast<size_t>(tp);
}

size_t contar_paginas_residentes(void *dir, size_t longitud, size_t tamano_pagina){

    size_t paginas = (longitud + tamano_pagina - 1) / tamano_pagina;
    vector<unsigned char> vec(paginas, 0);

    if (mincore(dir, longitud, vec.data()) != 0){
        cerr << "Aviso: mincore fallo: " << strerror(errno) << "\n";
        return 0;
    }

    size_t residentes = 0;

    for (unsigned char b : vec){
        if (b & 1){
            residentes++;
        }
    }

    return residentes;
}

Metricas leer_un_byte_por_pagina(const char *mapa, size_t longitud, size_t tamano_pagina, volatile uint64_t &suma_verif){

    rusage antes{}, despues{};

    if (!leer_uso(antes)){
        cerr << "Error en getrusage() antes: " << strerror(errno) << "\n";
    }

    auto inicio = high_resolution_clock::now();

    for (size_t i = 0; i < longitud; i += tamano_pagina){
        suma_verif += static_cast<unsigned char>(mapa[i]);
    }

    auto fin = high_resolution_clock::now();

    if (!leer_uso(despues)){

        cerr << "Error en getrusage() despues: " << strerror(errno) << "\n";
    }

    Metricas m;

    m.tiempo_ms = duration_cast<milliseconds>(fin - inicio).count();
    m.fallos_menores = despues.ru_minflt - antes.ru_minflt;
    m.fallos_mayores = despues.ru_majflt - antes.ru_majflt;
    m.bloques_entrada = despues.ru_inblock - antes.ru_inblock;

    return m;
}

int main(int argc, char *argv[]){

    if (argc != 2){
        cerr << "Uso: " << argv[0] << " <archivo>\n";
        return 1;
    }

    const string ruta = argv[1];
    const size_t tamano_pagina = obtener_tamano_pagina();

    int fd = open(ruta.c_str(), O_RDONLY);

    if (fd < 0){
        cerr << "No se pudo abrir el archivo: " << strerror(errno) << "\n";
        return 1;
    }

    struct stat estado{};

    if (fstat(fd, &estado) != 0){
        cerr << "fstat fallo: " << strerror(errno) << "\n";
        close(fd);
        return 1;
    }

    if (estado.st_size <= 0){
        cerr << "El archivo esta vacio.\n";
        close(fd);
        return 1;
    }

    size_t longitud = static_cast<size_t>(estado.st_size);
    void *mapeo = mmap(nullptr, longitud, PROT_READ, MAP_PRIVATE, fd, 0);

    if (mapeo == MAP_FAILED){
        cerr << "mmap fallo: " << strerror(errno) << "\n";
        close(fd);
        return 1;
    }

    const char *mapa = static_cast<const char *>(mapeo);
    volatile uint64_t suma_verif = 0;

    size_t residentes_antes = contar_paginas_residentes(mapeo, longitud, tamano_pagina);
    Metricas primera = leer_un_byte_por_pagina(mapa, longitud, tamano_pagina, suma_verif);
    size_t residentes_despues_primera = contar_paginas_residentes(mapeo, longitud, tamano_pagina);
    Metricas segunda = leer_un_byte_por_pagina(mapa, longitud, tamano_pagina, suma_verif);
    size_t residentes_despues_segunda = contar_paginas_residentes(mapeo, longitud, tamano_pagina);

    cout << "Archivo : " << ruta << "\n";
    cout << "Tamano : " << (longitud / (1024 * 1024)) << " MB\n";
    cout << "Tamano de pagina : " << tamano_pagina << " bytes\n\n";

    size_t paginas_totales = (longitud + tamano_pagina - 1) / tamano_pagina;

    cout << "Paginas totales : " << paginas_totales << "\n";
    cout << "Residentes antes : " << residentes_antes << "\n";
    cout << "Residentes tras 1ra : " << residentes_despues_primera << "\n";
    cout << "Residentes tras 2da : " << residentes_despues_segunda << "\n\n";
    cout << "Primera pasada\n";

    cout << " Tiempo : " << primera.tiempo_ms << " ms\n";
    cout << " minflt : " << primera.fallos_menores << "\n";
    cout << " majflt : " << primera.fallos_mayores << "\n";
    cout << " inblock : " << primera.bloques_entrada << "\n\n";

    cout << "Segunda pasada\n";

    cout << " Tiempo : " << segunda.tiempo_ms << " ms\n";
    cout << " minflt : " << segunda.fallos_menores << "\n";
    cout << " majflt : " << segunda.fallos_mayores << "\n";
    cout << " inblock : " << segunda.bloques_entrada << "\n\n";
    cout << "Checksum : " << suma_verif << "\n\n";

    
    if (munmap(mapeo, longitud) != 0){
        cerr << "munmap fallo: " << strerror(errno) << "\n";
    }

    close(fd);
    return 0;
}