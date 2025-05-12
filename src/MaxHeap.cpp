#include "MaxHeap.hpp"
#include <iostream>

void BikeStationHeap::swap(int i, int j) {
    BikeStation temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

bool BikeStationHeap::updateBikes(int stationId, int delta) {
    for (int i = 0; i < heap.size(); ++i) {
        if (heap[i].stationId == stationId) {
            heap[i].availableBikes += delta;
            // Fix heap after update
            heapify(i);
            bubbleUp(i);
            return true;
        }
    }
    return false; // Station not found
}

void BikeStationHeap::heapify(int index) {
    int size = heap.size();
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && heap[left].availableBikes > heap[largest].availableBikes)
        largest = left;
    if (right < size && heap[right].availableBikes > heap[largest].availableBikes)
        largest = right;

    if (largest != index) {
        swap(index, largest);
        heapify(largest);
    }
}

void BikeStationHeap::bubbleUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].availableBikes > heap[parent].availableBikes) {
            swap(index, parent);
            index = parent;
        } else {
            break;
        }
    }
}

void BikeStationHeap::insert(const BikeStation& station) {
    heap.push_back(station);
    bubbleUp(heap.size() - 1);
}

void BikeStationHeap::deleteStation(int stationId) {
    int size = heap.size();
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (heap[i].stationId == stationId) {
            index = i;
            break;
        }
    }

    if (index == -1) return; // Station not found

    swap(index, size - 1);
    heap.pop_back();
    if (index < heap.size()) {
        heapify(index);
        bubbleUp(index); // in case the value was smaller and now needs to rise
    }
}

BikeStation BikeStationHeap::getMax() const {
    if (heap.empty())
        throw std::runtime_error("Heap is empty");
    return heap[0];
}

BikeStation BikeStationHeap::extractMax() {
    if (heap.empty())
        throw std::runtime_error("Heap is empty");
    BikeStation max = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    heapify(0);
    return max;
}

void BikeStationHeap::printHeap() const {
    for (const BikeStation& station : heap) {
        std::cout << "[ID: " << station.stationId
                  << ", Location: " << station.location
                  << ", Bikes: " << station.availableBikes << "]\n";
    }
}

bool BikeStationHeap::isEmpty() const {
    return heap.empty();
}

int BikeStationHeap::size() const {
    return heap.size();
}