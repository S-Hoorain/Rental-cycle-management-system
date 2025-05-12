# 🚲 Automated Rental Cycle Management System

This project is a C++ implementation of a smart cycle rental system designed using core data structures and algorithms. It prioritizes bike station availability, finds optimal user routes, manages user data efficiently, and logs rental transactions persistently.

## 📌 Objective

Simulate a rental cycle network with efficient data management and real-time decision-making using:

- **Max-Heap** for station prioritization
- **Dijkstra’s Algorithm** for shortest path routing
- **Hash Maps** for user management
- **File I/O** for logging transactions

## 🧠 Features

### 🚉 Station Management

- Maintains bike availability using a **max-heap**
- Operations: Insert station, delete by ID, extract station with most bikes

### 🗺️ Route Optimization

- Implements **Dijkstra's algorithm** on a graph of stations
- Finds the shortest path between two stations

### 👥 User Management

- Uses a **hashmap** for fast insert/search/delete of users
- Stores user ID, name, and rental status

### 🧾 Transaction Logging

- Logs rentals and returns into `.txt` or `.csv` files
- Example log: `User U001 rented bike from Station S005 at 12:30PM`

## 📁 Project Structure

📦RentalCycleSystem
┣ 📂src
┃ ┣ bike_station_heap.cpp # Max-heap logic
┃ ┣ graph.cpp # Dijkstra’s algorithm
┃ ┣ user_map.cpp # Hashmap for user records
┃ ┗ main.cpp # Main program logic
┣ 📂data
┃ ┣ stations.csv # Station data
┃ ┣ users.csv # Registered users
┃ ┗ logs.txt # Transaction log
┣ Makefile # Build script
┗ README.md # Project description


## ⚙️ How to Compile & Run

g++ -std=c++17 -o rental_system src/*.cpp
./rental_system


## ✅ Outcome
This project demonstrates how fundamental data structures and algorithms can be used to build real-world management systems, with performance and scalability in mind.
