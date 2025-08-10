#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

class EditorData {
    
public:
    std::vector<std::string> lines;

public:
    EditorData() {}
    EditorData(std::string text) { lines = splitLines(text); }
    ~EditorData() = default;

    int getNumLines() { return lines.size(); }

    std::string getLine(int line_number) { return lines[line_number]; }

    void setLine(int line_number, std::string line) {
        lines[line_number] = line;
    }

    void insertLine(int line_number, std::string line) {
        // TODO: bounds checking
        if (line_number > lines.size()) {
            return;
        }
        lines.insert(lines.begin() + line_number, line);
    }

    void deleteLine(int line_number) {
        lines.erase(lines.begin() + line_number);
    }

    void insertChar(int line_number, int char_number, char c) {
        // TODO: bounds checking
        if (line_number > lines.size() ||
            char_number > lines[line_number].size()) {
            return;
        }

        if (c == '\t') {
            c = ' ';
            for (int i = 0; i < 4; i++) {
                lines[line_number].insert(
                    lines[line_number].begin() + char_number + i, c);
            }
        } else {
            lines[line_number].insert(lines[line_number].begin() + char_number,
                                      c);
        }
    }

    void deleteChar(int line_number, int char_number) {
        // delete a single character from a vector of characters
        lines[line_number].erase(lines[line_number].begin() + char_number);
    }

    void insertLineBreak(int line_number, int char_number) {
        // we should break this line into two lines at the given char_number
        std::string line = lines[line_number];
        lines[line_number] = line.substr(0, char_number);
        lines.insert(lines.begin() + line_number + 1, line.substr(char_number));
    }

    void printText() {
        for (int i = 0; i < lines.size(); i++) {
            std::cout << lines[i] << std::endl;
        }
    }
};