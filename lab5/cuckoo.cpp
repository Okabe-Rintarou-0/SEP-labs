#include "cuckoo.h"
#include <iostream>

cuckoo::cuckoo() {
    for (int i = 0; i < 2; ++i)
        tables[i].assign(curTableSize, nullptr);

    hash[0] = &hash1;
    hash[1] = &hash2;
}

bool cuckoo::_lookup(int key, int &val) {
    int hashVs[2] = {hash1(key), hash2(key)};

    for (int i = 0; i < 2; ++i) {
        int hashV = hashVs[i];
        KeyVal *keyVal = tables[i][hashV];
        if (keyVal && keyVal->key == key) {
            val = keyVal->val;
            return true;
        }
    }
    return false;
}

bool cuckoo::_delete(int key) {
    int hashVs[2] = {hash1(key), hash2(key)};

    for (int i = 0; i < 2; ++i) {
        int hashV = hashVs[i];
        KeyVal *keyVal = tables[i][hashV];
        if (keyVal && keyVal->key == key) {
            delete keyVal;
            tables[i][hashV] = nullptr;
            return true;
        }
    }
    return false;
}

void cuckoo::_insert(int key, int value) {
    int hashVs[2] = {hash1(key), hash2(key)};
    for (int i = 0; i < 2; ++i) {
        int hashV = hashVs[i];
        if (tables[i][hashV] == nullptr) {
            cout << "insert " << "(" << key << ", " << value << ") in table " << i << " and hashV " << hashV << endl;
            tables[i][hashV] = new KeyVal(key, value);
            return;
        }
    }

    int kickedSide = 0;
    int anotherSide;
    int hashV;
    KeyVal *kicked = new KeyVal(key, value);
    KeyVal *another = nullptr;
    do {
        KeyVal *tmp = tables[kickedSide][hashVs[kickedSide]];
        tables[kickedSide][hashVs[kickedSide]] = kicked;
        kicked = tmp;

        anotherSide = (kickedSide + 1) % 2;
        hashV = anotherSide ? hash2(kicked->key) : hash1(kicked->key);

        another = tables[anotherSide][hashV];
        if (another == nullptr) {
            tables[anotherSide][hashV] = kicked;
            return;
        }

        kickedSide = anotherSide;
    } while (1);
}