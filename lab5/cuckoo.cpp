#include "cuckoo.h"
#include <iostream>
#include <queue>

cuckoo::cuckoo() {
    for (int i = 0; i < 2; ++i)
        tables[i].assign(curTableSize, KeyVal());
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

    int kickSide = 0;
    int anotherSide;
    int kickPos = hashVs[0];
    int anotherPos;
    KeyVal kicker = KeyVal(key, value, true);
    int kickedCnt = 0;

    do {
        const KeyVal kickee = tables[kickSide][kickPos];
        tables[kickSide][kickPos] = kicker;
        printf("Kick %d with %d in table %d %d\n", kickee.key, kicker.key, kickSide, kickPos);
        kicker = kickee;
        if (++kickedCnt == 2 * curTableSize) {
            cout << "Loop Detect" << endl;
            break;
        }

        anotherSide = (kickSide + 1) % 2;
        anotherPos = anotherSide ? hash2(kicker.key) : hash1(kicker.key);

        KeyVal &another = tables[anotherSide][anotherPos];
        if (!another.valid) {
            another = kicker;
            return;
        }

        kickSide = anotherSide;
        kickPos = anotherPos;
    } while (true);

    // resize and rehash
    resize();
    rehash();
    _insert(kicker.key, kicker.val);
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
