#include "BikeStation.hpp"
#include "UserDatabase.hpp"
#include "MaxHeap.hpp"
#include "MinHeap.hpp"
#include "hashtable.hpp"
#include "graph.hpp"
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <iostream>
#include <string>

using namespace std;

string getLocation(const vector<BikeStationEntry> &stations, const string &prompt) {
    // cout << prompt << endl;
    int j = stations.size(), index;
    for(int i = 0; i < j; ++i) cout << i << ": " << stations[i].Location << endl;
    cout << prompt;
    cin >> index;
    while (! (index >= 0 && index < j)) {
        cout << "Invalid input. Try again: ";   cin >> index;
    }
    return stations[index].Location;
}

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
        cout << "4. Update phone number\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string currentLoc, destLoc;
            // cout << "Enter your current location: ";
            // getline(cin, currentLoc);
            // cout << "Enter your destination location: ";
            // getline(cin, destLoc);
            currentLoc = getLocation(bikeStation.stations, "Enter your current location: ");
            destLoc = getLocation(bikeStation.stations, "Enter your destination: ");

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

            StringReference *err = CreateStringReferenceLengthValue(0, L' ');
            RGBABitmapImageReference *img = CreateRGBABitmapImageReference();
            vector<double> X, Y;
            for (BikeStationEntry bs: path) {
                X.push_back(bs.Latitude);   Y.push_back(bs.Longitude);
            }
            DrawScatterPlot(img, 600, 400, &X, &Y, err);
            vector<double> *plot = ConvertToPNG(img->image);
            WriteToFile(plot, "shortest_path.png");
            DeleteImage(img->image);

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
            int id;
            string CNIC;
            cout << "\nEnter your ID: ";
            cin >> id;
            cout << "Enter your CNIC: ";
            cin >> CNIC;

            if (id < 0 || id >= userTable.getSize()) {
                cout << "❌ Invalid ID entered.\n";
                continue;
            }

            if (userTable.getIDAt(id) == -1) {
                cout << "❌ No user found with this ID.\n";
                continue;
            }

            User user = userTable.getUserAt(id);
            if (user.CNIC != CNIC) {
                cout << "❌ CNIC does not match the given ID.\n";
                continue;
            }

            string newPhone;
            cout << "Enter your new phone number: ";
            cin >> newPhone;

            bool success = userTable.update(id, newPhone);
            if (success) {
                cout << "✅ Phone number updated successfully.\n";
            } else {
                cout << "❌ Failed to update phone number.\n";
            }
            
        }

        else if (choice == 5) {
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