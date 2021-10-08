#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Class.h"
#include "Student.h"

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();

    void inputScore();

    void printAvgScore();

    void printGpa();

    Student *findStudent(const std::string &id) const;

    Class *findClass(const std::string &name, Student *st) const;

    Class *findClass(const std::string &name) const;

public:
    AppX();

    ~AppX();

    int run();
};

AppX::~AppX() {
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX() {
    loadFiles();
}

Student *AppX::findStudent(const std::string &id) const {
    for (Student *student:studentVec) {
        if (student->id == id)
            return student;
    }
    throw "No match student!";
}

Class *AppX::findClass(const std::string &name, Student *st) const {
    for (Class *cl: classVec) {
        if (cl->name == name && st->memberOfClass(name)) {
            return cl;
        }
    }
    throw "No match class!";
}

Class *AppX::findClass(const std::string &name) const {
    for (Class *cl: classVec) {
        if (cl->name == name) {
            return cl;
        }
    }
    throw "No match class!";
}

void AppX::loadFiles() {
    string line;
    size_t pos1, pos2;
    vector <string> bufv;
    Student *st = nullptr;
    Class *cl = nullptr;
    // Open a file as a input stream.
    ifstream stfile("./Students.txt");

    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each columnes in the line.
        bufv.clear();
        // Split the line into columns.
        //   pos1: beginning the the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        // TODO: uncomment next lines after implementing class Undergraduate
        // and Graduate.

        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);


        studentVec.push_back(st);
    }
    stfile.close();

    ifstream clfile("./Classes.txt");
    while (getline(clfile, line)) {
        std::string name, point;
        if (line.find_first_of("Class") == 0) {
            pos1 = line.find(":", 0);
            if (pos1 != std::string::npos) {
                name = line.substr(pos1 + 1);
                getline(clfile, line);
                if (line.find_first_of("Points") == 0) {
                    pos1 = line.find(":", 0);
                    if (pos1 != std::string::npos) {
                        point = line.substr(pos1 + 1);
                        cl = new Class(name, std::atoi(point.c_str()));
                        while (getline(clfile, line)) {
                            if (line.empty())
                                break;
                            try {
                                Student *st = findStudent(line);
                                cl->addStudent(*st);
                                st->addClass(cl);
                            } catch (const char *err_msg) {
                                std::cout << err_msg << std::endl;
                            }
                        }
                        classVec.push_back(cl);
                    }
                }
            }
        }
    }
    clfile.close();


    // TODO: load data from ./Classes.txt and push objects to the vector.
    // Hint: how is student information read?
}

void AppX::inputScore() {
    // TODO: implement inputScore.
    // Hint: Take a look at printAvgScore().
    string sbuf;
    Class *cl;
    Student *st;

    while (true) {
        cout << "Please input the student id (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;
        try {
            st = findStudent(sbuf);
        } catch (const char *err_msg) {
            std::cout << err_msg << std::endl;
            continue;
        }

        inputClass:
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        try {
            cl = findClass(sbuf, st);
        } catch (const char *err_msg) {
            std::cout << err_msg << std::endl;
            goto inputClass;
        }

        cout << "Please input the scores (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        double scores = -1;
        stringstream ss;
        ss << sbuf;
        ss >> scores;

        try {
            cl->getStudentWrapper(st->id).setScore(scores);
        } catch (const char *err_msg) {
            std::cout << err_msg << std::endl;
            continue;
        }
    }
}

void AppX::printAvgScore() {
    string sbuf;
    Class *cl;
    double avg;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        try {
            cl = findClass(sbuf);
        } catch (const char *err_msg) {
            std::cout << err_msg << std::endl;
            continue;
        }

        avg = cl->getAvgScore();
        printf("The average score is: %.2f\n", avg);
    }
}

void AppX::printGpa() {
    // TODO: implement printGpa.
    // Hint: Take a look at printAvgScore().
    string sbuf;
    Student *st;
    double gpa;

    while (true) {
        cout << "Please input the student id (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        try {
            st = findStudent(sbuf);
            gpa = st->getGrade();
            printf("GPA = %.2f\n", gpa);
        } catch (const char *err_msg) {
            std::cout << err_msg << std::endl;
            continue;
        }
    }

}

int AppX::run() {
    char cmd;
    while (true) {
        cout << "Command menu:\n"
             << "\ti: Input score\n"
             << "\ta: Compute average score of a class\n"
             << "\tg: Compute grade of a student\n"
             << "\tq: Quit\n"
             << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printAvgScore();
        } else if (cmd == 'g') {
            printGpa();
        } else if (cmd == 'q') {
            break;
        } else {
            cout << "Invalid command!\n" << endl;
        }
    }
    return 0;
}

int main() {
    AppX app;
    return app.run();
}
