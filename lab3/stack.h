#ifndef LAB3_STACK_H
#define LAB3_STACK_H

#include <iostream>

namespace lab {
    template<typename T>
    class Stack {
    private:
        static const unsigned int DEFAULT_INITIAL_SIZE = 4;
        unsigned int curSize;
        unsigned int capacity;
        T *data;

        void doubleSpace();

    public:
        Stack();

        ~Stack();

        unsigned int size() const;

        T top() const;

        T pop();

        bool isEmpty() const;

        void clear();

        void push(const T &data);

    };
}

template<typename T>
lab::Stack<T>::Stack() : curSize(0), capacity(DEFAULT_INITIAL_SIZE), data(new T[capacity]) {
}

template<typename T>
lab::Stack<T>::~Stack() {
    delete data;
}

template<typename T>
inline unsigned int lab::Stack<T>::size() const {
    return curSize;
}

template<typename T>
inline T lab::Stack<T>::top() const {
    return data[curSize - 1];
}

template<typename T>
inline void lab::Stack<T>::clear() {
    curSize = 0;
}

template<typename T>
inline bool lab::Stack<T>::isEmpty() const {
    return curSize == 0;
}

template<typename T>
inline T lab::Stack<T>::pop() {
    return data[--curSize];
}

template<typename T>
inline void lab::Stack<T>::push(const T &data) {
    if (curSize == capacity) {
        doubleSpace();
    }
    this->data[curSize++] = data;
}

template<typename T>
inline void lab::Stack<T>::doubleSpace() {
    T *new_data = new T[capacity << 1];
    for (unsigned int i = 0; i < capacity; ++i) {
        new_data[i] = data[i];
    }
    capacity <<= 1;
    delete data;
    data = new_data;
}

#endif //LAB3_STACK_H

