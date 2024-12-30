#include "GameOfLife.hpp"

int next(int i, int max) {
    return ((i + 1 < max) ? i + 1 : 0);
}

int prev(int i, int max) {
    return ((i - 1 >= 0) ? i - 1 : max - 1);
}

Calculator::Calculator() : survival(9, false), birth(9, false) {
    for (int i : {2, 3}) survival[i] = true;
    for (int i : {3})    birth[i] = true;
}

Calculator::Calculator(std::vector<int> s, std::vector<int> b) : survival(9, false), birth(9, false) {
    for (int i : s) survival[i] = true;
    for (int i : b) birth[i] = true;
}

Calculator::Calculator(std::list<int> s, std::list<int> b) : survival(9, false), birth(9, false) {
    for (int i : s) survival[i] = true;
    for (int i : b) birth[i] = true;
}

std::vector<bool> Calculator::birth_rule() {
    return birth;
}

const std::vector<bool> Calculator::birth_rule() const{
    return birth;
}

std::vector<bool> Calculator::survival_rule() {
    return survival;
}

const std::vector<bool> Calculator::survival_rule() const{
    return survival;
}

Keeper Calculator::calculate(const Keeper k1) {
    Keeper k2 = k1;
    int X = k2[0].size();
    int Y = k2.get().size();

    for (int y = 0; y < Y; y++)
        for (int x = 0; x < X; x++)
            k2[y][x] = calculate_cell(k1, x, y);

    return k2;
}

Keeper Calculator::calculate(const Keeper k1, unsigned int n) {
    Keeper k2 = k1;
    for (int i = 0; i < n; i++)
        k2 = calculate(k2);
    return k2;
}

bool Calculator::calculate_cell(const Keeper k, int x, int y) {
    int X = k[0].size();
    int Y = k.get().size();
    int alive = 0;
    bool res = false;

    for (int i : {prev(x, X), x, next(x, X)})
        for (int j : {prev(y, Y), y, next(y, Y)}) {
            if (i == x && j == y) continue;
            if (k[j][i]) alive++;
        }

    if (k[y][x])
        res = survival[alive];
    else
        res = birth[alive];

    return res;
}

Calculator& Calculator::operator=(const Calculator &c) {
    Calculator c1 = c;
    birth = c1.birth;
    survival = c1.survival;
    return *this;
}

bool operator==(const Calculator &c1, const Calculator &c2){
    return (c1.birth_rule()==c2.birth_rule()&&c1.survival_rule()==c2.survival_rule());
}
bool operator!=(const Calculator &c1, const Calculator &c2){
    return !(c1 == c2);
}

