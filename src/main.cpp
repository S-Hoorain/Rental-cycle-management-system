#include "BikeStation.hpp"
#include "UserDatabase.hpp"
#include "MaxHeap.hpp"
#include "MinHeap.hpp"
#include "hashtable.hpp"
#include "graph.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    // Initialize system
    BikeStations bikeStation;
    UserDatabase userDB;
    HashTable userTable;
    Graph graph;

    // Load data
    bikeStation.LoadStationsFromCsv("input/Bikestations.csv");
    userDB.LoadUsersFromCsv("input/Userdata.csv", userTable);

    // Build graph
    for (const auto& entry : bikeStation.stations) {
        graph.addNode(entry);
    }
    for (size_t i = 0; i < bikeStation.stations.size(); ++i) {
        for (size_t j = i + 1; j < bikeStation.stations.size(); ++j) {
            graph.addEdge(bikeStation.stations[i], bikeStation.stations[j]);
        }
    }

    cout << "🚲 Welcome to the Rental Cycle Optimization System!\n";

    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1. Find optimized pickup/drop-off stations\n";
        cout << "2. View all stations\n";
        cout << "3. Create new user account\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string currentLoc, destLoc;
            cout << "Enter your current location: ";
            getline(cin, currentLoc);
            cout << "Enter your destination location: ";
            getline(cin, destLoc);

            BikeStationEntry* source = nullptr;
            BikeStationEntry* dest = nullptr;

            for (auto& s : bikeStation.stations) {
                if (s.Location == currentLoc) source = &s;
                if (s.Location == destLoc) dest = &s;
            }

            if (!source || !dest) {
                cout << "❌ One or both locations not found.\n";
                continue;
            }

            auto path = graph.shortestPath(*source, *dest);
            if (path.empty()) {
                cout << "❌ No path found between the locations.\n";
                continue;
            }

            BikeStationHeap pickupHeap;
            BikeStationMinHeap dropHeap;

            for (const auto& s : path) {
                BikeStation bs;
                bs.stationId = s.TotalDocks; // fallback ID
                bs.location = s.Location;
                bs.availableBikes = s.AvailableBikes;

                if (s.AvailableBikes > 0)
                    pickupHeap.insert(bs);
                dropHeap.insert(bs);
            }

            if (pickupHeap.isEmpty() || dropHeap.isEmpty()) {
                cout << "❌ Cannot recommend stations. Check availability.\n";
                continue;
            }

            BikeStation bestPickup = pickupHeap.getMax();
            BikeStation bestDrop = dropHeap.getMin();

            cout << "\n✅ Recommended Stations:\n";
            cout << "Pickup ➤ " << bestPickup.location << " | Bikes: " << bestPickup.availableBikes << "\n";
            cout << "Drop-off ➤ " << bestDrop.location << " | Bikes: " << bestDrop.availableBikes << "\n";

            char confirm;
            cout << "\nProceed with renting and returning a bike? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                pickupHeap.updateBikes(bestPickup.stationId, -1);
                dropHeap.updateBikes(bestDrop.stationId, +1);
                cout << "🚴 Transaction complete.\n";
            } else {
                cout << "Transaction cancelled.\n";
            }
        }

        else if (choice == 2) {
            bikeStation.PrintBikeStationEntry();
        }

        else if (choice == 3) {
            cout << "\n📋 Create New User Account:\n";

            string name, phone, cnic;
            int age;

            cout << "Enter Name: ";
            getline(cin, name);

            cout << "Enter Phone: ";
            getline(cin, phone);

            cout << "Enter CNIC: ";
            getline(cin, cnic);

            if (userTable.containsCNIC(cnic)) {
                cout << "❌ User with this CNIC already exists.\n";
                continue;
            }

            cout << "Enter Age: ";
            cin >> age;

            int id = -1;
            for (int i = 0; i < userTable.getSize(); ++i) {
                if (userTable.getIDAt(i) == -1) {
                    id = i;
                    break;
                }
            }

            if (id == -1) {
                cout << "❌ User creation failed: HashTable is full.\n";
                continue;
            }

            bool inserted = userTable.insert(id, name, phone, cnic, age);
            if (!inserted) {
                cout << "❌ User insertion failed.\n";
            } else {
                UserEntry newUser = {id, name, phone, cnic, age};
                userDB.stations.push_back(newUser);
                cout << "✅ Account created! Assigned ID: " << id << "\n";
            }
        }

        else if (choice == 4) {
            cout << "💾 Saving users to file...\n";
            userDB.LoadUsersToCsv("input/Userdata.csv", userTable);
            cout << "👋 Exiting. Goodbye!\n";
            break;
        }

        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}