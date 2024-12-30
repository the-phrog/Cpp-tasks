#include "GameOfLife.hpp"

ArgumentParser::ArgumentParser(int argc, char* argv[]) {
    parseArguments(argc, argv);
}

int ArgumentParser::getIterations() const {
    return iterations;
}

std::string ArgumentParser::getOutputFile() const {
    return outputFile;
}

std::string ArgumentParser::getInputFile() const {
    return inputFile;
}

void ArgumentParser::parseArguments(int argc, char* argv[]) {
    int iter(0), in(0), out(0);
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-I") {
            if (i + 1 < argc)
                parseIterations(argv[++i]);
            else
                throw std::invalid_argument("Missing value for iterations argument");
            iter++;
        }
        else if (arg.rfind("--iterations=", 0) == 0) {
            parseIterations(arg.substr(13));
            iter++;
        }
        else if (arg == "-i") {
            if (i + 1 < argc)
                inputFile = argv[++i];
            else
                throw std::invalid_argument("Missing value for input argument");
            in++;
        }
        else if (arg.rfind("--input=", 0) == 0) {
            inputFile = arg.substr(8);
            in++;
        }
        else if (arg == "-o") {
            if (i + 1 < argc)
                outputFile = argv[++i];
            else
                throw std::invalid_argument("Missing value for output argument");
            out++;
        }
        else if (arg.rfind("--output=", 0) == 0) {
            outputFile = arg.substr(9);
            out++;
        }
        else
            throw std::invalid_argument("Unknown argument: " + arg);
    }

    if (iterations == -1)
        throw std::invalid_argument("The iterations argument is required but not provided");
    if (iter > 1)
        throw std::invalid_argument("There must be 1 definition of iterations count");
    if (in > 1)
        throw std::invalid_argument("There must be 1 definition of input file name");
    if (out > 1)
        throw std::invalid_argument("There must be 1 definition of output file name");
    if (inputFile.empty())
        throw std::invalid_argument("The input argument is required but not provided");
    if (outputFile.empty())
        throw std::invalid_argument("The output argument is required but not provided");
}

void ArgumentParser::parseIterations(const std::string& value) {
    try {
        iterations = std::stoi(value);
    }
    catch (const std::exception&) {
        throw std::invalid_argument("Invalid value for iterations: " + value);
    }

    if (iterations <= 0)
        throw std::invalid_argument("Iterations must be a positive integer");
}