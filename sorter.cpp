#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include "pagedarray.cpp" 

using namespace std;

void quickSort(PagedArray &arr, int left, int right) {
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
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
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        cerr << "Uso: sorter -input <RUTA DEL ARCHIV INPUT> -output <RUTA DEL ARCHIVO OUTPUT> -alg <ALGORITMO>\n";
        return 1;
    }

    string inputPath = argv[2];
    string outputPath = argv[4];
    string algorithm = argv[6];

    ifstream inFile(inputPath, ios::binary);
    if (!inFile) {
        cerr << "ERROR AL ABRIR input file.\n";
        return 1;
    }

    inFile.seekg(0, ios::end);
    size_t fileSize = inFile.tellg();
    size_t numIntegers = fileSize / sizeof(int);

    inFile.seekg(0, ios::beg);
    ofstream tempFile("temp.bin", ios::binary);
    tempFile << inFile.rdbuf();
    inFile.close();
    tempFile.close();

    PagedArray arr("temp.bin");

    auto start = chrono::high_resolution_clock::now();

    if (algorithm == "QS") {
        quickSort(arr, 0, numIntegers - 1);
    } else if (algorithm == "IS") {
        insertionSort(arr, numIntegers);
    } else if (algorithm == "BS") {
        bubbleSort(arr, numIntegers);
    } else {
        cerr << "ALGORITMO INVALIDO. Usar QS, IS, or BS.\n";
        return 1;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    ofstream outFile(outputPath);
    if (!outFile) {
        cerr << "Error opening output file.\n";
        return 1;
    }

    for (size_t i = 0; i < numIntegers; ++i) {
        outFile << arr[i] << "\n";
    }

    outFile.close();
    remove("temp.bin");

    cout << "Finalizado.\n";
    cout << "Tiempo de ejecucion: " << elapsed.count() << " seconds\n";
    cout << "Algoritmo usado: " << algorithm << "\n";
    cout << "Page faults: " << arr.getPageFaults() << "\n";
    cout << "Page hits: " << arr.getPageHits() << "\n";

    return 0;
}
