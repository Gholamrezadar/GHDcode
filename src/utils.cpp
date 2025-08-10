#include "utils.h"

std::string leftPad(std::string str, int length, char padChar)
{
    if (str.length() >= length)
        return str;
    return std::string(length - str.length(), padChar) + str;
}

std::string rightPad(std::string str, int length, char padChar)
{
    if (str.length() >= length)
        return str;
    return str + std::string(length - str.length(), padChar);
}

std::vector<std::string> splitLines(const std::string& input) {
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;
    
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

// file utilities //

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
}

std::string readFileFromCommandLine(int argc, char** argv) {
    if (argc < 2) {
        return "";
    }
    return readFile(argv[1]);
}