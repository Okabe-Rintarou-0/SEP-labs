#include "termio.h"
#include "stack.h"
#include "operation.h"
#include <sstream>

// Do not delete the following line
char Termio::buffer[Termio::CANVAS_HEIGHT][Termio::CANVAS_WIDTH + 1];

unsigned int maxDisks = 0;

int anchor[] = {0, 5, 20, 35};

lab::Stack<int> rods[4];

lab::Stack<lab::Operation> operations;

bool usingEnhancedMode = false;

void write_hi() {
    for (int i = 0; i < 5; i++) {
        Termio::buffer[i][0] = '|';
        Termio::buffer[i][5] = '|';
        Termio::buffer[i][9] = '|';
        Termio::buffer[i][13] = '*';
    }
    Termio::buffer[3][13] = ' ';
    Termio::buffer[1][9] = ' ';
    for (int i = 1; i < 5; i++) {
        Termio::buffer[2][i] = '-';
    }
}

void drawLine(int row, int from, int to, char ch) {
    for (int i = from; i <= to; ++i)
        Termio::buffer[row][i] = ch;
}

void ui_push(int rodId, int size) {
    int orgSize = rods[rodId].size();
    int tgtRow = Termio::CANVAS_HEIGHT - (2 * orgSize + 2);
    drawLine(tgtRow, anchor[rodId] - size, anchor[rodId] + size, '*');
}

void ui_pop(int rodId, int size) {
    int orgSize = rods[rodId].size();
    int tgtRow = Termio::CANVAS_HEIGHT - (2 * orgSize);
    drawLine(tgtRow, anchor[rodId] - size, anchor[rodId] + size, ' ');
    Termio::buffer[tgtRow][anchor[rodId]] = '|';
}

void ui_move(int from, int to, int size) {
    ui_pop(from, size);
    ui_push(to, size);
}

void drawBottom() {
    strcpy(Termio::buffer[Termio::CANVAS_HEIGHT - 1], "-----|--------------|--------------|-----");
}

void drawRods() {
    for (int i = 1; i <= 3; ++i) {
        for (int j = 0; j < Termio::CANVAS_HEIGHT - 1; ++j) {
            Termio::buffer[j][anchor[i]] = '|';
        }
    }
}

void drawDishes() {
    for (unsigned int i = 1; i <= maxDisks; ++i) {
        int tgtRow = Termio::CANVAS_HEIGHT - (2 * i);
        int size = maxDisks - i + 1;
        drawLine(tgtRow, anchor[1] - size, anchor[1] + size, '*');
    }
}

void initializeUi() {
    Termio::ResetBuffer();
    drawBottom();
    drawRods();
    drawDishes();
    Termio::Draw();
}

void initialize() {
    usingEnhancedMode = false;
    operations.clear();

    for (int i = 1; i <= 3; ++i)
        rods[i].clear();
    for (int i = maxDisks; i >= 1; --i) {
        rods[1].push(i);
    }

    initializeUi();
}

void move(int from, int to) {
    ui_move(from, to, rods[from].top());
    rods[to].push(rods[from].pop());
}

void tryMove(int from, int to) {
    // if feasible, then move from ... to ...
    if (!rods[from].isEmpty() && (rods[to].isEmpty() || rods[from].top() < rods[to].top())) {
        move(from, to);
        operations.push(lab::Operation(from, to));
    }
}

void hanoi(int num, int from, int to, int buffer) {
    if (num <= 0) return;
    if (num == 1) {
        move(from, to);
        printf("Auto moving:%d->%d\n", from, to);
        Termio::Draw();
        return;
    }
    hanoi(num - 1, from, buffer, to);
    hanoi(1, from, to, buffer);
    hanoi(num - 1, buffer, to, from);
}

void enhancedHanoi(int num_2, int num_3) {
    hanoi(num_2 + num_3, 1, 2, 3);
    hanoi(num_3, 2, 3, 1);
}

void autoMove() {
    hanoi(maxDisks, 1, 2, 3);
    std::cout << "Congratulations! You win!" << std::endl;
}

void enhancedAutoMove(int num_2, int num_3) {
    enhancedHanoi(num_2, num_3);
    std::cout << "Congratulations! You win!" << std::endl;
}

bool checkEnd() {
    if (!usingEnhancedMode) {
        return rods[2].size() == maxDisks || rods[3].size() == maxDisks;
    }
    return false;
}

void restore() {
    while (!operations.isEmpty()) {
        lab::Operation op = operations.pop();
        int from = op.from, to = op.to;
        move(to, from);
        printf("Auto moving:%d->%d\n", to, from);
        Termio::Draw();
    }
}

int main() {
    Termio::Clear();
    Termio::ResetBuffer();

    std::string cmdline;
    while (true) {
        std::cout << "How many disks do you want? (1 ~ 5)" << std::endl;
        getline(cin, cmdline);
        stringstream ss(cmdline);
        ss >> maxDisks;

        if (cmdline == "Q") {
            return 0;
        }

        if (!ss.eof() || maxDisks > 5 || maxDisks < 1) {
            continue;
        }

        initialize();

        int para1, para2, para3;
        int from, to;
        while (true) {
            std::cout << "Move a disk. Format: x y" << std::endl;
            getline(cin, cmdline);
            stringstream ss(cmdline);
            ss >> para1 >> para2;
//            std::cout << "para1: " << para1 << ", para2: " << para2 << std::endl;
            if (ss.eof()) {
                from = para1, to = para2;
                if (from == 0 && to == 0) {
                    // auto-play
                    restore();
                    autoMove();
                    break;
                } else if (from >= 1 && from <= 3 && to >= 1 && to <= 3) {
                    tryMove(from, to);
                }
            } else {
                ss >> para3;
//                std::cout << para1 << " " << para2 << " " << para3 << std::endl;
                if (ss.eof()) {
                    if (para1 <= 0 && para2 <= 0 && para3 <= 0 && abs(para1 + para2 + para3) == (int) maxDisks) {
                        restore();
                        enhancedAutoMove(-para2, -para3);
                        break;
                    }
                }
            }
            Termio::Draw();
            if (checkEnd()) {
                std::cout << "Congratulations! You win!" << std::endl;
                break;
            }
        }
    }
}
