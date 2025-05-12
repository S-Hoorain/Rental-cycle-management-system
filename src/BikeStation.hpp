#ifndef BIKESTATION_HPP
#define BIKESTATION_HPP

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

using namespace std;

struct BikeStationEntry {
    int    AvailableBikes;   // Number of bikes currently available
    string Location;         // Human-readable location name
    string TimeStamp;        // Timestamp of the current status
    int    TotalDocks;       // Total docking capacity at this station
    double Latitude;         // Latitude coordinate
    double Longitude;        // Longitude coordinate
    bool operator==(BikeStationEntry);
};

struct BikeStation {
    int stationId;
    std::string location;
    int availableBikes;

    bool operator==(const BikeStation& other) const {
        return stationId == other.stationId;
    }
};

class BikeStations {
public:
    vector<BikeStationEntry> stations;

    void   LoadStationsFromCsv(const string &csv_file);
    string ParseTimestamp(const string &timestampStr);
    void   PrintBikeStationEntry();
    double CalculateDistance(double lat1, double lon1, double lat2, double lon2);
};

#endif // BIKESTATION_HPP
