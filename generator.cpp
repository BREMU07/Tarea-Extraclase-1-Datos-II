#include <iostream>
#include <fstream>
#include <random>
#include <cstring>

using namespace std;

void generateFile(const string &filePath, size_t sizeInMB) {
    ofstream outFile(filePath, ios::binary);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(numeric_limits<int>::min(), numeric_limits<int>::max());

    size_t numIntegers = (sizeInMB * 1024 * 1024) / sizeof(int);

    for (size_t i = 0; i < numIntegers; ++i) {
        int num = dis(gen);
        outFile.write(reinterpret_cast<char*>(&num), sizeof(int));
    }
    outFile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Modo de uso: generator -size <TAMAÑO> -output <RUTA DEL ARCHIVO DE SALIDA>\n";
        return 1;
    }

    size_t sizeInMB;
    string sizeArg = argv[2];
    string filePath = argv[4];

    if (sizeArg == "SMALL") {
        sizeInMB = 512;
    } else if (sizeArg == "MEDIUM") {
        sizeInMB = 1024;
    } else if (sizeArg == "LARGE") {
        sizeInMB = 2048;       
    } else if (sizeArg == "CUSTOM") {
        sizeInMB = 1;
    } else {
        cerr << "Argumento de tamaño invalido. Utilice SMALL, MEDIUM, or LARGE.\n";
        return 1;
    }

    generateFile(filePath, sizeInMB);

    return 0;
}
