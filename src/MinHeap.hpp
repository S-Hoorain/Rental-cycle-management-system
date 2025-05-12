#ifndef BIKESTATIONMINHEAP_HPP
#define BIKESTATIONMINHEAP_HPP

#include <vector>
#include <string>
#include "BikeStation.hpp"

class BikeStationMinHeap {
private:
    std::vector<BikeStation> heap;

    void heapify(int index);
    void bubbleUp(int index);
    void swap(int i, int j);

public:
    void insert(const BikeStation& station);
    void deleteStation(int stationId);
    BikeStation getMin() const;
    BikeStation extractMin();
    void printHeap() const;
    bool isEmpty() const;
    int size() const;
};

#endif // BIKESTATIONMINHEAP_HPP
