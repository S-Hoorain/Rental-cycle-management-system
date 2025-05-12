#ifndef BIKESTATIONHEAP_HPP
#define BIKESTATIONHEAP_HPP

#include <vector>
#include "BikeStation.hpp"
#include <string>



class BikeStationHeap {
private:
    std::vector<BikeStation> heap;

    void heapify(int index);
    void bubbleUp(int index);
    void swap(int i, int j);

public:
    void insert(const BikeStation& station);
    void deleteStation(int stationId);
    BikeStation getMax() const;
    BikeStation extractMax();
    void printHeap() const;
    bool isEmpty() const;
    int size() const;
};

#endif // BIKESTATIONHEAP_HPP