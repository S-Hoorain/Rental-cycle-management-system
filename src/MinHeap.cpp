#include "MinHeap.hpp"
#include <iostream>
#include <stdexcept>

void BikeStationMinHeap::swap(int i, int j) {
    BikeStation temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void BikeStationMinHeap::heapify(int index) {
    int size = heap.size();
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && heap[left].availableBikes < heap[smallest].availableBikes)
        smallest = left;
    if (right < size && heap[right].availableBikes < heap[smallest].availableBikes)
        smallest = right;

    if (smallest != index) {
        swap(index, smallest);
        heapify(smallest);
    }
}

bool BikeStationMinHeap::updateBikes(int stationId, int delta) {
    for (int i = 0; i < heap.size(); ++i) {
        if (heap[i].stationId == stationId) {
            heap[i].availableBikes += delta;
            heapify(i);
            bubbleUp(i);
            return true;
        }
    }
    return false;
}

void BikeStationMinHeap::bubbleUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].availableBikes < heap[parent].availableBikes) {
            swap(index, parent);
            index = parent;
        } else {
            break;
        }
    }
}

void BikeStationMinHeap::insert(const BikeStation& station) {
    heap.push_back(station);
    bubbleUp(heap.size() - 1);
}

void BikeStationMinHeap::deleteStation(int stationId) {
    int size = heap.size();
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (heap[i].stationId == stationId) {
            index = i;
            break;
        }
    }

    if (index == -1) return;

    swap(index, size - 1);
    heap.pop_back();
    if (index < heap.size()) {
        heapify(index);
        bubbleUp(index);
    }
}

BikeStation BikeStationMinHeap::getMin() const {
    if (heap.empty())
        throw std::runtime_error("Heap is empty");
    return heap[0];
}

BikeStation BikeStationMinHeap::extractMin() {
    if (heap.empty())
        throw std::runtime_error("Heap is empty");
    BikeStation min = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    heapify(0);
    return min;
}

void BikeStationMinHeap::printHeap() const {
    for (const BikeStation& station : heap) {
        std::cout << "[ID: " << station.stationId
                  << ", Location: " << station.location
                  << ", Bikes: " << station.availableBikes << "]\n";
    }
}

bool BikeStationMinHeap::isEmpty() const {
    return heap.empty();
}

int BikeStationMinHeap::size() const {
    return heap.size();
}