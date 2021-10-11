#include <fstream>
#include <iostream>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::writeToFile(const string &filename) const {
    std::string trimmed;
    int from = filename.find_first_not_of(' ');
    int to = filename.find_last_not_of(' ');

    if (from >= 0)
        trimmed = filename.substr(from, from - to + 1);
    else
        throw "Filename not specified";

    std::ofstream file(trimmed);
    std::vector <std::string> array = lines.toArray();
    std::string writtenStr;
    for (int i = 0; i < (int) array.size(); ++i) {
        writtenStr += array[i] + '\n';
    };
    file << writtenStr;
    std::cout << writtenStr.size() << " byte(s) written" << std::endl;
    file.close();
}

void Buffer::showLines(int from, int to) {
    if (from > to) {
        throw "Number range error";
    }

    int size = lines.size();
    if (from > size || to > size || from < 1) {
        throw "Line number out of range";
    }

    std::vector <string> array = lines.toArray(from, to);
    int idx = from;
    for (const string &line:array) {
        std::cout << idx++ << "\t" << line << std::endl;
    }
    currentLineNum = to;
}

void Buffer::deleteLines(int from, int to) {
    if (from > to) {
        throw "Delete range error";
    }

    int size = lines.size();
    if (from > size || to > size || from < 1) {
        throw "Line number out of range";
    }

    lines.erase(from, to);
    if (to == size || lines.size() == 0) {
        currentLineNum = lines.size();
    } else {
        currentLineNum = from;
    }
}

void Buffer::insertLine(const string &text) {
    lines.insert(currentLineNum, text);
    if (currentLineNum == 0) currentLineNum = 1;
}

void Buffer::appendLine(const string &text) {
    lines.append(currentLineNum++, text);
}

const string &Buffer::moveToLine(int idx) {
    if (idx < 1 || idx > (int) lines.size())
        throw "Line number out of range";
    currentLineNum = idx;
    return lines.at(currentLineNum);
}

int Buffer::size() const {
    return lines.size();
};
