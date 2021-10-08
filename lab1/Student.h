#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
public:
    Student(const std::string &, const std::string &, const std::string &);

    enum Degree {
        graduate, undergraduate
    };
    const std::string id;

    std::string toString() const;

    virtual double getGrade() const;

    void addClass(Class *c);

    bool memberOfClass(std::string name);

protected:
    Degree degree;
    std::vector<Class *> classes;
    std::string name;
    std::string year;
};

// TODO: implement class Graduate.
class Graduate : public Student {
public:
    Graduate(const std::string &, const std::string &, const std::string &);

    double getGrade() const override;
};

// TODO: implement class Undergraduate.

class Undergraduate : public Student {
public:
    Undergraduate(const std::string &, const std::string &, const std::string &);

    double getGrade() const override;
};

class StudentWrapper {
private:
    const Student &student;
    double score = 0.0;
public:
    const std::string id;

    // TODO: fix error
    StudentWrapper(const std::string &id, const Student &student) : student(student), id(id) {}

    void setScore(double score) {
        if (score < 0 || score > 100)
            throw "Wrong score!";
        this->score = score;
    }

    double getScore() const {
        return this->score;
    }

    std::string toString() const {
        return student.toString();
    }
};

#endif // STUDENT_H_
