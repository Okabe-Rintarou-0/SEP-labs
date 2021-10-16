#ifndef LAB5_CUCKOO_H
#define LAB5_CUCKOO_H

#include <vector>

using namespace std;

struct KeyVal {
    KeyVal(int key, int val) : key(key), val(val) {}

    int key;
    int val;
};

class cuckoo {
private:
    vector<KeyVal *> tables[2];

    int curTableSize = 8;

    int (cuckoo::*hash[2])(int key);

    inline int hash1(int key) {
        return key % curTableSize;
    }

    inline int hash2(int key) {
        return (key / curTableSize) % curTableSize;
    }

    void rehash();

public:

    cuckoo();

    ~cuckoo() = default;

    void _insert(int key, int value);

    bool _lookup(int key, int &val);

    bool _delete(int key);
};


#endif //LAB5_CUCKOO_H
