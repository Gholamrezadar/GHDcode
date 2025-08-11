#pragma once

#include <math.h>
#include <raylib.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "EditorConfig.h"
#include "EditorData.h"

class Editor {
   public:
    Editor() {
        Init("lorem.txt");
    }

    Editor(const std::string& file_address) {
        Init(file_address);
    }

    ~Editor() {}

    void Init(const std::string& file_address);
    int Run();
    void Update();
    void Draw();
    void Cleanup();

   private:
    // Config
    EditorConfig config;
    Font font;
    // TODO: fontCache

    // Text data
    std::unique_ptr<EditorData> editorData = nullptr;
    std::string rawText = "";

    int viewportStart = 0;
    int viewportLineCount = 17;
    float lastCursorPositionX = 0.0f;  // why float?
    Vector2 cursorPosition = {0.0f, 0.0f};

   private:
    // Controls (called in Update())
    void HandleDebugGrid();
    void HandleDebugInfo();
    void HandleZoom();
    void HandleVerticalSpacing();
    void HandleLineNumbers();
    void HandleCursorMovement();
    void HandleMouseClick();
    void HandleTyping();
    void HandleBackspace();
    void HandleDelete();
    void HandleNewLine();
    void HandleTab();
    void HandleHomeEnd();
    void HandleLineDelete();

    // Drawing functions (called in Draw())
    void DrawDebugGrid();
    void DrawDebugInfo();
    void DrawCursor();
    void DrawLineNumbers();
    void DrawText();
};