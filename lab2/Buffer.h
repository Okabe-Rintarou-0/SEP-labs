#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace lab {
    template<typename T>
    class List {
    public:
        List();
        ~List() {
            Node *p = head;
            while(p) {
                Node *tmp = p;
                p = p->next;
                delete tmp;
            }
        }

        struct Node {
            Node(T value, Node *pre = nullptr, Node *next = nullptr);

            T value;
            Node *pre;
            Node *next;
        };

        void printForDebug();

        void insert(int pos, const T &data);

        void erase(int from, int to);

        void append(const T &data);

        void append(int pos, const T &data);

        T &at(int pos);

        size_t size() const;

        std::vector <T> toArray() const;

        std::vector <T> toArray(int from, int to) const;

    private:
        Node *head = nullptr;
        size_t curSize;

        void erase(Node *p);
    };
}

template<typename T>
lab::List<T>::List() :curSize(0) {}

template<typename T>
lab::List<T>::Node::Node(T value, Node *pre, Node *next) : value(value), pre(pre), next(next) {}

template<typename T>
void lab::List<T>::insert(int pos, const T &data) {
    ++curSize;
    if (pos == 0) {
        head = new Node(data);
        return;
    }
    Node *p = head;
    for (int i = 1; i < pos; ++i)
        p = p->next;
    if (p == head) {
        head = new Node(data, nullptr, p);
        p->pre = head;
    } else {
        Node *node = new Node(data, p->pre, p);
        p->pre->next = node;
        p->pre = node;
    }
}

template<typename T>
void lab::List<T>::erase(Node *p) {
    if (p == head) {
        head = head->next;
        delete p;
    } else {
        p->pre->next = p->next;
        if (p->next) p->next->pre = p->pre;
        delete p;
    }
}

template<typename T>
void lab::List<T>::printForDebug() {
    auto p = head;
    while (p) {
        std::cout << p->value << std::endl;
        p = p->next;
    }
}

template<typename T>
void lab::List<T>::erase(int from, int to) {
    Node *p = head;
    for (int i = 1; i < from; ++i)
        p = p->next;
    for (int i = from; i <= to; ++i) {
        Node *tmp = p;
        p = p->next;
        erase(tmp);
    }
    curSize -= to - from + 1;
}

template<typename T>
void lab::List<T>::append(const T &data) {
    Node *p = head;
    while (p->next) p = p->next;
    p->next = new Node(data, p, nullptr);
    ++curSize;
}

template<typename T>
void lab::List<T>::append(int pos, const T &data) {
    ++curSize;
    if (pos == 0) {
        head = new Node(data, nullptr, nullptr);
        return;
    }
    Node *p = head;
    for (int i = 1; i < pos; ++i)
        p = p->next;
    Node *p_next = p->next;
    p->next = new Node(data, p, p_next);
    if (p_next)p_next->pre = p->next;
}

template<typename T>
std::vector <T> lab::List<T>::toArray() const {
    Node *p = head;
    std::vector <T> array;
    while (p) {
        array.push_back(p->value);
        p = p->next;
    }
    return array;
}

template<typename T>
std::vector <T> lab::List<T>::toArray(int from, int to) const {
    Node *p = head;
    for (int i = 1; i < from; ++i) p = p->next;
    std::vector <T> array;
    for (int i = from; i <= to; ++i) {
        array.push_back(p->value);
        p = p->next;
    }
    return array;
}

template<typename T>
size_t lab::List<T>::size() const {
    return curSize;
}

template<typename T>
T &lab::List<T>::at(int pos) {
    Node *p = head;
    for (int i = 1; i < pos; ++i)
        p = p->next;
    return p->value;
}

using std::string;
using lab::List;

class Buffer {
private:
    int currentLineNum = 0;
    // TODO: add a List here for storing the input lines

    List<string> lines;

public:
    Buffer();

    ~Buffer();

    int size() const;

    void writeToFile(const string &filename) const;

    const string &moveToLine(int idx);

    void showLines(int from, int to);

    void deleteLines(int from, int to);

    void insertLine(const string &text);

    void appendLine(const string &text);
};
