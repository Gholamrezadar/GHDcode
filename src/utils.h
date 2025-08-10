#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

std::string leftPad(std::string str, int length, char padChar = ' ');
std::string rightPad(std::string str, int length, char padChar = ' ');
std::vector<std::string> splitLines(const std::string& input);
std::string readFile(const std::string& filename);
void writeFile(const std::string& filename, const std::string& content);
std::string readFileFromCommandLine(int argc, char** argv);