#include <fstream>
#include <iostream>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::locate(int from, int to, list<string>::const_iterator &iter) const {
    if (from > to) {
        throw "Number range error";
        return;
    }
    int size = lines.size();
    if (from > size || to > size) {
        throw "Line number out of range";
        return;
    }
    for (int i = 1; i < from; ++i) ++iter;
    for (int i = from; i <= to; ++i)
        std::cout << *(iter ++) << std::endl;
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
    auto iter = lines.begin();
    locate(from, to , iter);
}

void Buffer::deleteLines(int from, int to){
    auto iter = lines.begin();
//    locate(from, to , iter);
}

void Buffer::insertLine(const string &text){}

void Buffer::appendLine(const string &text){}

const string &Buffer::moveToLine(int idx) const { }
