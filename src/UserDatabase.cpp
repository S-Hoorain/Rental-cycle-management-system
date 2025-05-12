#include "UserDatabase.hpp"
#include "hashtable.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void UserDatabase::LoadUsersFromCsv(const string &csv_file, HashTable &table) {
    ifstream file(csv_file);
    if (!file.is_open()) {
        cerr << "Error opening file: " << csv_file << endl;
        return;
    }

    string line;
    getline(file, line);  // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        UserEntry user;
        string temp;

        getline(ss, temp, ',');
        user.ID = stoi(temp);

        getline(ss, user.name, ',');
        getline(ss, user.phone, ',');
        getline(ss, user.CNIC, ',');

        getline(ss, temp, ',');
        user.age = stoi(temp);

        // Add to internal vector
        stations.push_back(user);

        // Insert into the hash table
        bool success = table.insert(user.ID, user.name, user.phone, user.CNIC, user.age);
        if (!success) {
            cerr << "Failed to insert user ID " << user.ID << " into hash table.\n";
        }
    }

    file.close();
}


void UserDatabase::PrintUserDatabaseEntry() {
    for (const auto &user : stations) {
        cout << "ID: " << user.ID << "\n";
        cout << "Name: " << user.name << "\n";
        cout << "Phone: " << user.phone << "\n";
        cout << "CNIC: " << user.CNIC << "\n";
        cout << "Age: " << user.age << "\n";
        cout << "\n";
    }
}

void UserDatabase::LoadUsersToCsv(const string &csv_file, HashTable &table) {
    ofstream file(csv_file);
    if (!file.is_open()) {
        cerr << "Error opening file " << csv_file << " for writing." << endl;
        return;
    }

    file << "ID,Name,Phone,CNIC,Age\n";

    for (int i = 0; i < table.getSize(); ++i) {
        int id = table.getIDAt(i);
        if (id != -1) {
            User user = table.getUserAt(i);
            file << id << ","
                 << user.name << ","
                 << user.phone << ","
                 << user.CNIC << ","
                 << user.age << "\n";
        }
    }

    file.close();
}

// This method is currently unused but included in case of future timestamp needs.
string UserDatabase::ParseTimestamp(const string &timestampStr) {
    struct tm tm = {};
    stringstream ss(timestampStr);
    ss >> get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        cerr << "Error parsing timestamp: " << timestampStr << endl;
        return "";
    }

    stringstream formatted;
    formatted << put_time(&tm, "%Y-%m-%d");
    return formatted.str();
}
