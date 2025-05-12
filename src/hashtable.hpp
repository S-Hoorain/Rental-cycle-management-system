#ifndef USER
#define USER

#include <string>
using namespace std;
typedef string str;
#include <vector>

struct User {
    str name;
    str phone;
    str CNIC;
    int age;

    User(str = "", str = "", str = "", int = -1);
};
#include <vector>


class HashTable {
private:
    vector<pair<int, User>> table;
    int maxSize;

public:
    HashTable();
    bool containsCNIC(const str& cnic) const;
    bool insert(int, str, str, str, int);
    bool update(int, str);
    bool remove(int);
    int getSize() const { return maxSize; }
    int getIDAt(int i) const { return table[i].first; }
    User getUserAt(int i) const { return table[i].second; }
    int countIDs() const;
    void printDatabase() const;
};

#endif
