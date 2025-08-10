#pragma once
#include "Editor.h"

class Application {
public:
    Application (int argc, const char* argv[]) {
        std::string file_address = ParseArgs(argc, argv);
        editor = new Editor(file_address);
    }

    ~Application() {
        // TODO: replace with uniqueptr
        delete editor;
    }

    void Run() {
        editor->Run();
        editor->Cleanup();
    }
private:
    Editor *editor = nullptr;

private:
    std::string ParseArgs(int argc, const char* argv[]) {
        if (argc > 1) {
            return argv[1];
        }
        return "lorem.txt";
    }

};