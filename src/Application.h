#pragma once
#include "Editor.h"

class Application {
public:
    Application (int argc, const char* argv[]) {
        std::string file_address = ParseArgs(argc, argv);
        editor = std::make_unique<Editor>(file_address);
    }

    ~Application() {
    }

    void Run() {
        editor->Run();
        editor->Cleanup();
    }
private:
    std::unique_ptr<Editor> editor = nullptr;

private:
    std::string ParseArgs(int argc, const char* argv[]) {
        if (argc > 1) {
            return argv[1];
        }
        return "lorem.txt";
    }

};