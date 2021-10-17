#include "cuckoo.h"
#include <iostream>
#include <queue>

cuckoo::cuckoo() {
    for (int i = 0; i < 2; ++i)
        tables[i].assign(curTableSize, KeyVal());

    hash[0] = &hash1;
    hash[1] = &hash2;
}

bool cuckoo::_lookup(int key, int &val) {
    int hashVs[2] = {hash1(key), hash2(key)};

    for (int i = 0; i < 2; ++i) {
        int hashV = hashVs[i];
        KeyVal &keyVal = tables[i][hashV];
        if (keyVal.valid && keyVal.key == key) {
            val = keyVal.val;
            return true;
        }
    }
    return false;
}

bool cuckoo::_delete(int key) {
    int hashVs[2] = {hash1(key), hash2(key)};

    for (int i = 0; i < 2; ++i) {
        int hashV = hashVs[i];
        KeyVal &keyVal = tables[i][hashV];
        if (keyVal.valid && keyVal.key == key) {
            keyVal.valid = false;
            return true;
        }
    }
    return false;
}

void cuckoo::_insert(int key, int value) {
    int hashVs[2] = {hash1(key), hash2(key)};
    for (int i = 0; i < 2; ++i) {
        int hashV = hashVs[i];
        KeyVal &keyVal = tables[i][hashV];
        if (!keyVal.valid) {
//            cout << "insert " << "(" << key << ", " << value << ") in table " << i << " and hashV " << hashV << endl;
            keyVal.key = key;
            keyVal.val = value;
            keyVal.valid = true;
            return;
        } else if (keyVal.key == key) {
            keyVal.val = value;
            return;
        }
    }

    int kickedSide = 0;
    int anotherSide;
    int hashV;
    KeyVal kicked = KeyVal(key, value, true);
    int kickedCnt = 0;

    do {
        const KeyVal tmp = tables[kickedSide][hashVs[kickedSide]];
        tables[kickedSide][hashVs[kickedSide]] = kicked;
        printf("Kick %d with %d in table %d %d\n", tmp.key, kicked.key, kickedSide, hashVs[kickedSide]);
        kicked = tmp;
        if (++kickedCnt == 2 * curTableSize) {
            cout << "Loop Detect" << endl;
            break;
        }

        anotherSide = (kickedSide + 1) % 2;
        hashV = anotherSide ? hash2(kicked.key) : hash1(kicked.key);

        KeyVal &another = tables[anotherSide][hashV];
        if (!another.valid) {
            another = kicked;
            return;
        }

        kickedSide = anotherSide;
    } while (true);

    // resize and rehash
    resize();
    rehash();
    _insert(kicked.key, kicked.val);
}

void cuckoo::resize() {
    curTableSize *= 2;
    for (int i = 0; i < 2; ++i) {
        tables[i].resize(curTableSize);
    }
}

void cuckoo::rehash() {
    queue <KeyVal> keyVals;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < curTableSize / 2; ++j) {
            KeyVal &keyVal = tables[i][j];
            if (keyVal.valid) {
                keyVals.push(keyVal);
                keyVal.valid = false;
            }
        }
    }

    while (!keyVals.empty()) {
        KeyVal keyVal = keyVals.front();
        keyVals.pop();

        _insert(keyVal.key, keyVal.val);
    }
}
