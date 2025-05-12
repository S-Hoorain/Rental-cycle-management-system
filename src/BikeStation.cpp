#include "BikeStation.hpp"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

bool BikeStationEntry::operator==(BikeStationEntry S) {
    return (Latitude == S.Latitude && Longitude == S.Longitude);
}

void BikeStations::LoadStationsFromCsv(const string &csv_file) {
    ifstream file(csv_file);
    if (!file.is_open()) {
        cerr << "Error opening file " << csv_file << endl;
        return;
    }
    
    string line;
    getline(file, line);  // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        BikeStationEntry station;

        // Read available bikes
        ss >> station.AvailableBikes;
        ss.ignore();  // Skip the comma

        // Read location
        getline(ss, station.Location, ',');

        // Read total docks
        ss >> station.TotalDocks;
        ss.ignore();  // Skip the comma

        // Read latitude and longitude
        ss >> station.Latitude;
        ss.ignore();  // Skip the comma
        ss >> station.Longitude;
        ss.ignore();  // Skip the comma
        
        // Read timestamp
        string timestampStr;
        getline(ss, timestampStr, ',');
        station.TimeStamp = ParseTimestamp(timestampStr);

        stations.push_back(station);
    }
    file.close();
}

void BikeStations::PrintBikeStationEntry() {
    // Print the bike station entry
    for (const auto &station : stations) {
        cout << "Location: " << station.Location << "\n";
        cout << "Available Bikes: " << station.AvailableBikes << "\n";
        cout << "Total Docks: " << station.TotalDocks << "\n";
        cout << "Latitude: " << station.Latitude << "\n";
        cout << "Longitude: " << station.Longitude << "\n";
        cout << "TimeStamp: " << station.TimeStamp << "\n";
        cout << "\n";
    }
}

string BikeStations::ParseTimestamp(const string &timestampStr) {
    // Convert timestamp string to tm struct
    struct tm tm = {};
    stringstream ss(timestampStr);
    ss >> get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        cerr << "Error parsing timestamp: " << timestampStr << endl;
        return "";  // Return empty string if parsing fails
    }

    // Format tm struct to desired date string
    stringstream formatted_date;
    formatted_date << std::put_time(&tm, "%Y-%m-%d");
    return formatted_date.str();
}


//Haversine Formula: It calculates the great-circle distance between two points on a sphere given their longitudes and latitudes.
double BikeStations::CalculateDistance(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0; // Earth radius in kilometers

    //converting degrees to radian
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    //formula
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c * 1000; // Return distance in meters
}
