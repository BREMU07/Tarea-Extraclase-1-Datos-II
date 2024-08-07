#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <array>
#include <ctime>

using namespace std;

class PagedArray {
    static const size_t PAGE_SIZE = 1024;
    static const size_t NUM_PAGES = 4;
    string filePath;
    array<vector<int>, NUM_PAGES> pages;
    array<int, NUM_PAGES> pageNumbers;
    unordered_map<int, size_t> pageMap;
    size_t pageFaults = 0;
    size_t pageHits = 0;

public:
    PagedArray(const string &path) : filePath(path) {
        for (auto &page : pages) {
            page.resize(PAGE_SIZE);
        }
        fill(pageNumbers.begin(), pageNumbers.end(), -1);
    }

    int &operator[](size_t index) {
        int pageNumber = index / PAGE_SIZE;
        int offset = index % PAGE_SIZE;
        auto it = pageMap.find(pageNumber);

        if (it != pageMap.end()) {
            pageHits++;
            return pages[it->second][offset];
        } else {
            pageFaults++;
            size_t pageSlot = loadPage(pageNumber);
            return pages[pageSlot][offset];
        }
    }

    size_t getPageFaults() const { return pageFaults; }
    size_t getPageHits() const { return pageHits; }

private:
    size_t loadPage(int pageNumber) {
        size_t pageSlot = rand() % NUM_PAGES;

        if (pageNumbers[pageSlot] != -1) {
            savePage(pageSlot);
        }

        loadPageFromDisk(pageNumber, pageSlot);
        pageNumbers[pageSlot] = pageNumber;
        pageMap[pageNumber] = pageSlot;

        return pageSlot;
    }

    void savePage(size_t pageSlot) {
        int pageNumber = pageNumbers[pageSlot];
        ofstream outFile(filePath, ios::binary | ios::in | ios::out);
        outFile.seekp(pageNumber * PAGE_SIZE * sizeof(int));
        outFile.write(reinterpret_cast<char*>(pages[pageSlot].data()), PAGE_SIZE * sizeof(int));
        outFile.close();
        pageMap.erase(pageNumber);
    }

    void loadPageFromDisk(int pageNumber, size_t pageSlot) {
        ifstream inFile(filePath, ios::binary);
        inFile.seekg(pageNumber * PAGE_SIZE * sizeof(int));
        inFile.read(reinterpret_cast<char*>(pages[pageSlot].data()), PAGE_SIZE * sizeof(int));
        inFile.close();
    }
};
