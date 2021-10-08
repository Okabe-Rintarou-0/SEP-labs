#pragma once

#include <string>
#include <list>

using std::string;
using std::list;

class Buffer {
private:
    int currentLineNum;
    // TODO: add a List here for storing the input lines

    list <string> lines;

    void checkInterval(int from, int to) const;

public:
    Buffer();

    ~Buffer();

    int size() const;

    void writeToFile(const string &filename) const;

    const string &moveToLine(int idx) const;

    void showLines(int from, int to) const;

    void deleteLines(int from, int to);

    void insertLine(const string &text);

    void appendLine(const string &text);
};
