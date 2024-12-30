#include "GameOfLife.hpp"

void Interface::game(int argc, char* argv[], std::string start_file) {
    WReader rw;
    Keeper k;
    Calculator calc;

    if (argc > 1) {
        ArgumentParser parser(argc, argv);

        std::string input = parser.getInputFile();
        std::string output = parser.getOutputFile();
        int iterations = parser.getIterations();

        k = rw.readFile(input, &calc);
        k = calc.calculate(k, iterations);
        rw.writeFile(output, k, calc);
    } else {
        k = rw.readFile(start_file, &calc);
        std::string line;
        std::string args;
        this->print(k, calc);
        while (true) {
            getline(std::cin, line);
            line.erase(line.find_last_not_of(" \t") + 1);

            if (line.empty()) {
                k = calc.calculate(k);
                this->print(k, calc);
                continue;
            }

            if (line.find("dump ") == 0) {
                args = line.substr(5);
                args.erase(0, args.find_first_not_of(" \t"));
                rw.writeFile(args, k, calc);
                std::cout << "dumped." << std::endl;
            } 
            else if (line == "tick") {
                k = calc.calculate(k);
                this->print(k, calc);
            } 
            else if (line.find("tick ") == 0) {
                args = line.substr(5);
                args.erase(0, args.find_first_not_of(" \t"));

                int n;
                try {
                    n = stoi(args);
                }
                catch (const std::exception&) {
                    throw std::invalid_argument("Invalid value for iterations: " + args);
                }

                k = calc.calculate(k, n);
                this->print(k, calc);
            }
            else if (line == "t") {
                k = calc.calculate(k);
                this->print(k, calc);
            }
            else if (line.find("t ") == 0) {
                args = line.substr(2);
                args.erase(0, args.find_first_not_of(" \t"));

                int n;
                try {
                    n = stoi(args);
                }
                catch (const std::exception&) {
                    throw std::invalid_argument("Invalid value for iterations: " + args);
                }

                k = calc.calculate(k, n);
                this->print(k, calc);
            }
            else if (line == "exit")
                break;
            else if (line == "help") {
                std::cout << "dump <filename> - save the universe to a file" << std::endl;
                std::cout << "tick <n=1> (t <n=1>) - calculate n iterations (default is 1)" << std::endl;
                std::cout << "and print the universe, it's name and rule of transition" << std::endl;
                std::cout << "exit - exit the game" << std::endl;
                std::cout << "help - print the help about the commands" << std::endl;
            } 
            else {
                throw std::runtime_error("Unknown command");
            }
        }
    }
}

void Interface::print(Keeper k) {
    std::cout << k.getName() << std::endl;

    for (int y = 0; y < k.get().size(); y++) {
        for (int x = 0; x < k[0].size(); x++)
            std::cout << (k[y][x] ? "O" : ".") << " ";
        std::cout << std::endl;
    }
}

void Interface::print(Keeper k, Calculator calc) {
    std::cout << k.getName() << std::endl;

    std::cout << "B";
    for (int i = 0; i < 9; i++)
        if (calc.birth_rule()[i]) std::cout << i;
    std::cout << "/S";
    for (int i = 0; i < 9; i++)
        if (calc.survival_rule()[i]) std::cout << i;
    std::cout << std::endl;

    for (int y = 0; y < k.get().size(); y++) {
        for (int x = 0; x < k[0].size(); x++)
            std::cout << (k[y][x] ? "O" : ".") << " ";
        std::cout << std::endl;
    }
}