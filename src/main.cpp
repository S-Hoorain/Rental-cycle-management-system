#include "BikeStation.hpp"
#include "UserDatabase.hpp"
#include "MaxHeap.hpp"
#include "MinHeap.hpp"
#include "hashtable.hpp"
#include <iostream>

using namespace std;

int main() {
    // Create an instance of BikeStation to load data from CSV file
    BikeStations bikeStation;
    // bikeStation.
    bikeStation.LoadStationsFromCsv("input/Bikestations.csv");  // Assuming the file is in the working directory
    // Create an instance of the User database to load data from CSV file
    UserDatabase Users;
    // Create an instance of Hashtable to store user data
    HashTable table;
    //UserDatabase.
    Users.LoadUsersFromCsv("input/Userdata.csv", table);
    // Print all loaded bike stations
    cout << "All Loaded Bike Stations:" << endl;
    bikeStation.PrintBikeStationEntry();

    // Print all loaded user data
    cout << "All Loaded Users:" << endl;
    Users.PrintUserDatabaseEntry();
    
    // Create an instance of BikeStationHeap (Max Heap)
    BikeStationHeap heap;
    // Create an instance of BikeStationHeap (Min Heap)
    BikeStationMinHeap Minheap;
    
    // Insert bike stations into the heap
    for (const auto& entry : bikeStation.stations) {
        BikeStation station;
        station.location = entry.Location;
        station.availableBikes = entry.AvailableBikes;

        // Insert into heap
        heap.insert(station);
        Minheap.insert(station);
    }

    // Print the heap (bike stations sorted by available bikes)
    cout << "\nHeap of Bike Stations (Max Heap based on available bikes):" << endl;
    heap.printHeap();

    // Extract the max (station with the highest available bikes)
    BikeStation maxStation = heap.extractMax();
    BikeStation minStation = Minheap.extractMin();
    cout << "\nExtracted Max Bike Station (Highest available bikes):" << endl;
    cout << "[ID: " << maxStation.stationId << ", Location: " << maxStation.location 
         << ", Available Bikes: " << maxStation.availableBikes << "]" << endl;
        cout << "\nExtracted Min Bike Station (Highest available bikes):" << endl;
    cout << "[ID: " << minStation.stationId << ", Location: " << minStation.location 
         << ", Available Bikes: " << minStation.availableBikes << "]" << endl;


}