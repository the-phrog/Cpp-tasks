#include "GameOfLife.hpp"

WReader::WReader() {}

std::list<int> WReader::parseRulePart(const std::string& rulePart) {
    std::list<int> result;
    for (char c : rulePart)
        if (isdigit(c))
            result.push_back((int)(c - '0'));
    return result;
}

Keeper WReader::readFile(const std::string& fileName, Calculator* calc) {
    std::ifstream file(fileName);
    if (!file.is_open())
        throw std::runtime_error("Couldn't open the file: " + fileName);

    std::string name;
    std::string line;
    int x_size, y_size;
    std::list<std::pair<int,int>> cells;
    int lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;

        line.erase(line.find_last_not_of(" \t") + 1);
        if (line.empty()) continue;

        if (lineNumber == 1) {
            if (line != "#Life 1.06") 
                throw std::runtime_error("Incorrect title in line 1. Expected: #Life 1.06");
        }
        else if (lineNumber == 2) {
            if (line.rfind("#N ", 0) != 0)
                throw std::runtime_error("Incorrect format of line 2. Expected: #N <name of the universe>");
            name = line.substr(3);
            name.erase(0, name.find_first_not_of(" \t"));
        }
        else if (lineNumber == 3) {
            if (line.rfind("#S ", 0) != 0)
                throw std::runtime_error("Incorrect format of line 3. Expected: #S <x y>");
            line = line.substr(3);
            std::istringstream iss(line);
            if (!(iss >> x_size >> y_size))
                throw std::runtime_error("Error parsing universe size.");
        }
        else if (lineNumber == 4) {
            if (line.rfind("#R ", 0) != 0)
                throw std::runtime_error("Incorrect format of line 4. Expected: #R Bx/Sy");
            std::string rule = line.substr(3);
            rule.erase(0, rule.find_first_not_of(" \t"));
            std::regex ruleRegex("B([0-8]*)/S([0-8]*)");
            std::smatch match;
            if (!regex_match(rule, match, ruleRegex))
                throw std::runtime_error("Incorrect transitition rule format in line 4");
            std::list<int> birth = parseRulePart(match[1].str());
            std::list<int> survival = parseRulePart(match[2].str());
            *calc = Calculator(survival, birth);
        }
        else {
            std::istringstream iss(line);
            int x, y;
            if (!(iss >> x >> y))
                throw std::runtime_error("Error parsing coordinates in a string " + std::to_string(lineNumber));
            if(x < 0 || x >= x_size || y < 0 || y >= y_size)
                throw std::runtime_error("Value error: coordinates must be in range [0, size)");
            cells.emplace_back(x, y);
        }
    }

    file.close();
    Keeper k(x_size, y_size, cells, name);
    return k;
}

void WReader::writeFile(const std::string& filename, const Keeper k, const Calculator calc) {
    std::ofstream file(filename);
     if (!file.is_open())
        throw std::runtime_error("Couldn't open the file: " + filename);
    file << "#Life 1.06" << std::endl;
    file << "#N " << k.getName() << std::endl;
    file << "#S " << k[0].size() << " " << k.get().size() << std::endl;
    file << "#R B";
    for (int i = 0; i < 9; i++)
        if (calc.birth_rule()[i])
            file << i;
    file << "/S";
    for (int i = 0; i < 9; i++)
        if (calc.survival_rule()[i])
            file << i;
    file << std::endl;

    for (int y = 0; y < k.get().size(); y++)
        for (int x = 0; x < k[0].size(); x++)
            if (k[y][x]) file << x << " " << y << std::endl;
}