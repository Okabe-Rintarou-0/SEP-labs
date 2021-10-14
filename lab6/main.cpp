#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include "lexicon.h"

typedef std::pair<int, int> Pair;

int currentScore = 0;
int computerScore = 0;
std::vector <std::string> myWords;
std::vector <std::string> board;
std::set <std::string> computerWords;
std::map<char, std::vector<Pair>> letterPos;
std::set <std::string> foundWords;
const std::string filepath = "./EnglishWords.txt";
Lexicon lexicon;
int n;
int next[8][2] = {
        {0,  1},
        {0,  -1},
        {1,  0},
        {-1, 0},
        {1,  1},
        {-1, 1},
        {1,  -1},
        {-1, -1}
};

void initLetterPos() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            letterPos[board[i][j]].push_back(std::make_pair(i, j));
        }
    }
}

inline bool outOfBound(int x, int y) {
    return x < 0 || x >= n || y < 0 || y >= n;
}

bool dfs(Pair pos, int cursor, const std::string &tgt, std::set <Pair> &visited) {
    int curX = pos.first;
    int curY = pos.second;

    if (tgt[cursor] != board[curX][curY]) return false;

//    std::cout << "current pos: (" << curX << ", " << curY << ")" << std::endl;
//    std::cout << "current char: " << board[curX][curY] << std::endl;

    if (cursor == tgt.size() - 1) {
        return true;
    }

    visited.insert(pos);
    int nextX, nextY;
    for (int i = 0; i < 8; ++i) {
        nextX = curX + next[i][0];
        nextY = curY + next[i][1];
        if (!outOfBound(nextX, nextY) && visited.find(std::make_pair(nextX, nextY)) == visited.end()) {
            if (dfs(std::make_pair(nextX, nextY), cursor + 1, tgt, visited))
                return true;
        }
    }
    visited.erase(pos);
    return false;
}

std::string toUppercase(const std::string &str) {
    std::string ret(str);
    for (char &ch:ret) {
        if (ch >= 'a' && ch <= 'z')
            ch = 'A' + ch - 'a';
    }
    return ret;
}

std::string toLowercase(const std::string &str) {
    std::string ret(str);
    for (char &ch:ret) {
        if (ch >= 'A' && ch <= 'Z')
            ch = 'a' + ch - 'A';
    }
    return ret;
}

void showContent() {
    for (const std::string &row :board) {
        std::cout << row << std::endl;
    }
    printf("Your Score: %d\n", currentScore);
    std::cout << "Your Words: ";
    for (const std::string &word:myWords) {
        std::cout << toLowercase(word) << " ";
    }
    std::cout << std::endl;
}

bool searchWord(const std::string &word) {
    std::set <Pair> visited;
    std::string uppercaseWord = toUppercase(word);
    for (const Pair &pair:letterPos[uppercaseWord[0]]) {
        if (dfs(pair, 0, uppercaseWord, visited))
            return true;
    }
    return false;
}

inline bool tooShort(const std::string &word) {
    return word.size() < 4;
}

void dfs(std::string &curStr, Pair curPos, std::set <Pair> &visited) {
    if (lexicon.containsPrefix(curStr)) {
        if (lexicon.contains(curStr) && !tooShort(curStr)
            && foundWords.find(toLowercase(curStr)) == foundWords.end() &&
            computerWords.find(curStr) == computerWords.end()) {
//            std::cout << "found word: " << curStr << std::endl;
            computerWords.insert(curStr);
            computerScore += curStr.size() - 3;
        }
    } else
        return;

    int curX = curPos.first;
    int curY = curPos.second;
    int nextX, nextY;
    visited.insert(curPos);
    for (int i = 0; i < 8; ++i) {
        nextX = curX + next[i][0];
        nextY = curY + next[i][1];
        if (!outOfBound(nextX, nextY) && visited.find(std::make_pair(nextX, nextY)) == visited.end()) {
            curStr += board[nextX][nextY];
            dfs(curStr, std::make_pair(nextX, nextY), visited);
            curStr.erase(curStr.end() - 1);
        }
    }
    visited.erase(curPos);
}

void findByComputer() {
    std::set <Pair> visited;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::string str;
            str += board[i][j];
            visited.clear();
            dfs(str, std::make_pair(i, j), visited);
        }
    }

    printf("Computer Score: %d\n", computerScore);
    std::cout << "Computer Words:";
    for (const std::string &word :computerWords) {
        std::cout << " " << word;
    }
    std::cout << std::endl;
}

int main() {
    lexicon.addWordsFromFile(filepath);
    std::cin >> n;
    board.assign(n, std::string());

    for (int i = 0; i < n; ++i) {
        std::cin >> board[i];
    }
    initLetterPos();

    std::string word;
    while (true) {
        showContent();
        std::cin >> word;
        if (word == "???") {
            findByComputer();
            break;
        }

        if (tooShort(word)) {
            printf("%s is too short.\n", word.c_str());
            continue;
        }

        if (!lexicon.contains(word)) {
            printf("%s is not a word.\n", word.c_str());
            continue;
        }

        if (!searchWord(word)) {
            printf("%s is not on board.\n", word.c_str());
            continue;
        } else {
            word = toLowercase(word);
            if (foundWords.find(word) == foundWords.end()) {
                myWords.push_back(word);
                foundWords.insert(word);
                currentScore += word.size() - 3;
            } else {
                printf("%s is already found.\n", word.c_str());
            }
        }
    }
}

/* Test data:
5
EEIRD
AGMRS
CIILN
DLOTE
FRWOT
 */