#include "Student.h"
#include <string>
#include <iostream>
#include <sstream>
#include "Class.h"

std::string Student::toString() const {
    // TODO: uncomment the following code after implementing class Student.

    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
}

Student::Student(const std::string &id, const std::string &name, const std::string &year) : id(id), name(name),
                                                                                            year(year) {}

double Student::getGrade() const {
    return 0;
}

void Student::addClass(Class *c) {
    classes.push_back(c);
}

bool Student::memberOfClass(std::string name) {
    for (Class *cl: classes) {
        if (cl->name == name)
            return true;
    }
    return false;
}

Graduate::Graduate(const std::string &id, const std::string &name, const std::string &year)
        : Student(id, name, year) {
    degree = graduate;
}

double Graduate::getGrade() const {
    double gpa = 0;
    int totalPoints = 0;
    double grade;
    double scores;
    for (Class *cl: classes) {
        StudentWrapper sw = cl->getStudentWrapper(id);
        scores = sw.getScore();
        if (scores >= 90)
            grade = 4.0;
        else if (scores >= 80)
            grade = 3.5;
        else if (scores >= 70)
            grade = 3.0;
        else if (scores >= 60)
            grade = 2.5;
        else grade = 2.0;
        gpa += grade * cl->point;
        totalPoints += cl->point;
    }
    return gpa / totalPoints;
}

Undergraduate::Undergraduate(const std::string &id, const std::string &name, const std::string &year)
        : Student(id, name, year) {
    degree = undergraduate;
}

double Undergraduate::getGrade() const {
    double gpa = 0;
    int totalPoints = 0;
    double grade;
    double scores;
    for (Class *cl: classes) {
        StudentWrapper sw = cl->getStudentWrapper(id);

        scores = sw.getScore();
//        std::cout << "read scores: " << scores << std::endl;
        grade = scores / 20;
        gpa += grade * cl->point;
        totalPoints += cl->point;
    }
    return gpa / totalPoints;
}

// TODO: implement functions which are declared in Student.h.
