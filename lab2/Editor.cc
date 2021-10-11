#include <iostream>
#include <sstream>
#include "Editor.h"

using namespace std;

Editor::Editor() {
    buffer = new Buffer();
}

Editor::~Editor() {
    // TODO: Implement destructor
    delete buffer;
}

void Editor::run() {
    string cmd;
    while (true) {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            cout << "? " << e << endl;
        } catch (const out_of_range &oor) {
            cout << "? " << oor.what() << endl;
        } catch (const range_error &re) {
            cout << "? " << re.what() << endl;
        }
    }
}

void Editor::cmdAppend() {
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    // TODO: finish cmdAppend.
    while (true) {
        string text;
        getline(cin, text);
        if (text == ".")
            break;
        buffer->appendLine(text);
    }
}

void Editor::cmdInsert() {
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    bool firstLine = true;
    while (true) {
        string text;
        getline(cin, text);
        if (text == ".")
            break;
        if (firstLine) {
            buffer->insertLine(text);
            firstLine = false;
        } else {
            buffer->appendLine(text);
        }
    }
}

void Editor::cmdDelete(int start, int end) {
    buffer->deleteLines(start, end);
}

void Editor::cmdNull(int line) {
    cout << buffer->moveToLine(line) << endl;
}

void Editor::cmdNumber(int start, int end) {
    buffer->showLines(start, end);
}

void Editor::cmdWrite(const string &filename) {
    buffer->writeToFile(filename);
}

void Editor::dispatchCmd(const string &cmd) {
    if (cmd == "a") {
        cmdAppend();
        return;
    }
    if (cmd == "i") {
        cmdInsert();
        return;
    }
    if (cmd[0] == 'w') {
        if (cmd.size() >= 2) {
            if (cmd[1] == ' ')
                cmdWrite(cmd.substr(2, cmd.size() - 2));
            else
                throw "Bad/Unknown command";
        }
        else throw "Filename not specified";
        return;
    }
    // TODO: handle special case "1,$n".
    if (cmd == "1,$n") {
        if (buffer->size() > 0)
            cmdNumber(1, buffer->size());
        else throw "Number range error";
        return;
    }
    int start, end;
    char comma, type = ' ';
    stringstream ss(cmd);
    ss >> start;
    if (ss.eof()) {
        cmdNull(start);
        return;
    }
    ss >> comma >> end >> type;
    if (ss.good()) {
        if (type == 'n') {
            cmdNumber(start, end);
            return;
        } else if (type == 'd') {
            cmdDelete(start, end);
            return;
        }
    }
    throw "Bad/Unknown command";
}

