#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <filesystem>
#include <iostream>


const int GARBAGE_LIMIT = 3;

template <typename T>
class Table {
private:
    std::string dataFileName;
    std::string indexFileName;

    std::string dataFileExtension = ".fl";
    std::string indexFileExtension = ".ind";

    std::fstream dataFile;
    std::vector<std::pair<int, int>> indexTable;
    std::vector<int> garbageCollection;

    void loadIndex();
    void saveIndex();
    void initializeFiles(const std::string& baseName);
    void compactFile();
    int findRecordPosition(int key);
    T readRecordAtPosition(int position);

public:
    Table(const std::string& tableName);
    ~Table();
    void insert(const T& record);
    void update(int key, const T& updatedRecord);
    void remove(int key);
    T get(int key);
    std::vector<T> getAll();
    std::vector<T> getAllRecords();
    bool exists(int key);
};


template <typename T>
Table<T>::Table(const std::string& tableName) {
    initializeFiles(tableName);
    bool dataExists = std::filesystem::exists(dataFileName);

    dataFile.open(dataFileName, std::ios::in | std::ios::out | std::ios::binary);
    if (!dataFile.is_open()) {
        dataFile.clear();
        dataFile.open(dataFileName, std::ios::out | std::ios::binary);
        dataFile.close();
        dataFile.open(dataFileName, std::ios::in | std::ios::out | std::ios::binary);
    }
    if (dataExists) {
        loadIndex();
    }
}

template <typename T>
Table<T>::~Table() {
    saveIndex();
    if (dataFile.is_open()) dataFile.close();
}

template <typename T>
void Table<T>::initializeFiles(const std::string& baseName) {
    dataFileName = baseName + dataFileExtension;
    indexFileName = baseName + indexFileExtension;
}

template <typename T>
void Table<T>::loadIndex() {
    indexTable.clear();
    garbageCollection.clear();

    std::ifstream ifs(indexFileName);
    if (!ifs.is_open()) return;

    std::string firstLine;
    if (std::getline(ifs, firstLine)) {
        std::istringstream ss(firstLine);
        int pos;
        while (ss >> pos) {
            garbageCollection.push_back(pos);
        }
    }

    int key, pos;
    while (ifs >> key >> pos) {
        indexTable.emplace_back(key, pos);
    }
    ifs.close();
    std::sort(indexTable.begin(), indexTable.end());
}

template <typename T>
void Table<T>::saveIndex() {
    std::ofstream ofs(indexFileName, std::ios::trunc);

    for (size_t i = 0; i < garbageCollection.size(); i++) {
        ofs << garbageCollection[i];
        if (i < garbageCollection.size() - 1) ofs << " ";
    }
    ofs << "\n";

    for (auto& kp : indexTable) {
        ofs << kp.first << " " << kp.second << "\n";
    }
    ofs.close();
}

template <typename T>
int Table<T>::findRecordPosition(int key) {
    auto it = std::lower_bound(indexTable.begin(), indexTable.end(), std::make_pair(key, 0),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.first < b.first; });

    if (it != indexTable.end() && it->first == key) {
        return it->second;
    }
    else {
        throw std::runtime_error("record not found");
    }
}


template <typename T>
T Table<T>::readRecordAtPosition(int position) {
    T record{};
    dataFile.seekg(position * sizeof(T), std::ios::beg);
    dataFile.read(reinterpret_cast<char*>(&record), sizeof(T));
    return record;
}

template <typename T>
void Table<T>::insert(const T& record) {
    int position;
    if (!garbageCollection.empty()) {
        position = garbageCollection.back();
        garbageCollection.pop_back();
    }
    else {
        dataFile.seekp(0, std::ios::end);
        position = static_cast<int>(dataFile.tellp() / sizeof(T));
    }
    dataFile.seekp(position * sizeof(T), std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&record), sizeof(T));
    indexTable.emplace_back(record.id, position);
    std::sort(indexTable.begin(), indexTable.end());
}

template <typename T>
T Table<T>::get(int key) {
    int pos = findRecordPosition(key);
    return readRecordAtPosition(pos);
}

template <typename T>
void Table<T>::update(int key, const T& updatedRecord) {
    int pos = findRecordPosition(key);
    dataFile.seekp(pos * sizeof(T), std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&updatedRecord), sizeof(T));
}

template <typename T>
void Table<T>::remove(int key) {
    int pos = findRecordPosition(key);
    T record = readRecordAtPosition(pos);
    record.isDeleted = true;
    dataFile.seekp(pos * sizeof(T), std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&record), sizeof(T));
    indexTable.erase(
        std::remove_if(indexTable.begin(), indexTable.end(),
            [key](auto& p) {return p.first == key; }),
        indexTable.end());
    garbageCollection.push_back(pos);
    if (garbageCollection.size() >= GARBAGE_LIMIT) {
        compactFile();
    }
}

template <typename T>
std::vector<T> Table<T>::getAll() {
    std::vector<T> result;
    result.reserve(indexTable.size());
    for (auto& kp : indexTable) {
        result.push_back(readRecordAtPosition(kp.second));
    }
    return result;
}

template <typename T>
void Table<T>::compactFile() {
    std::sort(garbageCollection.begin(), garbageCollection.end());
    std::sort(indexTable.begin(), indexTable.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    size_t limit = std::min(garbageCollection.size(), indexTable.size());
    for (size_t i = 0; i < limit; i++) {
        int freePos = garbageCollection[i];
        int usedPos = indexTable[i].second;
        if (usedPos > freePos) {
            dataFile.seekg(usedPos * sizeof(T), std::ios::beg);
            T rec;
            dataFile.read(reinterpret_cast<char*>(&rec), sizeof(T));
            dataFile.seekp(freePos * sizeof(T), std::ios::beg);
            dataFile.write(reinterpret_cast<const char*>(&rec), sizeof(T));
            indexTable[i].second = freePos;
        }
        else {
            break;
        }
    }

    dataFile.flush();
    dataFile.close();

    int maxPos = 0;
    for (const auto& entry : indexTable)
        if (entry.second > maxPos) maxPos = entry.second;

    int newSize = indexTable.empty() ? 0 : (maxPos + 1) * sizeof(T);
    dataFile.close();
    dataFile.open(dataFileName, std::ios::in | std::ios::out | std::ios::binary);

    std::sort(indexTable.begin(), indexTable.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    garbageCollection.clear();
}

template <typename T>
std::vector<T> Table<T>::getAllRecords() {
    dataFile.seekg(0, std::ios::beg);
    std::vector<T> result;
    while (dataFile) {
        T r;
        dataFile.read(reinterpret_cast<char*>(&r), sizeof(T)); 
        if (dataFile) {
            result.push_back(r);
        }
    }
    dataFile.clear();
    return result;
}

template <typename T>
bool Table<T>::exists(int key) {
    auto it = std::lower_bound(indexTable.begin(), indexTable.end(),
        std::make_pair(key, 0),
        [](auto& a, auto& b) { return a.first < b.first; });
    return it != indexTable.end() && it->first == key;
}
