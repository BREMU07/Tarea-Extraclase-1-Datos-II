#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include "pagedarray.cpp"  // Asegúrate de tener una implementación correcta de PagedArray

void quickSort(PagedArray &arr, int left, int right) {
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

void insertionSort(PagedArray &arr, int size) {
    for (int i = 1; i < size; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void bubbleSort(PagedArray &arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        std::cerr << "Modo de uso: sorter -input <RUTA DEl ARCHIVO DE ENTRADA> -output <RUTA DEL ARCHIVO DE SALIDA> -alg <ALGORITMO>\n";
        return 1;
    }

    std::string inputPath = argv[2];
    std::string outputPath = argv[4];
    std::string algorithm = argv[6];

    std::ifstream inFile(inputPath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error abriendo el archivo de entrada.\n";
        return 1;
    }

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    size_t numIntegers = fileSize / sizeof(int);

    inFile.seekg(0, std::ios::beg);
    std::ofstream tempFile("temp.bin", std::ios::binary);
    tempFile << inFile.rdbuf();
    inFile.close();
    tempFile.close();

    PagedArray arr("temp.bin");

    auto start = std::chrono::high_resolution_clock::now();

    if (algorithm == "QS") {
        quickSort(arr, 0, numIntegers - 1);
    } else if (algorithm == "IS") {
        insertionSort(arr, numIntegers);
    } else if (algorithm == "BS") {
        bubbleSort(arr, numIntegers);
    } else {
        std::cerr << "Algoritmo invalido. Utilice QS, IS, o BS.\n";
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::ofstream outFile(outputPath);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo de salida.\n";
        return 1;
    }

    for (size_t i = 0; i < numIntegers; ++i) {
        outFile << arr[i];
        if (i < numIntegers - 1) {
            outFile << ",";  // Agrega una coma después de cada número excepto el último
        }
    }

    outFile.close();
    std::remove("temp.bin");

    std::cout << "Ejecucion terminada.\n";
    std::cout << "Tiempo de ejecucion: " << elapsed.count() << " seconds\n";
    std::cout << "Algoritmo utilizado: " << algorithm << "\n";
    std::cout << "Page faults: " << arr.getPageFaults() << "\n";
    std::cout << "Page hits: " << arr.getPageHits() << "\n";

    return 0;
}
