#include "GameOfLife.hpp"

Keeper::Keeper() : name("default") {}

Keeper::Keeper(int x, int y, std::string n) : fld(std::vector<std::vector<bool>>(y, std::vector<bool>(x, false))), name(n) {}

Keeper::Keeper(int x, int y, std::list<std::pair<int, int>> list, std::string n) : Keeper(x, y, n) {
    for (auto i : list)
        fld[i.second][i.first] = true;
}

Keeper::Keeper(const Keeper &f) : fld(f.fld), name(f.name) {}

std::vector<std::vector<bool>> Keeper::get() {
    return fld;
}

const std::vector<std::vector<bool>> Keeper::get() const {
    return fld;
}

std::string Keeper::getName() {
    return name;
}

const std::string Keeper::getName() const{
    return name;
}

std::vector<bool>& Keeper::operator[](int i) {
    return fld[i];
}

const std::vector<bool>& Keeper::operator[](int i) const {
    return fld[i];
}

Keeper& Keeper::operator=(const Keeper &k) {
    Keeper k1 = k;
    fld = k1.fld;
    name = k1.name;
    return *this;
}

bool operator==(const Keeper &k1, const Keeper &k2) {
    return k1.get() == k2.get();
}

bool operator!=(const Keeper &k1, const Keeper &k2) {
    return !(k1 == k2);
}
