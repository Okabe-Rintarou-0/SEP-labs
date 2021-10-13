//
// Created by 92304 on 2021/10/13.
//

#ifndef LAB3_OPERATION_H
#define LAB3_OPERATION_H
#include <iostream>
namespace lab {
    class Operation {
    public:
        Operation(int from, int to) : from(from), to(to) {}

        Operation() = default;

        int from;
        int to;

        friend std::ostream &operator <<(std::ostream &os, const Operation &op) {
            return os << "(" << op.from << ", " << op.to;
        }
    };
}
#endif //LAB3_OPERATION_H
