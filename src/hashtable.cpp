#include "hashtable.hpp"
using namespace std;
#include <iostream>

// User constructor
User::User(str n, str p, str c, int a) {
    name = n;
    phone = p;
    CNIC = c;
    age = a;
}

bool HashTable::containsCNIC(const str& cnic) const {
    for (const auto& entry : table) {
        if (entry.first != -1 && entry.second.CNIC == cnic) {
            return true;
        }
    }
    return false;
}

// HashTable constructor
HashTable::HashTable() : maxSize(50) {
    table.resize(maxSize, {-1, User()});
}

bool HashTable::insert(int key, str name, str phone, str CNIC, int age) {
    if (key < 0 || key >= maxSize) return false;
    if (table[key].first != -1) return false;  // Slot already taken

    table[key] = {key, User(name, phone, CNIC, age)};
    return true;
}

bool HashTable::update(int key, str newPhone) {
    if (key < 0 || key >= maxSize) return false;
    if (table[key].first != key) return false;

    table[key].second.phone = newPhone;
    return true;
}

bool HashTable::remove(int key) {
    if (key < 0 || key >= maxSize) return false;
    if (table[key].first != key) return false;

    table[key] = {-1, User()};
    return true;
}

int HashTable::countIDs() const {
    int count = 0;
    for (const auto& entry : table) {
        if (entry.first != -1)
            ++count;
    }
    return count;
}

void HashTable::printDatabase()const {
    for (const auto& entry : table) {
        if (entry.first != -1)
            cout << "Name:" << entry.second.name << "\n";
            cout << "phone:" << entry.second.phone << "\n";
            cout << "CNIC:" << entry.second.CNIC << "\n";
            cout << "age:" << entry.second.age << "\n";
            cout << "\n";
    }
}
