#include "cuckoo.h"
#include <iostream>
#include <sstream>

using namespace std;

int main() {
    int M;
    cin >> M;

    cuckoo cuckooHash;

    string cmdline;
    string op;
    int para1, para2;
    cin.ignore();
    for (int i = 0; i < M; ++i) {
        getline(cin, cmdline);
//        cout << "cmdline: " << cmdline << endl;
        stringstream ss(cmdline);
        ss >> op;
//        cout << "op: " << op << endl;
        if (op == "Insert") {
            ss >> para1 >> para2;
            if (ss.eof())
                cuckooHash._insert(para1, para2);
        } else if (op == "Lookup") {
            ss >> para1;
            if (ss.eof()) {
                int val;
                if (cuckooHash._lookup(para1, val))
                    cout << val << endl;
                else
                    cout << "Key Not Found" << endl;
            }
        } else if (op == "Delete") {
            ss >> para1;
            if (ss.eof()) {
                if (!cuckooHash._delete(para1))
                    cout << "Key Not Found" << endl;
            }
        }
    }
}