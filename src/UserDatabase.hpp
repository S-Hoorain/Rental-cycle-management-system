#ifndef USERDATABASE_HPP
#define USERDATABASE_HPP

#pragma once

#include <ctime>
#include <fstream>
#include <iomanip> // For get_time and put_time
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>    // For mathematical functions
#include "hashtable.hpp"

using namespace std;

struct UserEntry {
    int    ID;   // User ID
    string name;         // User Name
    string phone;        // User phone number
    string CNIC;         // User CNIC number
    int    age;       // User age
};

class UserDatabase {
public:
    vector<UserEntry> stations;

    void   LoadUsersFromCsv(const string &csv_file, HashTable &table);
    string ParseTimestamp(const string &timestampStr);
    void   PrintUserDatabaseEntry();
    void LoadUsersToCsv(const string &csv_file, HashTable &table);
};

#endif // USERDATABASE_HPP
