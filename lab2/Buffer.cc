#include <fstream>
#include <iostream>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::checkInterval(int from, int to) const {
    if (from > to) {
        throw "Number range error";
    }
    int size = lines.size();
    if (from > size || to > size || from < 1) {
        throw "Line number out of range";
    }
}

void Buffer::writeToFile(const string &filename) const {
    std::ofstream file(filename);
    int writtenBytes = 0;
    for (const string &line: lines) {
        file << line << std::endl;
        writtenBytes += line.size();
    }
    std::cout << writtenBytes << " byte(s) written" << std::endl;
}

void Buffer::showLines(int from, int to) const {
    checkInterval(from, to);
    auto iter = lines.begin();
    for (int i = 1; i < from; ++i) ++iter;
    for (int i = from; i <= to; ++i)
        std::cout << i << "\t" << *(iter++) << std::endl;
}

void Buffer::deleteLines(int from, int to) {
    checkInterval(from, to);
    auto iter = lines.begin();
    for (int i = 1; i < from; ++i) ++iter;
    for (int i = from; i <= to; ++i)
        lines.erase(iter++);
}

void Buffer::insertLine(const string &text) {
    auto iter = lines.begin();
    for (int i = 1; i < currentLineNum; ++i)
        ++iter;
    lines.insert(iter, text);
}

void Buffer::appendLine(const string &text) {
    lines.push_back(text);
}

const string &Buffer::moveToLine(int idx) const {
    auto iter = lines.begin();
    for (int i = 1; i < idx; ++i)
        ++iter;
}

int Buffer::size() const {
    return lines.size();
};
