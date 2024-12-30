#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <regex>
#include <algorithm>


// Forward declarations of classes
class Keeper;
class Calculator;
class Interface;
class ArgumentParser;
class FileParser;
class WReader;

// Keeper: Stores the universe field and its name
class Keeper {
    // field
    std::vector<std::vector<bool>> fld;
    // name
    std::string name;
public:
    // default constructor
    Keeper();
    // empty universe constructor by field size and name
    Keeper(int x, int y, std::string n);
    // constructor by field size, coordinates and name
    Keeper(int x, int y, std::list<std::pair<int, int>> list, std::string n);
    // copy constructor
    Keeper(const Keeper &f);
    // returns field
    std::vector<std::vector<bool>> get();
    // returns field
    const std::vector<std::vector<bool>> get() const ;
    // returns universe name
    std::string getName();
    // returns universe name
    const std::string getName() const;
    std::vector<bool> &operator[](int i);
    const std::vector<bool> &operator[](int i) const;
    Keeper &operator= (const Keeper &k);
};

bool operator==(const Keeper &k1, const Keeper &k2);
bool operator!=(const Keeper &k1, const Keeper &k2);

// circular incrementor
int next(int i, int max);
// circular decrementor
int prev(int i, int max);

// Calculator: Handles the rules and calculates new field states
class Calculator {
    // contains survival rule
    std::vector<bool> survival;
    // contains birth rule
    std::vector<bool> birth;
    // calculates whether the cell will be alive
    bool calculate_cell(const Keeper k, int x, int y);
public:
    // default constructor
    Calculator();
    // constructor by lists of numbers
    Calculator(std::vector<int> s, std::vector<int> b);
    // constructor by lists of numbers
    Calculator(std::list<int> s, std::list<int> b);
    // returns birth rule
    std::vector<bool> birth_rule();
    // returns birth rule
    const std::vector<bool> birth_rule() const;
    // returns survival rule
    std::vector<bool> survival_rule();
    // returns survival rule
    const std::vector<bool> survival_rule() const;
    // calculates an iteration by given transitition rules
    Keeper calculate(const Keeper k1);
    // calculates n iterations by given transitition rules
    Keeper calculate(const Keeper k1, unsigned int n);
    Calculator &operator= (const Calculator &k);
};
bool operator==(const Calculator &c1, const Calculator &c2);
bool operator!=(const Calculator &c1, const Calculator &c2);

// ArgumentParser: Parses command-line arguments
class ArgumentParser {
public:
    // constructor by command-line arguments.
    // starts parsing.
    ArgumentParser(int argc, char* argv[]);
    // returns count of iterations
    int getIterations() const;
    // returns output filename
    std::string getOutputFile() const;
    // returns input  filename
    std::string getInputFile() const;
private:
    // count of iterations
    int iterations;
    // output filename
    std::string outputFile;
    // input  filename
    std::string inputFile;
    // parses command-line arguments
    void parseArguments(int argc, char* argv[]);
    // parses iterations
    void parseIterations(const std::string& value);
};

// WReader: Reads and writes game data from (to) files
class WReader {
    // parses sequences of numbers in 4-th line of .life-file
    std::list<int> parseRulePart(const std::string& rulePart);
public:
    // default contructor
    WReader();
    // reads .life (or .lif) file
    Keeper readFile(const std::string& fileName, Calculator* calc);
    // makes .life (or .lif) file from given university and transitition rules
    void writeFile(const std::string& filename, const Keeper k, const Calculator calc);
};

// Interface: Handles terminal I/O
class Interface {
public:
    // start the game
    void game(int argc, char* argv[], std::string start_file);
    // print universe
    void print(Keeper k);
    // print universe with rules
    void print(Keeper k, Calculator calc);
};

#endif // GAMEOFLIFE_H