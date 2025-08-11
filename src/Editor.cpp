#include "Editor.h"

void Editor::Init(const std::string& file_address) {
    // TODO: load from file
    rawText = "Hi hello bye!";

#pragma region rawText
    rawText = R"(1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
    )";
#pragma endregion

    editorData = std::make_unique<EditorData>(rawText);

    // Initial Cursor Position
    cursorPosition.x = config.lineNumberWidth + config.spaceBetweenNumbersAndText;
    lastCursorPositionX = cursorPosition.x;
    cursorPosition.y = 0;

    // Window Creation
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(config.windowWidth, config.windowHeight, config.applicationName.c_str());

    // Set FPS limit
    SetTargetFPS(60);

    // Load Fonts (This has to happen after window initialization)
    font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                      config.fontSize, nullptr, 0);

    // Set grid dimensions based on font
    config.gridWidth = MeasureTextEx(font, "A", config.fontSize, 0).x;
    config.gridHeight = MeasureTextEx(font, "A", config.fontSize, 0).y;
}

void Editor::Cleanup() {
    UnloadFont(font);
    CloseWindow();
}

// TODO: Don't use hardcoded keybinds. Read keybinds from a config file
void Editor::Update() {
    // Debugging
    {
        // Toggle debug grid (Ctrl+G)
        HandleDebugGrid();
        // Toggle debug info (Ctrl+H)
        HandleDebugInfo();
    }

    // Appearance
    {
        // Zoom (Ctrl+Q and Ctrl+E)
        HandleZoom();
        // Change vertical spacing (Shift+E and Shift+Q)
        HandleVerticalSpacing();
        //Toggle line numbers (Ctrl+L)
        HandleLineNumbers();
    }

    // Cursor (Cadet)
    {
        // Cursor movement
        HandleCursorMovement();
        // Handle mouse click
        HandleMouseClick();
    }

    // Text editing (typing, deleting, etc.)
    {
        // Insert character by typing
        HandleTyping();
        // Delete character by pressing backspace
        HandleBackspace();
        // new line by pressing enter
        HandleNewLine();
        // Home and End keys
        HandleHomeEnd();
        // Delete line
        HandleLineDelete();
        // Tab key
        HandleTab();
        // Scroll wheel
        HandleScrollWheel();
        // Calculate viewport sizes
        CalculateViewportSizes();
    }

    // Cleanup and clamp work; TODO: Not ideal, change this
    {
        // if the cursor is out of bounds, clamp it to the current line
        // width
        if (cursorPosition.x > editorData->getLine(cursorPosition.y).size() +
                                   config.lineNumberWidth +
                                   config.spaceBetweenNumbersAndText) {
            // Save the last cursor position.x
            lastCursorPositionX =
                std::max(lastCursorPositionX, cursorPosition.x);

            // Clamp cursor x position to the current line width
            cursorPosition.x = editorData->getLine(cursorPosition.y).size() +
                               config.lineNumberWidth +
                               config.spaceBetweenNumbersAndText;
        }

        // reload the last cursor position.x if possible
        if (lastCursorPositionX >= cursorPosition.x &&
            lastCursorPositionX <=
                editorData->getLine(cursorPosition.y).size() +
                    config.lineNumberWidth +
                    config.spaceBetweenNumbersAndText) {
        }
    }
}

void Editor::Draw() {
    BeginDrawing();
    ClearBackground(config.editorBackgroundColor);

    // Draw the working grid (this is a grid that every character can be placed on)
    DrawDebugGrid();
    // Draw the text character by character
    DrawText();
    // Draw the line numbers
    DrawLineNumbers();
    // Draw the cursor
    DrawCursor();
    // Draw FPS
    if (config.drawFPS) {
        DrawFPS(GetScreenWidth() - 110, 10);
    }
    // Draw debug info (viewportStart, viewportLineCount, cursorPosition, etc)
    DrawDebugInfo();

    EndDrawing();
}

int Editor::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    // TODO: return -1 if something went wrong
    return 0;
}
